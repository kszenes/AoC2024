#include "fmt/core.h"
#include "fmt/ranges.h"
#include <cmath>
#include <cstdlib>
#include <ranges>
#include <string>
#include <vector>

auto get_digits(const long long val) {
  return static_cast<long long>(std::log10l(val) + 1);
}

auto two_nums(const long long val) {
  const auto ndigits = get_digits(val);
  return std::lldiv(val, std::pow(10, ndigits / 2));
}

auto dfs(const auto val, const auto level = 0) {
  if (level == 45) {
    return 1;
  }
  const auto ndigits = get_digits(val);
  if (val == 0) {
    return dfs(1, level + 1);
  } else if (ndigits % 2 == 0) {
    const auto [quot, rem] = two_nums(val);
    return dfs(quot, level + 1) + dfs(rem, level + 1);
  } else {
    return dfs(val * 2024, level + 1);
  }
}

int main(int argc, char *argv[]) {
  const std::vector<long long> stones{890, 0, 1, 935698, 68001, 3441397, 7221, 27};
  std::vector<long long> children;
  for (const auto stone : stones) {
    children.push_back(dfs(stone, 0));
  }
  auto sum = std::ranges::fold_left(children, 0ll, std::plus{});
  fmt::println("children = {}\nsum = {}", children, sum);
}
