#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using val_t = int;

struct Coord {
  val_t x;
  val_t y;

  bool operator==(const Coord &other) const {
    return x == other.x && y == other.y;
  }
};

enum class DIR {
  UP,
  LEFT,
  RIGHT,
  DOWN,
};

struct Problem {
  const std::vector<std::vector<char>> grid;
  const int nrows, ncols;
  const Coord start, end;

  auto get_coord(const Coord &coord, const DIR dir) const {
    Coord new_coord;
    switch (dir) {
    case DIR::UP:
      new_coord = {coord.x, coord.y - 1};
      break;
    case DIR::RIGHT:
      new_coord = {coord.x + 1, coord.y};
      break;
    case DIR::DOWN:
      new_coord = {coord.x, coord.y + 1};
      break;
    case DIR::LEFT:
      new_coord = {coord.x - 1, coord.y};
      break;
    }
    return new_coord;
  }

  bool check_move(const Coord &coord) const {
    const auto [x, y] = coord;
    const auto inbounds = x > 0 && x < nrows && y > 0 && y < ncols;
    const auto valid_square = grid[y][x] != '#';
    return inbounds && valid_square;
  }
};

auto read_file(const std::string &fname) -> Problem {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::string line;
  std::vector<std::vector<char>> grid;
  Coord start, end;

  int row_num = 0;

  while (std::getline(f, line)) {
    if (auto it = std::ranges::find(line, 'S'); it != line.end()) {
      const auto col_num = std::distance(line.begin(), it);
      start = {static_cast<val_t>(col_num), row_num};
    }
    if (auto it = std::ranges::find(line, 'E'); it != line.end()) {
      const auto col_num = std::distance(line.begin(), it);
      end = {static_cast<val_t>(col_num), row_num};
    }
    auto ss = std::istringstream(line);
    auto row =
        std::views::istream<char>(ss) | std::ranges::to<std::vector<char>>();
    grid.push_back(row);
    row_num += 1;
  }
  int nrows = grid.size();
  int ncols = grid.front().size();
  // fmt::println("start = ({}, {})", start.x, start.y);
  // fmt::println("end = ({}, {})", end.x, end.y);
  // fmt::println("grid = {}", grid);
  return {grid, nrows, ncols, start, end};
}

struct Pos {
  Coord coord;
  DIR dir;
  bool operator==(const Pos &other) const {
    return coord == other.coord && dir == other.dir;
  }
};

namespace std {
template <> struct hash<Coord> {
  size_t operator()(const Coord &p) const {
    size_t h1 = hash<int>()(p.x);
    size_t h2 = hash<int>()(p.y);
    return h1 ^ (h2 << 1);
  }
};
template <> struct hash<Pos> {
  size_t operator()(const Pos &p) const {
    size_t h1 = hash<int>()(p.coord.x);
    size_t h2 = hash<int>()(p.coord.y);
    size_t h3 = hash<int>()(static_cast<int>(p.dir));

    return (h1 ^ (h2 << 1)) ^ (h3 << 1);
  }
};
} // namespace std

auto solve_problem(const auto &problem)
    -> std::pair<int, std::unordered_map<Pos, int>> {
  const auto &[cost, nrows, ncols, start, end] = problem;
  std::unordered_map<Pos, int> visited;
  auto cmp = [](const auto &a, const auto &b) { return a.second > b.second; };
  std::priority_queue<std::pair<Pos, int>, std::vector<std::pair<Pos, int>>,
                      decltype(cmp)>
      queue;

  const Pos begin = {start, DIR::RIGHT};
  visited[begin] = 0;
  queue.push({begin, 0});

  const std::array<DIR, 4> dirs{DIR::UP, DIR::RIGHT, DIR::DOWN, DIR::LEFT};

  while (!queue.empty()) {
    const auto curr = queue.top();
    queue.pop();
    const auto [x, y] = curr.first.coord;
    const auto curr_dir = curr.first.dir;
    const auto curr_cost = visited.at({curr.first.coord, curr_dir});
    if (problem.grid[y][x] == 'E') {
      return {curr_cost, visited};
    }

    for (const auto new_dir : dirs) {
      const auto new_coord = problem.get_coord(curr.first.coord, new_dir);
      if (problem.check_move(new_coord)) {
        auto new_cost = curr_cost + ((curr_dir == new_dir) ? 1 : 1001);
        if (visited.contains({new_coord, new_dir}) &&
            visited.at({new_coord, new_dir}) < new_cost) {
          continue;
        }
        visited.insert({{new_coord, new_dir}, new_cost});
        queue.push({{new_coord, new_dir}, new_cost});
      }
    }
  }
  return {0, visited};
}

auto backtrack(const auto &costs, const auto &curr, const auto &start) -> int {
  fmt::println("({}, {})", curr.x, curr.y);
  int spots = 0;
  if (curr == start) {
    return 1;
  }
  const std::array<DIR, 4> dirs{DIR::UP, DIR::RIGHT, DIR::DOWN, DIR::LEFT};
  // get minimum
  int min = 100000;
  for (const auto dir : dirs) {
    Coord new_coord;
    switch (dir) {
    case DIR::UP:
      new_coord = {curr.x, curr.y - 1};
      break;
    case DIR::RIGHT:
      new_coord = {curr.x + 1, curr.y};
      break;
    case DIR::DOWN:
      new_coord = {curr.x, curr.y + 1};
      break;
    case DIR::LEFT:
      new_coord = {curr.x - 1, curr.y};
      break;
    }
    if (costs.count(new_coord) != 0) {
      min = std::min(min, costs.at(new_coord));
      fmt::println("new_coord: ({}, {}) = {}", new_coord.x, new_coord.y,
                   costs.at(new_coord));
    }
  }
  for (const auto dir : dirs) {
    Coord new_coord;
    switch (dir) {
    case DIR::UP:
      new_coord = {curr.x, curr.y - 1};
      break;
    case DIR::RIGHT:
      new_coord = {curr.x + 1, curr.y};
      break;
    case DIR::DOWN:
      new_coord = {curr.x, curr.y + 1};
      break;
    case DIR::LEFT:
      new_coord = {curr.x - 1, curr.y};
      break;
    }
    if (costs.count(new_coord) != 0 && costs.at(new_coord) == min) {
      spots += backtrack(costs, new_coord, start);
    }
  }
  return spots;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto problem = read_file(argv[1]);
  const auto [cost1, visited] = solve_problem(problem);
  fmt::println("part1 = {}", cost1);
  std::unordered_map<Coord, int> costs;
  for (const auto &[key, value] : visited) {
    const auto &[coord, dir] = key;
    fmt::println("({}, {}) = {}", coord.x, coord.y, value);
    if (costs.count(coord) != 0) {
      fmt::println("Found duplicate at ({}, {})", coord.x, coord.y);
      exit(1);
    }
    costs.insert({coord, value});
  }
  const auto places = backtrack(costs, problem.end, problem.start);
  fmt::println("cost = {}", places);
}
