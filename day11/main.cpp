#include "fmt/core.h"
#include "fmt/ranges.h"
#include <cmath>
#include <cstdlib>
#include <ranges>
#include <unordered_map>
#include <utility>
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

auto get_elems_and_freqs(const auto &curr_vals, const auto &curr_freqs)
    -> std::pair<std::vector<unsigned long long>,
                 std::vector<unsigned long long>> {
  std::unordered_map<unsigned long long, unsigned long long> val2freq;
  for (const auto [val, freq] : std::views::zip(curr_vals, curr_freqs)) {
    val2freq[val] += freq;
  }
  std::vector<unsigned long long> vals;
  std::vector<unsigned long long> freqs;

  for (const auto &e : val2freq) {
    vals.push_back(e.first);
    freqs.push_back(e.second);
  }

  return {vals, freqs};
}

auto bfs(const unsigned long long val, const auto max_level = 25) {
  std::vector<unsigned long long> prev_vals{val};
  std::vector<unsigned long long> prev_freqs{1};
  for (int i = 0; i < max_level; ++i) {
    const auto [curr_vals, curr_freqs] =
        get_elems_and_freqs(prev_vals, prev_freqs);
    prev_vals.clear();
    prev_freqs.clear();
    for (const auto [val, freq] : std::views::zip(curr_vals, curr_freqs)) {
      const auto ndigits = get_digits(val);
      if (val == 0) {
        prev_vals.push_back(1);
        prev_freqs.push_back(freq);
      } else if (ndigits % 2 == 0) {
        const auto [quot, rem] = two_nums(val, ndigits);
        prev_vals.push_back(quot);
        prev_vals.push_back(rem);
        prev_freqs.push_back(freq);
        prev_freqs.push_back(freq);
      } else {
        prev_vals.push_back(val * 2024);
        prev_freqs.push_back(freq);
      }
    }
  }
  auto nleaves = std::ranges::fold_left(prev_freqs, 0, std::plus{});
  return nleaves;
}

int main(int argc, char *argv[]) {
  // auto const zero_level = memoize(0, 26);
  // fmt::println("zero: levels = {}\n", zero_level);
  const std::vector<unsigned long long> stones{890,   0,       1,    935698,
                                               68001, 3441397, 7221, 27};
  std::vector<unsigned long long> children(stones.size());
#pragma omp parallel for
  for (int i = 0; i < children.size(); ++i) {
    // auto val = dfs(stones[i], 25, 0);
    auto val = bfs(stones[i], 75);
    children[i] = val;
  }
  auto sum = std::ranges::fold_left(children, 0ull, std::plus{});
  fmt::println("num_children = {}\nsum = {}", children, sum);
}
