use ndarray::prelude::*;
use std::env;
use std::fs::File;
use std::io::{BufRead, BufReader};

struct Problem {
    grid: Array2<char>,
    xsize: usize,
    ysize: usize,
    moves: Vec<char>,
    loc: (usize, usize),
}

fn find_robot(grid: &Vec<Vec<char>>) -> Option<(usize, usize)> {
    for (row_idx, row) in grid.iter().enumerate() {
        if let Some(col_idx) = row.iter().position(|&x| x == '@') {
            return Some((row_idx, col_idx));
        }
    }
    return None;
}

fn parse_file(fname: &String) -> Problem {
    let file = File::open(fname).expect(&format!("'{fname}' could not be opened"));
    let reader = BufReader::new(file);

    let mut grid: Vec<Vec<char>> = Vec::new();

    let mut reading_grid = true;
    let mut moves: Vec<char> = Vec::new();

    for line in reader.lines() {
        let line = line.unwrap();
        if line.is_empty() {
            reading_grid = false;
            continue;
        }
        if reading_grid {
            grid.push(line.chars().collect());
        } else {
            moves.extend(line.chars());
        }
    }
    let xsize = grid.first().unwrap().len();
    let ysize = grid.len();

    let loc = find_robot(&grid).unwrap();

    let grid: Array2<char> =
        Array::from_shape_vec((ysize, xsize), grid.concat()).expect("Failed conversion");

    return Problem {
        grid,
        xsize,
        ysize,
        moves,
        loc,
    };
}

fn print_problem(problem: &Problem) {
    let Problem {
        grid,
        xsize,
        ysize,
        loc,
        moves,
    } = &problem;

    println!("Grid: {ysize} x {xsize}");
    println!("{}", grid);

    println!("Robot located: {loc:?}\nmoves = {moves:?}");
}

fn get_new_slice(slice: &[char], _do_reverse: bool) -> Vec<char> {
    if let Some((index, &value)) = slice.iter().enumerate().find(|(_, &ch)| ch == '#') {
        println!("Found value '{}' at index {}", value, index);
    }

    let slice_iter = slice.iter();

    let moved: Vec<char> = slice_iter
        .filter(|&&x| x == '@' || x == 'O')
        .copied()
        .collect();

    let mut new_slice: Vec<char> = vec!['.'; slice.len() - moved.len() - 1];
    new_slice.extend(&moved);
    new_slice.push('#');
    return new_slice;
}

fn make_step(grid: &mut Array2<char>, loc: &mut (usize, usize), dir: char) {
    match dir {
        '^' => {}
        '>' => {
            let slice = grid.slice_mut(s![loc.0,loc.1..]);
            println!("{:?}", slice);
            // let new_slice = get_new_slice(slice, false);
            // slice.copy_from_slice(&new_slice);
            // println!("{:?}", slice);
        }
        'v' => {}
        '<' => {
            let slice = grid.slice_mut(s![loc.0,..loc.1]);
            println!("{:?}", slice);

        }
        _ => println!("Error"),
    }
}

fn solve_problem(problem: &mut Problem) {
    let moves = problem.moves.clone();
    make_step(&mut problem.grid, &mut problem.loc, '<');
    // print_problem(problem);
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut problem = parse_file(&args[1]);
    print_problem(&problem);
    solve_problem(&mut problem);
}
