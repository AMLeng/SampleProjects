#!/usr/bin/env python

input_path = '/mnt/c/Users/eagle/Downloads/input.txt'

def is_valid(rules, update):
    seen = set()
    for num in update:
        seen.add(num)
        if not num in rules:
            continue
        if rules[num].intersection(seen):
            return False
    return True

def part_one(rules, updates):
    result = 0
    for update in updates:
        if is_valid(rules,update):
            result += update[len(update)//2]
    return result

def part_two(rules, updates):
    result = 0
    for update in updates:
        if is_valid(rules,update):
            continue
        def rearrange(update):
            r = []
            us = set(update)
            seen = set()
            def add_next(num):
                if num in seen:
                    return
                if num in rules:
                    for n in us.intersection(rules[num]):
                        add_next(n)
                seen.add(num)
                r.append(num)
            for u in update:
                add_next(u)
            return r
        result += rearrange(update)[len(update)//2]
    return result

if __name__ == '__main__':
    with open(input_path, 'r') as f:
        rules = dict()
        while line := f.readline().strip():
            k, v = map(int,line.split('|'))
            if k in rules:
                rules[k].add(v)
            else:
                rules[k] = set({v})
        updates = [list(map(int,l.strip().split(','))) for l in f]
    #rules = {47: {53, 13, 61, 29}, 97: {13, 61, 47, 29, 53, 75}, 75: {29, 53, 47, 61, 13}, 61: {13, 53, 29}, 29: {13}, 53: {29, 13}}
    #updates = [[75,47,61,53,29],[97,61,53,29,13],[75,29,13],[75,97,47,61,53],[61,13,29],[97,13,75,29,47]]
    print(part_one(rules,updates))
    print(part_two(rules,updates))
