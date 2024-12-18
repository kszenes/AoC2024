#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <numeric>
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
  std::vector<std::vector<char>> grid;
  const int nrows = 71;
  const int ncols = 71;
  const Coord start = {0, 0};
  const Coord end = {70, 70};

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
    const auto inbounds = x >= 0 && x < nrows && y >= 0 && y < ncols;
    if (inbounds) {
      const auto valid_square = grid[y][x] != '#';
      return valid_square;
    } else {
      return false;
    }
  }

  void print() const {
    for (const auto &row : grid) {
      fmt::println("{}", row);
    }
  }
};

auto read_file(const std::string &fname) -> std::vector<Coord> {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::string line;
  std::vector<Coord> obstacles;
  Coord start, end;

  while (std::getline(f, line)) {
    const auto obstacle = std::views::split(line, ',') |
                          std::views::transform([](const auto &e) {
                            return std::stoi(std::string(std::string_view(e)));
                          }) |
                          std::ranges::to<std::vector<int>>();
    const auto x = obstacle.front();
    const auto y = obstacle.back();
    obstacles.push_back({x, y});
  }

  return {obstacles};
}

auto build_problem(const auto &obs, const int steps) -> Problem {
  Problem p;
  std::vector<std::vector<char>> grid;

  for (int row = 0; row < p.nrows; ++row) {
    grid.push_back(std::vector<char>(p.ncols, '.'));
  }

  int i = 0;
  for (const auto &o : obs) {
    const auto [x, y] = o;
    grid.at(y).at(x) = '#';
    ++i;
    if (i >= steps) {
      break;
    }
  }

  p.grid = grid;
  return p;
}

namespace std {
template <> struct hash<Coord> {
  size_t operator()(const Coord &p) const {
    size_t h1 = hash<int>()(p.x);
    size_t h2 = hash<int>()(p.y);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std

auto solve_problem(const auto &problem) -> std::vector<std::vector<int>> {
  const auto &[cost, nrows, ncols, start, end] = problem;
  std::vector<std::vector<int>> visited;
  for (int row = 0; row < nrows; ++row) {
    visited.emplace_back(ncols, 0);
  }
  std::queue<Coord> queue;

  const Coord begin = start;
  visited[begin.y][begin.x] = 1;
  queue.push(begin);

  const std::array<DIR, 4> dirs{DIR::UP, DIR::RIGHT, DIR::DOWN, DIR::LEFT};

  while (!queue.empty()) {
    const auto curr = queue.front();
    queue.pop();
    const auto [x, y] = curr;
    const auto curr_cost = visited[y][x];
    if (x == end.x && y == end.y) {
      return visited;
    }

    for (const auto new_dir : dirs) {
      const auto new_coord = problem.get_coord(curr, new_dir);
      if (problem.check_move(new_coord)) {
        auto new_cost = curr_cost + 1;
        if (visited[new_coord.y][new_coord.x] != 0) {
          continue;
        }
        visited[new_coord.y][new_coord.x] = new_cost;
        queue.push(new_coord);
      }
    }
  }
  return visited;
}

auto solve_part2(const auto &obstacles) {
  int start = 1023;
  int end = obstacles.size() - 1;

  while (true) {
    if (start == end) {
      break;
    }
    int cur = std::midpoint(start, end);
    const auto problem = build_problem(obstacles, cur);
    const auto visited = solve_problem(problem);
    const auto part1 = visited[problem.end.y][problem.end.x] - 1;
    const bool success = part1 != -1;
    if (success) {
      start = cur + 1;
    } else {
      end = cur - 1;
    }
  }
  const auto [x, y] = obstacles[start - 1];
  fmt::println("Finished part 2 with byte {} with ({}, {})", start, x, y);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fmt::println("Provide input file and number of bytes");
    exit(1);
  }
  const auto obstacles = read_file(argv[1]);
  const auto problem = build_problem(obstacles, std::stoi(argv[2]));
  const auto visited = solve_problem(problem);
  const auto part1 = visited[problem.end.y][problem.end.x] - 1;
  fmt::println("part1 = {}", part1);
  solve_part2(obstacles);
}
