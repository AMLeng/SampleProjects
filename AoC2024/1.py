#!/usr/bin/env python

input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def part_one(left, right):
    return sum([abs(r-l) for r,l in zip(sorted(left),sorted(right))])

def part_two(left, right):
    l = {x: left.count(x) for x in left}
    r = {x: right.count(x) for x in right}
    sum = 0
    for k,v in l.items():
        if k in r:
            sum += k*v*r[k]
    return sum

if __name__ == '__main__':
    with open(input_path, 'r') as f:
        ints = (map(int,line.split()) for line in f)
        # We know there will be exactly two values in each line
        left, right = zip(*ints)
    #left = [3,4,2,1,3,3]
    #right = [4,3,5,3,9,3]
    print(part_one(left, right))
    print(part_two(left, right))
