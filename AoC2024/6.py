#!/usr/bin/env python
input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def find_guard(lab_map):
    for i in range(len(lab_map)):
        for j in range(len(lab_map[i])):
            if lab_map[i][j] == '^':
                return i,j,(-1,0)
            if lab_map[i][j] == '>':
                return i,j,(0,1)
            if lab_map[i][j] == 'v':
                return i,j,(1,0)
            if lab_map[i][j] == '<':
                return i,j,(0,-1)

def next_direction(direction):
    if direction == (-1,0):
        return (0,1)
    if direction == (0,1):
        return (1,0)
    if direction == (1,0):
        return (0,-1)
    return (-1,0)

def in_bounds(i,j,lab_map):
    return i >= 0 and j >= 0 and i < len(lab_map) and j < len(lab_map[0])

def next_pos(i,j,direction,lab_map, extra_obstruction = None):
    a = i + direction[0]
    b = j + direction[1]
    try:
        if lab_map[a][b] == '#' or (a,b) == extra_obstruction:
            return i,j,next_direction(direction)
    except IndexError:
        pass
    # Treat out of bounds, '.', and the guard initial position as the same
    return a,b,direction

def part_one(lab_map):
    seen = set()
    i,j,direction = find_guard(lab_map)
    while in_bounds(i,j,lab_map):
        seen.add((i,j))
        i,j,direction = next_pos(i,j,direction,lab_map)
    return len(seen)

def part_two(lab_map):
    seen = set()
    i,j,direction = find_guard(lab_map)
    start_i, start_j, start_direction = i,j, direction
    while in_bounds(i,j,lab_map):
        seen.add((i,j))
        i,j,direction = next_pos(i,j,direction,lab_map)

    seen.remove((start_i,start_j))
    valid_obstructions = set()
    for obstruction in seen:
        i,j,direction = start_i, start_j, start_direction
        states = set()
        while in_bounds(i,j,lab_map):
            states.add((i,j,direction))
            i,j,direction = next_pos(i,j,direction,lab_map, obstruction)
            if (i,j,direction) in states:
                valid_obstructions.add(obstruction)
                break

    return len(valid_obstructions)

if __name__ == '__main__':
    with open(input_path, 'r') as f:
        lab_map = [l.strip() for l in f]
    #lab_map = ['....#.....','.........#','..........','..#.......','.......#..','..........','.#..^.....','........#.','#.........','......#...']
    print(part_one(lab_map))
    print(part_two(lab_map))
