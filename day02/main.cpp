#include "fmt/core.h"
#include "fmt/ranges.h"
#include <cmath>
#include <fstream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

auto is_monotonic(const auto &signs) {
  std::vector<bool> v;
  std::ranges::unique_copy(signs, std::back_inserter(v));
  return std::size(v) == 1;
}

auto check_line_(const auto &reports) -> bool {
  auto diffs = reports | std::views::adjacent_transform<2>(std::minus{});
  auto signs = diffs | std::views::transform(
                           [](const auto e) { return std::signbit(e); });
  auto monotonic = is_monotonic(signs);
  auto invalid_steps = diffs | std::views::filter([](const auto e) {
                         return std::abs(e) < 1 || std::abs(e) > 3;
                       }) |
                       std::ranges::to<std::vector>();
  auto is_valid_line = std::size(invalid_steps) == 0 && monotonic;
  // fmt::println("reports = {}\ndiffs = {}\nsigns = {}\nmonotonic = "
  //              "{}\ninvalid_steps = {}\nis_valid_line = {}\n",
  //              reports, diffs, signs, monotonic, invalid_steps, is_valid_line);
  return is_valid_line;
}

auto check_line(const auto &reports) -> int {
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

auto determine_ascending(const auto &reports) {
  if (reports.size() < 4) {
    fmt::println("Less than 4 elements");
    exit(1);
  }
  int num_ascending = 0;
  int _ = std::ranges::fold_left(
      reports.begin() + 1, reports.begin() + 4, reports.front(),
      [&](const auto prev, const auto curr) {
        num_ascending += static_cast<int>(prev < curr);
        return curr;
      });
  auto is_ascending = num_ascending >= 2;
  fmt::println("num_ascending = {}", num_ascending);
  return is_ascending;
}

auto check_line2(const auto &reports) -> bool {
  auto should_be_ascending = determine_ascending(reports);
  auto min_step_size = 1;
  auto max_step_size = 3;
  auto is_invalid_step = [&should_be_ascending](const auto &left,
                                                const auto &right) {
    auto diff = should_be_ascending ? right - left : left - right;
    return (diff < 1) || (diff > 3);
  };
  auto num_invalid_steps = 0;
  auto _ = std::ranges::fold_left(reports.begin() + 1, reports.end(),
                                  reports.front(),
                                  [&](const auto prev, const auto curr) {
                                    if (is_invalid_step(prev, curr)) {
                                      ++num_invalid_steps;
                                      return prev;
                                    } else {
                                      return curr;
                                    }
                                  });
  return (num_invalid_steps < 2);
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
    if (!is_dampned) {
      if (check_line_(std::views::istream<int>(ss) |
                      std::ranges::to<std::vector>())) {
        ++num_valid_lines;
      }
    } else {

    }
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
  // auto num_valid_lines_damped = read_file(argv[1], true);
  // fmt::println("Number of valid lines damped: {}", num_valid_lines_damped);
}
