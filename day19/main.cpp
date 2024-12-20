#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <fstream>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

struct Problem {
  const std::vector<std::string> chunks;
  const std::vector<std::string> designs;
};

auto read_file(const std::string &fname) -> Problem {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }
  std::string line;

  // chunks
  std::getline(f, line);
  // fmt::println("{}", line);
  const auto [start, end] = std::ranges::remove(line, ',');
  line.erase(start, end);
  const auto chunks = std::views::split(line, ' ') |
                      std::ranges::to<std::vector<std::string>>();
  // fmt::println("chunks = {}", chunks);

  std::getline(f, line); // skip line

  // designs
  std::vector<std::string> designs;
  while (std::getline(f, line)) {
    designs.emplace_back(line);
  }
  // fmt::println("designs = {}", designs);

  return {chunks, designs};
}

auto check_chunk(const std::string_view str, const std::string_view chunk)
    -> std::pair<bool, std::string_view> {
  if (str.size() < chunk.size()) {
    return {false, {}};
  }
  if (str.starts_with(chunk)) {
    return {true, str.substr(chunk.size())};
  } else {
    return {false, {}};
  }
}

auto check_design(std::string_view design,
                  const std::vector<std::string> &chunks) -> bool {
  std::queue<std::string_view> queue;
  queue.push(design);
  while (!queue.empty()) {
    const auto cur = queue.front();
    queue.pop();
    for (const auto &chunk : chunks) {
      const auto [success, substr] = check_chunk(cur, chunk);
      if (success) {
        if (substr.empty()) {
          return true;
        }
        queue.push(substr);
      }
    }
  }
  return false;
}

auto check_design_dynamic(std::string_view design,
                          const std::vector<std::string> &chunks) -> bool {
  std::vector<int> flags(design.size() + 1);
  flags.at(0) = 1;

  for (size_t i = 1; i <= design.size(); ++i) {
    for (const auto &chunk : chunks) {
      const auto chunk_size = chunk.size();
      if (chunk_size <= i &&
          design.substr(i - chunk_size, chunk_size) == chunk) {
        flags[i] = flags[i - chunk_size];
      }
    }
  }
  return flags.back();
}

auto check_designs(const auto &designs, const auto &chunks) {
  int nsuccess = 0;
  for (const auto &design : designs) {
    auto success = check_design_dynamic(design, chunks);
    fmt::println("{}: {}", design, success);
    nsuccess += static_cast<int>(success);
  }
  fmt::println("nsucc = {}", nsuccess);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto [chunks, designs] = read_file(argv[1]);
  check_designs(designs, chunks);
}
