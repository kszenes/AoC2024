#include "ctre.hpp"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include <fstream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals::string_view_literals;

auto read_file(const std::string &fname) -> std::vector<std::string> {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(f, line)) {
    lines.push_back(line);
  }
  return lines;
}

auto process_lines1(const auto &lines) -> int {
  auto val = 0;
  for (const auto &line : lines) {
    auto result = ctre::range<"mul\\(([0-9]+),([0-9]+)\\)">(line);
    for (const auto &[match, v1, v2] : result) {
      val += stoi(v1.str()) * stoi(v2.str());
    }
  }
  return val;
}

auto eval_chunk(const auto &chunk) -> int {
  auto val = 0;
  auto mul_regex = ctre::range<"mul\\(([0-9]+),([0-9]+)\\)">;
  // this check is needed for some reason
  for (const auto &[match, v1, v2] : mul_regex(chunk)) {
    val += stoi(v1.str()) * stoi(v2.str());
  }
  return val;
}

// Doesn't work for some reason even though checked chunks by hand
auto process_lines2(const auto &lines) -> int {
  auto val = 0;
  int i = 0;
  for (const auto &line : lines) {
    // fmt::println("\nLine {}", i++);
    auto chunks = ctre::split<"don't\\(\\)">(line);
    // Treat first element separately
    auto first_chunk = (*std::begin(chunks)).view();
    // fmt::println("{}\n", first_chunk);
    val += eval_chunk(first_chunk);
    for (const auto &chunk : chunks | std::views::drop(1)) {
      auto res = ctre::search<"do\\(\\).*">(chunk);
      if (res) {
        // fmt::println("{}\n", res.view());
        val += eval_chunk(res);
      }
    }
  }
  return val;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }

  const auto lines = read_file(argv[1]);
  auto res1 = process_lines1(lines);
  fmt::println("Result 1 = {}", res1);
  auto res2 = process_lines2(lines);
  fmt::println("Result 2 = {}", res2);
}
