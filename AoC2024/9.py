#!/usr/bin/env python
input_path = '/mnt/c/Users/eagle/Downloads/input.txt'


def part_one(disk):
    checksum = 0
    back_id = len(disk)//2
    back_index = len(disk) - 1
    current_pos = 0
    while current_pos <=back_index:
    return checksum
if __name__ == '__main__':
    def parse(line: str):
        test,eq = line.strip().split(':')
        return (int(test),[int(e) for e in eq.split()])

    with open(input_path, 'r') as f:
        disk,*_ = [list(map(int,list(l.strip()))) for l in f]
    disk = [2, 3, 3, 3, 1, 3, 3, 1, 2, 1, 4, 1, 4, 1, 3, 1, 4, 0, 2]
    assert(len(disk) % 2 == 1) # Must be odd so that it ends with an allocation
    print(part_one(disk))
