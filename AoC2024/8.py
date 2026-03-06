#!/usr/bin/env python
input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def in_bounds(city, pos):
    i,j = pos
    return i>=0 and j>=0 and i<len(city) and j<len(city[0])

symbols = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'

def part_one(city):
    antennas = dict()
    nodes = set()
    for s in symbols:
        antennas[s] = []
    for i,row in enumerate(city):
        for j,val in enumerate(row):
            if val in symbols:
                antennas[val].append((i,j))
    for s in symbols:
        a = antennas[s]
        for i in range(len(a)):
            for j in range(i+1,len(a)):
                for pos in ((2*a[i][0]-a[j][0],2*a[i][1]-a[j][1]),(2*a[j][0]-a[i][0],2*a[j][1]-a[i][1])):
                    if in_bounds(city,pos):
                        nodes.add(pos)
    return len(nodes)

def part_two(city):
    antennas = dict()
    nodes = set()
    for s in symbols:
        antennas[s] = []
    for i,row in enumerate(city):
        for j,val in enumerate(row):
            if val in symbols:
                antennas[val].append((i,j))
    for s in symbols:
        a = antennas[s]
        for i in range(len(a)):
            for j in range(i+1,len(a)):
                d = (a[i][0] - a[j][0], a[i][1]-a[j][1])
                pos = a[i]
                while in_bounds(city,pos):
                    nodes.add(pos)
                    pos = (pos[0]+d[0],pos[1]+d[1])
                pos = a[j]
                while in_bounds(city,pos):
                    nodes.add(pos)
                    pos = (pos[0]-d[0],pos[1]-d[1])
    return len(nodes)

if __name__ == '__main__':
    def parse(line: str):
        test,eq = line.strip().split(':')
        return (int(test),[int(e) for e in eq.split()])

    with open(input_path, 'r') as f:
        city = [l.strip() for l in f]
    #city = ["............","........0...",".....0......",".......0....","....0.......","......A.....","............","............","........A...",".........A..","............","............"]
    print(part_one(city))
    print(part_two(city))
