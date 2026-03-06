#!/usr/bin/env python

input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def get_diffs(levels):
    return [levels[i+1] - levels[i] for i in range(len(levels)-1)]
def gradual(diffs):
    return all(1 <= abs(x) <= 3 for x in diffs)
def monotonic(diffs):
    return all(x < 0 for x in diffs) or all(x > 0 for x in diffs)

def part_one(reports):
    diffs = map(get_diffs, reports)
    return sum(1 for diff in diffs if gradual(diff) and monotonic(diff))

def part_two(reports):
    result = 0
    for index,level in enumerate(reports):
        diff = get_diffs(level)
        if gradual(diff) and monotonic(diff):
            result += 1
            continue
        # Now we can assume not monotonic somewhere
        def make_copy_without(diff, index):
            if index == -1:
                return diff[1:]
            if index == len(diff) - 1:
                return diff[:index]
            return diff[:index] +[diff[index]+diff[index+1]]+ diff[index+2:]
        possible_i = [i for i in range(-1,len(diff)) if gradual(d:= make_copy_without(diff,i)) and monotonic(d)]
        if possible_i:
            result += 1
    return result

if __name__ == '__main__':
    with open(input_path, 'r') as f:
        reports = [list(map(int,line.split())) for line in f]
    #reports=[[7,6,4,2,1],[1,2,7,8,9],[9,7,6,2,1],[1,3,2,4,5],[8,6,4,4,1],[1,3,6,7,9]]
    print(part_one(reports))
    print(part_two(reports))
