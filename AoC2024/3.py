#!/usr/bin/env python

input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

import re

def part_one(line):
    regex = r'mul\([0-9]+,[0-9]+\)'
    matches = re.findall(regex,line)
    result = 0
    for match in matches:
        a,b = map(int,re.findall('[0-9]+',match))
        result += a*b
    return result


def part_two(line):
    enabled = True
    regex = r'mul\([0-9]+,[0-9]+\)|do\(\)|don\'t\(\)'
    matches = re.findall(regex,line)
    result = 0
    for match in matches:
        if re.match(r'do\(\)',match):
            enabled = True
            continue
        if re.match(r'don\'t\(\)',match):
            enabled = False
            continue
        if enabled:
            a,b = map(int,re.findall('[0-9]+',match))
            result += a*b
    return result

if __name__ == '__main__':
    with open(input_path, 'r') as f:
        line = ''.join(l for l in f)
    #line = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"
    print(part_one(line))
    #line = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"
    print(part_two(line))
