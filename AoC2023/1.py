#!/usr/bin/python3
digit_map = {
    'one'   : '1',
    'two'   : '2',
    'three' : '3',
    'four'  : '4',
    'five'  : '5',
    'six'   : '6',
    'seven' : '7',
    'eight' : '8',
    'nine'  : '9'
}

part_two = True

def get_num(line, is_left):
    for i in range(len(line)) if is_left else reversed(range(len(line))):
        if line[i].isdigit():
            return line[i]
        elif part_two:
            for key in digit_map:
                s = slice(i,i+len(key)) if is_left else slice(i-len(key),i)
                if line[s] == key: 
                    return digit_map[key]
    return "ERROR at"+line+"\n"

def main():
    with open("1_input.txt", "r") as f:
        sum = 0
        for line in f:
            sum += int(get_num(line,True)+get_num(line,False))
        print(sum)


if __name__ == "__main__":
    main()
