#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <bitset>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

struct Problem {
  std::vector<long long> res;
  std::vector<std::vector<long long>> args;
};

auto read_file(const std::string &fname) -> Problem {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::string line;
  std::vector<long long> res;
  std::vector<std::vector<long long>> operands;

  while (std::getline(f, line)) {
    auto chunks = std::views::split(line, ' ') |
                  std::ranges::to<std::vector<std::string>>();
    operands.push_back(
        chunks | std::views::drop(1) |
        std::views::transform([](const auto &e) { return std::stoll(e); }) |
        std::ranges::to<std::vector<long long>>());
    auto res_str = chunks.front();
    res.push_back(std::stoll(res_str.substr(0, std::size(res_str) - 1)));
  }
  return {res, operands};
}

auto solve_line(const auto res, const auto &args) {
  auto nops = args.size() - 1;
  auto ops_secs = std::views::iota(0, 1 << nops) |
                  std::views::transform([nops](const auto e) {
                    std::bitset<64> bits(e);
                    std::vector<int> binary_vec(nops);
                    for (int i = 0; i < nops; ++i) {
                      binary_vec[i] = bits[i];
                    }
                    return binary_vec;
                  });
  fmt::println("ops_secs = {}", ops_secs);
  for (const auto &op_sec : ops_secs) {
    long val = args.front();
    for (const auto [index, op] : std::views::enumerate(op_sec)) {
      int curr = args[index + 1];
      val = op ? val * curr : val + curr;
      if (val > res) {
        continue;
      }
    }
    if (val == res) {
      return true;
    }
  }
  return false;
}

auto solve_problem(const auto &problem) {
  long long ret = 0;
  const auto &[res, args] = problem;
  for (const auto &[res, args] : std::views::zip(res, args)) {
    auto is_correct = solve_line(res, args);
    if (is_correct) {
      ret += res;
    }
  }
  return ret;
}

// taken from:
// https://stackoverflow.com/questions/75815328/viewscartesian-product-how-to-do-same-vector-n-times
template <size_t N> auto product(auto &&v) {
  return [&v]<size_t... Is>(std::index_sequence<Is...>) {
    return std::views::cartesian_product((Is, v)...);
  }(std::make_index_sequence<N>{});
}

auto solve_line2(const auto res, const auto &args) {
  auto nops = args.size() - 1;
  auto op_types = std::views::iota(0) | std::views::take(2);

  for (auto i : op_types) {
    for (auto j : op_types) {
      for (auto k : op_types) {
        std::vector<int> op_sec(nops);
        for (auto& op : op_sec) {

        }
      }
    }
  }

  for (int op_loc = 0; op_loc < nops; ++op_loc) {

  }
    long long val = args.front();
    for (const auto [index, op] : std::views::enumerate(op_sec)) {
      int curr = args[index + 1];
      if (op == 0) {
        val *= curr;
      } else if (op == 1) {
        val += curr;
      } else {
        val = stoll(std::to_string(val) + std::to_string(curr));
      }
      if (val > res) {
        continue;
      }
    }
    if (val == res) {
      return true;
    }
  return false;
}

auto solve_problem2(const auto &problem) {
  long long ret = 0;
  const auto &[res, args] = problem;
  for (const auto &[res, args] : std::views::zip(res, args)) {
    auto is_correct = solve_line2(res, args);
    if (is_correct) {
      ret += res;
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  auto problem = read_file(argv[1]);
  auto res1 = solve_problem(problem);
  fmt::println("res1 = {}", res1);
  solve_line2(3267, std::vector{81, 40, 27});
  auto res2 = solve_problem2(problem);
  fmt::println("res2 = {}", res2);
}
