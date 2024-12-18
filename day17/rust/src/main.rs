use std::env::args;
use std::fs::File;
use std::io::{BufRead, BufReader};

struct Problem {
    a: i32,
    b: i32,
    c: i32,
    program: Vec<i32>,
}

fn read_file(fname: &str) -> Problem {
    let file = File::open(fname).expect(&format!("'{fname}' could not be opened"));
    let reader = BufReader::new(file);
    let lines: Vec<String> = reader.lines().map(|l| l.unwrap()).collect();
    let registers = [
        lines[0][11..].trim().parse::<i32>().unwrap(),
        lines[1][11..].trim().parse::<i32>().unwrap(),
        lines[2][11..].trim().parse::<i32>().unwrap(),
    ];
    let program: Vec<i32> = lines[4][8..]
        .trim()
        .split(',')
        .map(|s| s.parse().unwrap())
        .collect();
    println!("a = {}\nb = {}\nc = {}\np = {:?}",registers[0], registers[1], registers[2], program);
    return Problem{a: registers[0], b:  registers[1], c: registers[2], program};
}

fn main() {
    let args = args();
    let problem = read_file(&args.last().expect("Please provide file"));
}
