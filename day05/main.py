from collections import defaultdict
from functools import cmp_to_key

map = defaultdict(int)
smaller = set()
larger = set()
def sort(n1, n2):
    return map[f"{n1},{n2}"]

total_correct = 0
total_wrong = 0
with open("./inputs/short.txt") as f:
    rules = True
    for line in f:
        if rules:
            map[line.strip().replace("|", ",")] = -1
            if line != "\n":
                small, large = line.strip().split('|')
                smaller.add(small)
                larger.add(large)
        if line == "\n":
            rules = False
        elif not rules:
            inp = line.strip().split(",")
            idx = (len(inp) - 1)//2
            if (sorted_input := sorted(inp, key=cmp_to_key(sort))) == inp:
                total_correct += int(inp[idx])
            else:
                total_wrong += int(sorted_input[idx])

print(f"Sum part A {total_correct}")
print(f"Sum part B {total_wrong}")
print(f"({len(smaller)}) {smaller = }")
print(f"({len(larger)}) {larger = }")
print(f"({len(set.intersection(smaller, larger))})")

