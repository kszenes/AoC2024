#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <fstream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <vector>

auto read_file(const std::string &fname) {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }
  std::string line;
  std::getline(f, line);
  auto ss = std::istringstream(line);
  std::vector<int> ret;
  for (const auto c : std::views::istream<char>(ss)) {
    ret.push_back(static_cast<int>(c - '0'));
  }
  return ret;
}

auto parse(const auto &v) {
  auto pages = v | std::views::stride(2);
  auto spaces = v | std::views::drop(1) | std::views::stride(2) |
                std::ranges::to<std::vector<int>>();
  auto page_id = std::views::iota(0) | std::views::take(std::size(pages)) |
                 std::ranges::to<std::vector<int>>();
  std::vector<int> reps;
  for (const auto &[index, elem] :
       std::views::enumerate(std::views::zip(pages, spaces))) {
    auto page_rep =
        std::views::repeat(index) | std::views::take(std::get<0>(elem));
    auto space_rep =
        std::views::repeat(-1) | std::views::take(std::get<1>(elem));
    std::ranges::copy(page_rep, std::back_inserter(reps));
    std::ranges::copy(space_rep, std::back_inserter(reps));
  }
  // handle if size(pages) != size(spaces)
  if (std::size(pages) > std::size(spaces)) {
    std::ranges::copy(std::views::repeat(page_id.back()) |
                          std::views::take(pages.back()),
                      std::back_inserter(reps));
  } else {
    std::ranges::copy(std::views::repeat(-1) | std::views::take(spaces.back()),
                      std::back_inserter(reps));
  }
  // fmt::println("pages  =   {}\nspaces =   {}\npages_id = {}\nreps     = {}",
  //              pages, spaces, page_id, reps);
  return reps;
}

auto sort(auto v) {
  auto left = v.begin();
  auto right = v.end() - 1;

  while (left != right) {
    if (*left != -1) {
      ++left;
      continue;
    }
    if (*right == -1) {
      --right;
      continue;
    }
    std::iter_swap(left, right);
    --right;
  }
  return v;
}

auto count_val(const auto &v) {
  auto view = v | std::views::filter([](const auto e) { return e >= 0; });
  long long val = 0;
  for (const auto [index, elem] : std::views::enumerate(view)) {
    val += static_cast<long long>(index * elem);
  }
  // fmt::println("\nv    = {}\nview = {}\nval = {}", v, view, val);
  return val;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  auto in = read_file(argv[1]);
  const auto reps = parse(in);
  const auto sorted_reps = sort(reps);
  auto res1 = count_val(sorted_reps);
  fmt::println("res1 = {}", res1);
}
