#include "fmt/core.h"
#include "fmt/ranges.h"
#include <fstream>
#include <mdspan>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

auto read_file(const std::string &fname)
    -> std::tuple<std::vector<char>, int, int> {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<char> content;
  std::string line;

  auto num_lines = 0;
  auto line_length = 0;

  while (std::getline(f, line)) {
    num_lines++;
    line_length = line.size();
    auto ss = std::istringstream(line);
    std::ranges::copy(std::views::istream<char>(ss),
                      std::back_inserter(content));
  }
  return {content, line_length, num_lines};
}

auto check_letter1(const auto &grid, const auto row, const auto col,
                   const auto &word) -> int {
  auto remaining_letters = 3;
  auto num_matches = 0;

  // Vertical
  if (row >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row - i, col] == word[i]);
    }
    num_matches += matches;
  }
  if (grid.extent(1) - 1 - row >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row + i, col] == word[i]);
    }
    num_matches += matches;
  }

  // Horizontal
  if (col >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row, col - i] == word[i]);
    }
    num_matches += matches;
  }
  if (grid.extent(0) - 1 - col >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row, col + i] == word[i]);
    }
    num_matches += matches;
  }

  // Diagonal
  if (row >= remaining_letters && col >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row - i, col - i] == word[i]);
    }
    num_matches += matches;
  }
  if (row >= remaining_letters &&
      grid.extent(0) - 1 - col >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row - i, col + i] == word[i]);
    }
    num_matches += matches;
  }
  if (grid.extent(1) - 1 - row >= remaining_letters &&
      col >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row + i, col - i] == word[i]);
    }
    num_matches += matches;
  }
  if (grid.extent(1) - 1 - row >= remaining_letters &&
      grid.extent(0) - 1 - col >= remaining_letters) {
    auto matches = 1;
    for (int i = 1; i <= remaining_letters; ++i) {
      matches *= static_cast<int>(grid[row + i, col + i] == word[i]);
    }
    num_matches += matches;
  }
  return num_matches;
}

auto solve_grid1(const auto &grid) -> int {
  std::string word("XMAS");
  auto num_matches = 0;
  for (int col = 0; col < grid.extent(0); ++col) {
    for (int row = 0; row < grid.extent(1); ++row) {
      auto letter = grid[row, col];
      if (letter == 'X') {
        auto matched = check_letter1(grid, row, col, word);
        num_matches += matched;
      }
    }
  }
  return num_matches;
}

auto check_letter2(auto grid, auto row, auto col) -> bool {
  if (row == 0 || row == grid.extent(1) - 1 || col == 0 ||
      col == grid.extent(0)) {
    return false;
  }

  auto check_outer = [&]() {
    return (grid[row + 1, col + 1] == 'M' && grid[row - 1, col - 1] == 'S') ||
           (grid[row + 1, col + 1] == 'S' && grid[row - 1, col - 1] == 'M');
  }();

  auto check_inner = [&]() {
    return (grid[row - 1, col + 1] == 'M' && grid[row + 1, col - 1] == 'S') ||
           (grid[row - 1, col + 1] == 'S' && grid[row + 1, col - 1] == 'M');
  }();

  return check_outer && check_inner;
}

auto solve_grid2(const auto &grid) -> int {
  auto num_matches = 0;
  for (int col = 0; col < grid.extent(0); ++col) {
    for (int row = 0; row < grid.extent(1); ++row) {
      auto letter = grid[row, col];
      if (letter == 'A') {
        auto matched = static_cast<int>(check_letter2(grid, row, col));
        num_matches += matched;
      }
    }
  }
  return num_matches;
}

auto print_grid(const auto &grid) {
  for (int col = 0; col < grid.extent(0); ++col) {
    for (int row = 0; row < grid.extent(1); ++row) {
      fmt::print("{} ", grid[col, row]);
    }
    fmt::println("");
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto [content, cols, rows] = read_file(argv[1]);
  auto grid = std::mdspan(content.data(), rows, cols);
  // print_grid(grid);
  auto num_matches = solve_grid1(grid);
  fmt::println("#XMAS = {}", num_matches);
  auto num_matches2 = solve_grid2(grid);
  fmt::println("#X-MAS = {}", num_matches2);
}
