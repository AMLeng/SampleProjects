#!/usr/bin/env python
input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def part_one(equations):
    result = 0
    for test,(start,*values) in equations:
        cur = set({start})
        nex = set()
        for v in values:
            for c in cur:
                nex.update({c*v,c+v})
            cur = nex
            nex = set()
        if test in cur:
            result += test
    return result

def part_two(equations):
    result = 0
    for test,(start,*values) in equations:
        cur = set({start})
        nex = set()
        for v in values:
            for c in cur:
                for x in (c*v,c+v,int(str(c)+str(v))):
                    if x <= test:
                        nex.add(x)
            cur = nex
            nex = set()
        if test in cur:
            result += test
    return result

if __name__ == '__main__':
    def parse(line: str):
        test,eq = line.strip().split(':')
        return (int(test),[int(e) for e in eq.split()])

    with open(input_path, 'r') as f:
        equations = [parse(l) for l in f]
    #equations = [(190, [10, 19]), (3267, [81, 40, 27]), (83, [17, 5]), (156, [15, 6]), (7290, [6, 8, 6, 15]), (161011, [16, 10, 13]), (192, [17, 8, 14]), (21037, [9, 7, 18, 13]), (292, [11, 6, 16, 20])]
    print(part_one(equations))
    print(part_two(equations))
