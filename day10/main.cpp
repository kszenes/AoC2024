#include "fmt/core.h"
#include "fmt/ranges.h"
#include <fstream>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

auto get_trailhead_loc_x(const auto &line_vec) {
  std::vector<int> locs;
  auto it = std::ranges::find(line_vec, 0);
  while (it != line_vec.end()) {
    locs.push_back(std::distance(line_vec.begin(), it));
    it = std::find(it + 1, line_vec.end(), 0);
  }
  return locs;
}

struct Grid {
  Grid(const std::vector<std::vector<int>> nums,
       const std::vector<std::pair<int, int>> heads)
      : nums(nums), heads(heads) {
    ysize = std::size(nums);
    xsize = std::size(nums.front());
  }
  int xsize;
  int ysize;
  std::vector<std::vector<int>> nums;
  std::vector<std::pair<int, int>> heads;
};

auto read_file(const std::string &fname) -> Grid {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<std::vector<int>> content;
  std::vector<std::pair<int, int>> trailheads;
  std::string line;

  auto y = 0;
  while (std::getline(f, line)) {
    auto line_vec =
        std::views::transform(line, [](const char c) { return c - '0'; }) |
        std::ranges::to<std::vector<int>>();
    content.push_back(line_vec);
    auto trailheads_x = get_trailhead_loc_x(line_vec);
    std::ranges::for_each(trailheads_x,
                          [&](const auto x) { trailheads.emplace_back(x, y); });
    ++y;
  }
  return {content, trailheads};
}

auto print_grid(const auto &grid) {
  for (const auto &y : grid.nums) {
    for (const auto &x : y) {
      fmt::print("{} ", x);
    }
    fmt::println("");
  }
  fmt::println("\nHeads\n{}", grid.heads);
}

auto make_step(const auto &grid, const auto &loc, const auto &val,
               auto &dest_set) -> void {
  int ntrails = 0;
  const auto [x, y] = loc;
  const auto xsize = grid.xsize;
  const auto ysize = grid.ysize;
  const auto &nums = grid.nums;
  if (val == 10) {
    dest_set.insert({x, y});
    // fmt::println("Finished at ({}, {})", y, x);
  }

  if (xsize > x + 1 && nums[y][x + 1] == val) {
    make_step(grid, std::make_pair(x + 1, y), val + 1, dest_set);
    // fmt::println("{}: ->({}, {})", val, x+1, y);
  }
  if (x > 0 && nums[y][x - 1] == val) {
    make_step(grid, std::make_pair(x - 1, y), val + 1, dest_set);
    // fmt::println("{}: ->({}, {})", val, x-1, y);
  }
  if (ysize > y + 1 && nums[y + 1][x] == val) {
    make_step(grid, std::make_pair(x, y + 1), val + 1, dest_set);
    // fmt::println("{}: ->({}, {})", val, x, y+1);
  }
  if (y > 0 && nums[y - 1][x] == val) {
    make_step(grid, std::make_pair(x, y - 1), val + 1, dest_set);
    // fmt::println("{}: ->({}, {})", val, x, y-1);
  }
}

auto solve_head(const auto &grid, const auto &start) {
  std::set<std::pair<int, int>> dest_set;
  make_step(grid, start, 1, dest_set);
  fmt::println("dest_set = {}", dest_set);
}

auto solve_grid(const auto &grid) {
  int ndests = 0;
  std::vector<std::set<std::pair<int, int>>> dests;
  for (const auto &start : grid.heads) {
    std::set<std::pair<int, int>> dest;
    make_step(grid, start, 1, dest);
    dests.push_back(dest);
  }

  auto res = 0;
  // fmt::println("ENDS");
  for (const auto &dest : dests) {
    // fmt::print("{} ", std::size(dest));
    res += std::size(dest);
  }
  return res;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto grid = read_file(argv[1]);
  // print_grid(grid);
  auto res1 = solve_grid(grid);
  fmt::println("res1 = {}", res1);
}
