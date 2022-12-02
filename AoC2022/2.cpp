#include <iostream>
#include <string>
#include <cassert>

int count_points(char a, char x){
    int them = a-'A';
    int us = x-'X';
    int points = us + 1;
    if((3+us-them)%3 == 1){
        points += 6;
    }else{
        if((3+us-them)%3 == 0){
            points += 3;
        }
    }
    return points;
}

int count_points2(char a, char x){
    int them = a-'A';
    int us = x-'X';
    switch(us){
        case 0:
            return((3+them - 1)%3)+1;
            break;  
        case 1:
            return((3+them)%3)+4;
            break;
        case 2:
            return((them + 1)%3)+7;
            break;
        default:
            assert(false);
            return 0;
            break;
    }
}
int main(){
    std::string nextline;
    int total = 0;
    while(std::getline(std::cin,nextline)){
        total += count_points(nextline[0], nextline[2]);
        total2 += count_points2(nextline[0], nextline[2]);
    }
    std::cout<<total<<std::endl;
    std::cout<<total2<<std::endl;
}
