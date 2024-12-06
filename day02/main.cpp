#include "fmt/core.h"
#include "fmt/ranges.h"
#include <fstream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

auto check_line(const auto &reports) {
  auto should_be_ascending = reports[0] < reports[1];
  auto min_step_size = 1;
  auto max_step_size = 3;
  auto is_invalid_step = [&should_be_ascending](const auto &left,
                                                const auto &right) {
    auto diff = should_be_ascending ? right - left : left - right;
    return (diff < 1) || (diff > 3);
  };
  auto num_invalid_steps = 0;
  for (const auto &[left, right] : reports | std::views::adjacent<2>) {
    if (is_invalid_step(left, right)) {
      ++num_invalid_steps;
    }
  }
  return num_invalid_steps;
}

auto read_file(const std::string &fname, const bool is_dampned = false) {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::string line;

  auto num_valid_lines = 0;
  while (std::getline(f, line)) {
    auto ss = std::istringstream(line);
    auto num_invalid_steps = check_line(std::views::istream<int>(ss) |
                                        std::ranges::to<std::vector<int>>());
    if ((!is_dampned && num_invalid_steps == 0) ||
        (is_dampned && num_invalid_steps <= 1))
      num_valid_lines += 1;
  }
  return num_valid_lines;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  auto num_valid_lines = read_file(argv[1]);
  fmt::println("Number of valid lines: {}", num_valid_lines);
  auto num_valid_lines_damped = read_file(argv[1], true);
  fmt::println("Number of valid lines damped: {}", num_valid_lines_damped);
}
