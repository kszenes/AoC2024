#include "fmt/core.h"
#include "fmt/ranges.h"
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

struct Problem {
  int a, b, c;
  std::vector<int> program;
  int ptr = 0;
};

auto read_file(const std::string &fname) -> Problem {
  std::ifstream f(fname);
  if (!f.is_open()) {
    fmt::println("File '{}' could not be opened", fname);
    exit(1);
  }
  int a, b, c;
  std::string line;

  {
    std::getline(f, line);
    auto str = std::views::split(line, ' ') |
               std::views::transform(
                   [](const auto &e) { return std::string_view(e); }) |
               std::ranges::to<std::vector<std::string>>();
    a = std::stoi(str.back());
  }
  {
    std::getline(f, line);
    auto str = std::views::split(line, ' ') |
               std::views::transform(
                   [](const auto &e) { return std::string_view(e); }) |
               std::ranges::to<std::vector<std::string>>();
    b = std::stoi(str.back());
  }
  {
    std::getline(f, line);
    auto str = std::views::split(line, ' ') |
               std::views::transform(
                   [](const auto &e) { return std::string_view(e); }) |
               std::ranges::to<std::vector<std::string>>();
    c = std::stoi(str.back());
  }
  std::getline(f, line);
  std::getline(f, line);
  auto str = std::views::split(line, ' ') |
             std::views::transform([](const auto &e) {
               return std::string(std::string_view(e));
             }) |
             std::views::drop(1) | std::ranges::to<std::vector<std::string>>();
  auto program = std::views::split(str.front(), ',') |
                 std::views::transform([](const auto &e) {
                   return std::stoi(std::string(std::string_view(e)));
                 }) |
                 std::ranges::to<std::vector<int>>();
  return {a, b, c, program};
}

auto print_problem(const auto &problem) {
  const auto &[a, b, c, program, ptr] = problem;
  fmt::println("A: {}\nB: {}\nC: {}\nprogram: {}\nptr = {}", a, b, c, program,
               ptr);
}

enum class Op { adv, bxl, bst, jnz, bxc, out, bdv, cdv };

auto literal2combo(const Problem &problem, const int literal) {
  const auto &[a, b, c, program, ptr] = problem;
  switch (literal) {
  case 4:
    return a;
  case 5:
    return b;
  case 6:
    return c;
  default:
    return literal;
  }
}

auto do_op(Problem &problem, const Op &op, const int literal) {
  auto &[a, b, c, program, ptr] = problem;
  auto combo = literal2combo(problem, literal);
  switch (op) {
  case Op::adv:
    // fmt::println("adv({})", operand);
    a = (a >> combo); 
    ptr += 2;
    break;
  case Op::bdv:
    // fmt::println("bdv({})", operand);
    b = (a >> combo);
    ptr += 2;
    break;
  case Op::cdv:
    // fmt::println("cdv({})", operand);
    c = (a >> combo);
    ptr += 2;
    break;
  case Op::bxl:
    // fmt::println("bxl({})", operand);
    b = b ^ literal;
    ptr += 2;
    break;
  case Op::bst:
    // fmt::println("bst({})", operand);
    b = combo & 7;
    ptr += 2;
    break;
  case Op::jnz:
    // fmt::println("jnz({})", operand);
    ptr = (a != 0) ? literal : ptr + 2;
    break;
  case Op::bxc:
    // fmt::println("bxc({})", operand);
    b = b ^ c;
    ptr += 2;
    break;
  case Op::out:
    fmt::print("{},", combo & 7);
    ptr += 2;
    break;
  }
}

void solve_problem(Problem &p) {
  while (p.ptr < p.program.size() - 1) {
    const Op op = static_cast<Op>(p.program.at(p.ptr));
    const int arg = p.program.at(p.ptr + 1);
    do_op(p, op, arg);
    // print_problem(p);
  }
  fmt::println("");
}

auto test() {
  Problem p{0, 2024, 43690, {4, 0}};
  solve_problem(p);
  print_problem(p);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::println("Provide input file");
    exit(1);
  }
  auto problem = read_file(argv[1]);
  print_problem(problem);
  solve_problem(problem);
  print_problem(problem);
  // test();
}
