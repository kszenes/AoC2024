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

using val_t = int;

struct Coord {
  val_t x;
  val_t y;

  bool operator==(const Coord &other) const {
    return x == other.x && y == other.y;
  }
};

struct Robot {
  Coord pos;
  Coord vel;
};

struct Problem {
  std::vector<Robot> robots;
  const val_t xsize = 101;
  const val_t ysize = 103;
};

auto parse_line(std::ifstream &stream) -> Robot {
  Coord pos, vel;
  char ch;
  while (stream.get(ch) && ch != '=') {
  }
  stream >> pos.x;
  stream >> ch;
  stream >> pos.y;
  while (stream.get(ch) && ch != '=') {
  }
  stream >> vel.x;
  stream >> ch;
  stream >> vel.y;
  return {pos, vel};
}

auto print_problem(const Problem &problem) {
  const auto &[robots, xsize, ysize] = problem;
  // fmt::println("Size = ({}, {})", xsize, ysize);
  // for (const auto &[i, robot] : std::views::enumerate(robots)) {
  //   const auto &[pos, vel] = robot;
  //   fmt::println("{}: pos = ({}, {}); vel = ({}, {})", i, pos.x, pos.y,
  //   vel.x,
  //                vel.y);
  // }
  // fmt::println("");
  for (int y = 0; y < ysize; ++y) {
    for (int x = 0; x < xsize; ++x) {
      const Coord loc = {x, y};
      int nrob = std::ranges::count(robots, loc, &Robot::pos);
      if (nrob) {
        // fmt::print("{}", nrob);
        fmt::print("*");
      } else {
        fmt::print(" ");
      }
    }
    fmt::println("");
  }
  fmt::println("");
  fflush(stdout);
}

auto read_file(const std::string &fname) -> Problem {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<Robot> robots;
  while (!f.eof()) {
    robots.emplace_back(parse_line(f));
  }
  robots.pop_back();
  return {robots};
}

auto solve_problem(auto &problem, const int nsec) {
  auto &[robots, xsize, ysize] = problem;
  for (auto &robot : robots) {
    auto &[pos, vel] = robot;
    pos.x = std::remainder(pos.x + nsec * vel.x, xsize);
    pos.y = std::remainder(pos.y + nsec * vel.y, ysize);
    if (pos.x < 0) {
      pos.x = xsize + pos.x;
    }
    if (pos.y < 0) {
      pos.y = ysize + pos.y;
    }
  }
}

auto count_quadrants(const auto &problem) {
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
  const auto &[robots, xsize, ysize] = problem;
  for (int y = 0; y < ysize / 2; ++y) {
    for (int x = 0; x < xsize / 2; ++x) {
      const Coord loc{x, y};
      int nrob = std::ranges::count(robots, loc, &Robot::pos);
      q1 += nrob;
      // if (nrob) {
      //   fmt::print("{}", nrob);
      // } else {
      //   fmt::print(".");
      // }
    }
    // fmt::println("");
  }
  // fmt::println("");
  for (int y = 0; y < ysize / 2; ++y) {
    for (int x = xsize / 2 + 1; x < xsize; ++x) {
      const Coord loc{x, y};
      int nrob = std::ranges::count(robots, loc, &Robot::pos);
      q2 += nrob;
      // if (nrob) {
      //   fmt::print("{}", nrob);
      // } else {
      //   fmt::print(".");
      // }
    }
    // fmt::println("");
  }
  // fmt::println("");
  for (int y = ysize / 2 + 1; y < ysize; ++y) {
    for (int x = 0; x < xsize / 2; ++x) {
      const Coord loc{x, y};
      int nrob = std::ranges::count(robots, loc, &Robot::pos);
      q3 += nrob;
      // if (nrob) {
      //   fmt::print("{}", nrob);
      // } else {
      //   fmt::print(".");
      // }
    }
    // fmt::println("");
  }
  // fmt::println("");
  for (int y = ysize / 2 + 1; y < ysize; ++y) {
    for (int x = xsize / 2 + 1; x < xsize; ++x) {
      const Coord loc{x, y};
      int nrob = std::ranges::count(robots, loc, &Robot::pos);
      q4 += nrob;
      // if (nrob) {
      //   fmt::print("{}", nrob);
      // } else {
      //   fmt::print(".");
      // }
    }
    // fmt::println("");
  }
  // fmt::println("");
  fmt::println("{}, {}, {}, {}", q1, q2, q3, q4);
  const auto safety_factor = q1 * q2 * q3 * q4;
  fmt::println("Safety factor: {}", safety_factor);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  auto problem = read_file(argv[1]);
  print_problem(problem);
  int init = 500;
  char c;
  solve_problem(problem, init);

  int i = init;
  while (true) {
    fmt::println("================================================= Second {} "
                 "=====================================================n",
                 i + 1);
    fmt::println("============================================================="
                 "====================================================n");
    solve_problem(problem, 1);
    print_problem(problem);
    std::cin.get();
    ++i;
  }
}
