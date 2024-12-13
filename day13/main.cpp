#include "fmt/core.h"
#include "fmt/ranges.h"
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using val_t = long long;

struct Coord {
  val_t x;
  val_t y;
};

struct Problem {
  Coord a;
  Coord b;
  Coord prize;
};

auto parse_line(std::ifstream &stream, const char sep) -> Coord {
  val_t x = 0;
  val_t y = 0;
  char ch;
  while (stream.get(ch) && ch != sep) {
  }
  stream >> x;
  while (stream.get(ch) && ch != sep) {
  }
  stream >> y;
  if (sep == '=') {
    x += static_cast<long long>(std::pow(10, 13));
    y += static_cast<long long>(std::pow(10, 13));
  }
  return {x, y};
}

auto parse_problem(std::ifstream &file) -> Problem {
  auto a = parse_line(file, '+');
  auto b = parse_line(file, '+');
  auto prize = parse_line(file, '=');
  std::string line;
  std::getline(file, line);
  return {a, b, prize};
}

auto print_problem(const auto &problem) {
  fmt::println("A = ({}, {})", problem.a.x, problem.a.y);
  fmt::println("B = ({}, {})", problem.b.x, problem.b.y);
  fmt::println("prize = ({}, {})", problem.prize.x, problem.prize.y);
}

auto read_file(const std::string &fname) {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }

  std::vector<Problem> problems;
  while (true) {
    auto problem = parse_problem(f);
    problems.push_back(problem);
    if (f.eof()) {
      break;
    }
  }
  problems.pop_back();
  // for (const auto &p : problems) {
  //   print_problem(p);
  // }
  return problems;
}

auto is_lin_dep(const auto &a, const auto &b) {
  auto determinant = a.x * b.y - a.y * b.x;
  return determinant == 0;
}

auto solve_problem(const auto &problem) {
  const auto &[a, b, prize] = problem;
  if (is_lin_dep(a, b)) {
    fmt::println("Linear dependent");
    exit(1);
  }

  const val_t beta1 = prize.y * a.x - prize.x * a.y;
  const val_t beta2 = b.y * a.x - b.x * a.y;
  const auto [beta, rem1] = std::lldiv(beta1, beta2);

  const val_t alpha1 = prize.x - beta * b.x;
  const auto [alpha, rem2] = std::lldiv(alpha1, a.x);

  if (alpha < 0 || beta < 0 || rem1 != 0 || rem2 != 0) {
    fmt::println("NO SOLUTION POSSIBLE");
    return 0ll;
  } else {
    const auto sum = 3 * alpha + beta;
    fmt::println("{} + {} = {}", alpha, beta, sum);
    return sum;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  const auto problems = read_file(argv[1]);
  auto sum = 0ll;
  for (const auto &p : problems) {
    sum += solve_problem(p);
  }
  fmt::println("sum = {}", sum);
}
