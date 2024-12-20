use ndarray::prelude::*;
use std::env;
use std::fs::File;
use std::io::{BufRead, BufReader};

struct Problem {
    grid: Array2<char>,
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
            let mut wide_row: Vec<char> = Vec::new();
            for e in line.chars() {
                match e {
                    '#' => wide_row.extend(vec!['#', '#']),
                    'O' => wide_row.extend(vec!['[', ']']),
                    '.' => wide_row.extend(vec!['.', '.']),
                    '@' => wide_row.extend(vec!['@', '.']),
                    _ => panic!("Problem parsing"),
                }
            }

            grid.push(wide_row);
        } else {
            moves.extend(line.chars());
        }
    }
    let xsize = grid.first().unwrap().len();
    let ysize = grid.len();

    let loc = find_robot(&grid).unwrap();

    let grid: Array2<char> =
        Array::from_shape_vec((ysize, xsize), grid.concat()).expect("Failed conversion");

    return Problem { grid, moves, loc };
}

fn print_problem(problem: &Problem) {
    let Problem {
        grid,
        loc,
        moves,
    } = &problem;

    println!("{}", grid);

    println!("Robot located: {loc:?}\nmoves = {moves:?}");
}

fn get_new_slice(slice: ArrayView1<char>, _do_reverse: bool) -> Option<Vec<char>> {
    let (index, val) = slice
        .iter()
        .enumerate()
        .find(|(_, &x)| x == '#' || x == '.')
        .unwrap();
    // println!("slice = {slice}");
    // println!("Found '{val}' at {index}");

    if *val == '.' {
        let mut new_range: Vec<char> = Vec::new();
        new_range.push('.');
        let pre = slice.slice(s![0..index]);
        let post = slice.slice(s![index + 1..]);
        new_range.extend(pre);
        new_range.extend(post);
        // println!("new_range = {new_range:?}");
        return Some(new_range);
    }
    return None;
}

fn make_step(grid: &mut Array2<char>, loc: &mut (usize, usize), dir: char) {
    match dir {
        '^' => {
            let col = grid.column(loc.1);
            let slice = col.slice(s![..loc.0 + 1; -1]);
            if let Some(new_slice) = get_new_slice(slice, false) {
                let arr = Array1::from_vec(new_slice);
                let mut new_loc: (usize, usize) = (0, 0);
                for i in 0..arr.len() {
                    grid[[loc.0 - i, loc.1]] = arr[i];
                    if arr[i] == '@' {
                        new_loc = (loc.0 - i, loc.1);
                    }
                }
                *loc = new_loc.clone();
            }
        }
        'v' => {
            let slice_range = s![loc.0.., loc.1];
            let slice = grid.slice(&slice_range);
            if let Some(new_slice) = get_new_slice(slice, false) {
                let arr = Array1::from_vec(new_slice);
                grid.slice_mut(slice_range).assign(&arr);
                *loc = (loc.0 + arr.iter().position(|&x| x == '@').unwrap(), loc.1);
            }
        }
        '>' => {
            let slice_range = s![loc.0, loc.1..];
            let slice = grid.slice(&slice_range);
            if let Some(new_slice) = get_new_slice(slice, false) {
                let arr = Array1::from_vec(new_slice);
                grid.slice_mut(slice_range).assign(&arr);
                *loc = (loc.0, loc.1 + arr.iter().position(|&x| x == '@').unwrap());
            }
        }
        '<' => {
            let slice_range = s![loc.0,..loc.1 + 1; -1];
            let slice = grid.slice(&slice_range);
            if let Some(new_slice) = get_new_slice(slice, false) {
                let arr = Array1::from_vec(new_slice);
                grid.slice_mut(slice_range).assign(&arr);
                *loc = (loc.0, loc.1 - arr.iter().position(|&x| x == '@').unwrap());
            }
        }
        _ => println!("Error"),
    }
}

fn evaluate_gps_coords(grid: &Array2<char>) -> usize {
    let mut coords: Vec<(usize, usize)> = Vec::new();
    for (row_idx, row) in grid.outer_iter().enumerate() {
        for (col_idx, val) in row.iter().enumerate() {
            if *val == 'O' {
                coords.push((row_idx, col_idx));
            }
        }
    }
    let sum = coords.iter().fold(0, |acc, &x| acc + (x.1 + 100 * x.0));
    return sum;
}

fn solve_problem(problem: &mut Problem) {
    for m in &problem.moves {
        make_step(&mut problem.grid, &mut problem.loc, *m);
    }
    let sum = evaluate_gps_coords(&problem.grid);
    println!("sum = {sum}");
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut problem = parse_file(&args[1]);
    solve_problem(&mut problem);
}
