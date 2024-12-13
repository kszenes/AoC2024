use ndarray::Array2;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::path::Path;

fn read_file(fname: &String) -> Array2<char> {
    let file = match File::open(Path::new(fname)) {
        Ok(file) => file,
        Err(e) => {
            eprintln!("Error opening file: {}", e);
            return Array2::from_elem((0, 0), ' '); // Return empty array on error
        }
    };
    let buffered = BufReader::new(file);
    let mut v = Vec::<Vec<char>>::new();

    for line in buffered.lines() {
        match line {
            Ok(line) => v.push(line.chars().collect()), // Convert line to Vec<char>
            Err(e) => {
                eprintln!("Error reading line: {}", e);
                return Array2::from_elem((0, 0), ' '); // Return empty array on error
            }
        }
    }
    let size = v.len();
    let array: Array2<char> = Array2::from_shape_fn((size, size), |(i, j)| {
        return v[i][j];
    });

    return array;
}

struct Island {
    val: char,
    locs: Vec<(usize, usize)>,
}

fn visit_cell(
    start_x: usize,
    start_y: usize,
    grid: &Array2<char>,
    visited: &mut Array2<bool>,
) -> Island {
    let size = grid.shape()[0];
    let mut queue: Vec<(usize, usize)> = Vec::new();
    queue.push((start_x, start_y));
    let val = grid[[start_x, start_y]];
    visited[[start_x, start_y]] = true;
    let mut locs: Vec<(usize, usize)> = Vec::new();
    while !queue.is_empty() {
        let (x, y) = *queue.last().unwrap();
        // println!("queue = {queue:?}");
        // println!("({}, {})", x, y);
        locs.push((x, y));
        queue.pop();
        if x + 1 < size && !visited[[x + 1, y]] && grid[[x + 1, y]] == val {
            visited[[x + 1, y]] = true;
            queue.push((x + 1, y));
        }
        if y + 1 < size && !visited[[x, y + 1]] && grid[[x, y + 1]] == val {
            visited[[x, y + 1]] = true;
            queue.push((x, y + 1));
        }
        if x > 0 && !visited[[x - 1, y]] && grid[[x - 1, y]] == val {
            visited[[x - 1, y]] = true;
            queue.push((x - 1, y));
        }
        if y > 0 && !visited[[x, y - 1]] && grid[[x, y - 1]] == val {
            visited[[x, y - 1]] = true;
            queue.push((x, y - 1));
        }
    }
    return Island { val, locs };
}

fn get_num_neighbors(grid: &Array2<char>, val: char, loc: (usize, usize)) -> i32 {
    let size = grid.nrows();
    let mut num_neighbors = 0;
    let (x, y) = loc;
    if x > 0 && grid[[x - 1, y]] == val {
        num_neighbors += 1;
    }
    if y > 0 && grid[[x, y - 1]] == val {
        num_neighbors += 1;
    }
    if x + 1 < size && grid[[x + 1, y]] == val {
        num_neighbors += 1;
    }
    if y + 1 < size && grid[[x, y + 1]] == val {
        num_neighbors += 1;
    }
    return num_neighbors;
}

fn score_island(grid: &Array2<char>, island: &Island) -> (i32, i32) {
    // Compute area
    let area = island.locs.len();
    // Compute perimeter
    let val = island.val;
    let mut perim = 0;
    for loc in island.locs.clone() {
        perim += 4 - get_num_neighbors(grid, val, loc);
    }
    return (area as i32, perim);
}

fn solve_grid(grid: &Array2<char>) {
    let mut visited: Array2<bool> = Array2::from_elem((grid.nrows(), grid.ncols()), false);
    let mut islands: Vec<Island> = Vec::new();
    while true {
        if let Some(loc) = visited.indexed_iter().find(|(_, &x)| !x) {
            islands.push(visit_cell(loc.0 .0, loc.0 .1, grid, &mut visited));
        } else {
            break;
        }
    }

    let mut score = 0;
    for island in islands {
        let (area, perim) = score_island(grid, &island);
        // println!("{}: {area}, {perim}", island.val);
        score += area * perim;
    }
    println!("score = {score}");
}

fn main() {
    let grid = read_file(&"inputs/long.txt".to_string());
    // println!("{grid:?}");
    solve_grid(&grid);
}
