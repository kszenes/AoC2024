#include <algorithm>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

auto read_locs(const std::string &fname)
    -> std::pair<std::vector<int>, std::vector<int>> {
  std::vector<int> left, right;

  std::ifstream f(fname);
  if (!f.is_open()) {
    std::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::string chunk1, chunk2;

  while (f >> chunk1 >> chunk2) {
    left.push_back(std::stoi(chunk1));
    right.push_back(std::stoi(chunk2));
  }

  return {left, right};
}

auto get_distance(auto &left, auto &right) {
  std::ranges::sort(left);
  std::ranges::sort(right);

  auto val = std::transform_reduce(
      left.cbegin(), left.cend(), right.cbegin(), 0, std::plus{},
      [](auto l, auto r) { return std::abs(l - r); });
  return val;
}

auto get_sim_score(const auto &left, const auto &right) {
  auto val = 0;
  for (const auto &chunk :
       left | std::views::chunk_by(std::ranges::equal_to{})) {
    auto element = chunk[0];
    auto chunk_size = std::size(chunk);
    auto right_count = std::size(std::ranges::equal_range(right, element));
    val += chunk_size * element * right_count;
  }
  return val;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::println("Provide input file");
    exit(1);
  }
  auto [left, right] = read_locs(argv[1]);
  auto dist = get_distance(left, right);
  std::println("dist = {}", dist);
  auto sim_score = get_sim_score(left, right);
  std::println("sim_score = {}", sim_score);
}
