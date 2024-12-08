#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <fstream>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

auto parse_order(std::string_view str) -> std::pair<int, int> {
  auto values = std::views::split(str, '|') |
                std::ranges::to<std::vector<std::string>>() |
                std::views::transform([](const auto e) { return stoi(e); });
  return {values.front(), values.back()};
}

auto parse_page(std::string_view str) -> std::vector<int> {
  auto values = std::views::split(str, ',') |
                std::ranges::to<std::vector<std::string>>() |
                std::views::transform([](const auto e) { return stoi(e); }) |
                std::ranges::to<std::vector>();
  return values;
}

struct Problem {
  const std::vector<int> smaller, larger;
  const std::vector<std::vector<int>> pages;
};

auto read_file(const std::string &fname) -> Problem {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<int> smaller, larger;
  std::vector<std::vector<int>> pages;
  std::string line;

  auto parsing_order = true;
  while (std::getline(f, line)) {
    if (line.empty()) {
      parsing_order = false;
      continue;
    }
    if (parsing_order) {
      const auto [small, large] = parse_order(line);
      smaller.push_back(small);
      larger.push_back(large);
    } else {
      const auto page = parse_page(line);
      pages.push_back(page);
    }
  }
  return {smaller, larger, pages};
}

struct DAG {
  const std::vector<int> nodes;
  const std::vector<std::pair<int, int>> edges;
  const std::vector<int> in_degrees;
};

auto build_dag(const auto &smaller, const auto &larger) -> DAG {
  std::vector<int> nodes(std::vector{smaller, larger} | std::views::join |
                         std::ranges::to<std::vector>());
  std::ranges::sort(nodes);
  const auto ret = std::ranges::unique(nodes);
  nodes.erase(ret.begin(), ret.end());

  std::vector<std::pair<int, int>> edges;
  std::ranges::copy(std::views::zip(smaller, larger),
                    std::back_inserter(edges));

  std::vector<int> in_degrees(nodes.size());
  for (const auto [i, node] : std::views::enumerate(nodes)) {
    in_degrees[i] = std::ranges::count(larger, node);
  }
  // fmt::println("nodes = {}\nedges = {}\nin_degrees = {}", nodes, edges,
  //              in_degrees);
  return {nodes, edges, in_degrees};
}

// Uses Kahn's Algorithm (BFS)
auto toplogical_sort(const auto &dag) {
  const auto &[nodes, edges, in_degrees] = dag;
  auto degrees(in_degrees);
  // Populate queue with no incoming nodes (root)
  std::queue<int> q;
  std::vector<int> top_sorted;
  for (const auto [i, node] : std::views::enumerate(nodes)) {
    if (degrees[i] == 0) {
      q.push(node);
    }
  }
  while (!q.empty()) {
    auto node = q.front();
    q.pop();
    top_sorted.push_back(node);
    for (const auto [in, out] : edges) {
      if (in == node) {
        auto out_idx =
            std::distance(nodes.begin(), std::ranges::find(nodes, out));
        --degrees[out_idx];
        if (degrees[out_idx] == 0) {
          q.push(out);
        }
      }
    }
  }
  // fmt::println("top = {}", top_sorted);
  return top_sorted;
}

auto check_page(const auto &page, const auto &order) -> std::pair<bool, int> {
  auto l = std::views::filter(
      page, [&](const auto &e) { return std::ranges::contains(order, e); });
  auto r = std::views::filter(
      order, [&](const auto &e) { return std::ranges::contains(page, e); });
  auto same = std::ranges::equal(l, r);

  auto mid_idx = static_cast<int>((page.size() - 1) / 2);

  return {same, page[mid_idx]};
}

auto part1(const auto &pages, const auto& order) {
  auto ret = 0;
  for (const auto &page : pages) {
    const auto [valid, mid] = check_page(page, order);
    if (valid) {
      ret += mid;
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto [smaller, larger, pages] = read_file(argv[1]);
  auto dag = build_dag(smaller, larger);
  auto order = toplogical_sort(dag);
  auto ret1 = part1(pages, order);
  fmt::println("ret1 = {}", ret1);
}
