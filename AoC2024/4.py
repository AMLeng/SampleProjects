#!/usr/bin/env python

input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def part_one(grid):
    xmas = ['X','M','A','S']
    samx = ['S','A','M','X']
    n = len(grid)
    m = len(grid[0])
    count = 0
    for i in range(n-3):
        for j in range(m-3):
            word = [grid[i+a][j+a] for a in range(4)]
            if word == xmas:
                count += 1
            if word == samx:
                count += 1

    for i in range(3,n):
        for j in range(m-3):
            word = [grid[i-a][j+a] for a in range(4)]
            if word == xmas:
                count += 1
            if word == samx:
                count += 1

    for i in range(n-3):
        for j in range(m):
            word = [grid[i+a][j] for a in range(4)]
            if word == xmas:
                count += 1
            if word == samx:
                count += 1
    for i in range(n):
        for j in range(m-3):
            word = [grid[i][j+a] for a in range(4)]
            if word == xmas:
                count += 1
            if word == samx:
                count += 1
    return count

def part_two(grid):
    n = len(grid)
    m = len(grid[0])
    count = 0
    for i in range(1,n-1):
        for j in range(1,m-1):
            if grid[i][j] != 'A':
                continue
            if {grid[i-1][j-1],grid[i+1][j+1]} != {'M','S'}:
                continue
            if {grid[i+1][j-1],grid[i-1][j+1]} != {'M','S'}:
                continue
            count += 1
    return count

if __name__ == '__main__':
    with open(input_path, 'r') as f:
        grid = [l for l in f]
    #grid = ["MMMSXXMASM","MSAMXMSMSA","AMXSXMAAMM","MSAMASMSMX","XMASAMXAMM","XXAMMXXAMA","SMSMSASXSS","SAXAMASAAA","MAMMMXMMMM","MXMXAXMASX"]
    print(part_one(grid))
    print(part_two(grid))
