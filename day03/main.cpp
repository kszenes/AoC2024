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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto [content, cols, rows] = read_file(argv[1]);
  auto grid = std::mdspan(content.data(), rows, cols);
  for (int col = 0; col < grid.extent(0); ++col) {
    for (int row = 0; row < grid.extent(1); ++row) {
      fmt::print("{} ", grid[row, col]);
    }
    fmt::println("");
  }
}
