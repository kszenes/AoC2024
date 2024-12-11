#include "fmt/core.h"
#include "fmt/ranges.h"
#include <cmath>
#include <cstdlib>
#include <ranges>
#include <string>
#include <vector>

auto get_digits(const unsigned long long val) -> unsigned long long {
  return static_cast<unsigned long long>(std::log10l(val) + 1);
}

auto two_nums(const unsigned long long val, const auto ndigits) {
  return std::lldiv(val, std::pow(10, ndigits / 2));
}

auto dfs(const auto val, const auto max_level = 25, const auto level = 0) {
  if (level == max_level) {
    return 1;
  }
  const auto ndigits = get_digits(val);
  if (val == 0) {
    return dfs(1, max_level, level + 1);
  } else if (ndigits % 2 == 0) {
    const auto [quot, rem] = two_nums(val, ndigits);
    return dfs(quot, max_level, level + 1) + dfs(rem, max_level, level + 1);
  } else {
    return dfs(val * 2024, max_level, level + 1);
  }
}

auto memoize(const unsigned long long val, const auto max_level = 25) {
  std::vector<unsigned long long> children_level(max_level);
  std::vector<unsigned long long> curr{val};
  std::vector<unsigned long long> prev;
  for (int i = 0; i < max_level; ++i) {
    children_level[i] = curr.size();
    prev.swap(curr);
    curr.clear();
    for (const auto node : prev) {
      const auto ndigits = get_digits(node);
      if (node == 0) {
        curr.push_back(1);
      } else if (ndigits % 2 == 0) {
        const auto [quot, rem] = two_nums(node, ndigits);
        curr.push_back(quot);
        curr.push_back(rem);
      } else {
        curr.push_back(node * 2024);
      }
    }
  }
  return children_level;
}

int main(int argc, char *argv[]) {
  auto const zero_level = memoize(0, 26);
  fmt::println("zero: levels = {}\n", zero_level);
  const std::vector<unsigned long long> stones{890,   0,       1,    935698,
                                               68001, 3441397, 7221, 27};
  std::vector<unsigned long long> children(stones.size());
#pragma omp parallel for
  for (int i = 0; i < children.size(); ++i) {
    auto val = dfs(stones[i], 25, 0);
    children[i] = val;
  }
  auto sum = std::ranges::fold_left(children, 0ull, std::plus{});
  fmt::println("num_children = {}\nsum = {}", children, sum);
}
