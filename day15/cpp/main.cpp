#include "fmt/core.h"
#include "fmt/ranges.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct Coord {
  int x;
  int y;
};

struct Problem {
  const int xsize;
  const int ysize;
  std::vector<std::vector<char>> grid;
  const std::vector<char> moves;
  Coord loc;
};

auto read_file(const std::string &fname) {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<std::vector<char>> grid;
  std::string line;

  while (f.peek() != '\n') {
    auto row =
        std::views::istream<char>(f) | std::ranges::to<std::vector<char>>();
    grid.push_back(row);
    fmt::println("row = {}", row);
  }
  const int xsize = grid.front().size();
  const int ysize = grid.size();

  for (auto y : std::views::iota(0, ysize)) {
    for (auto x : std::views::iota(0, xsize)) {
      fmt::print("{}", grid[y][x]);
    }
    fmt::println("");
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  read_file(argv[1]);
}
