#include <iostream>
#include <vector>
#include <cassert>

bool step(std::vector<std::vector<char>>& grid){
    int n = grid.size();
    int m = grid.back().size();
    bool movement = false;

    for(int i=0; i< n; i++){
        bool move_last = (grid.at(i).at(0) == '.' && grid.at(i).at(m-1) == '>');
        for(int j=0; j< m - 1; j++){
            if(grid.at(i).at(j) == '>' && grid.at(i).at(j+1) == '.'){
                grid.at(i).at(j) = '.';
                grid.at(i).at(j+1) = '>';
                movement = true;
                j++;
            }
        }
        if(move_last){
            grid.at(i).at(0) = '>';
            grid.at(i).at(m-1) = '.';
            movement = true;
        }
    }

    for(int j=0; j< m ; j++){
        bool move_last = (grid.at(0).at(j) == '.'&& grid.at( n -1).at(j) =='v');
        for(int i=0; i< n - 1; i++){
            if(grid.at(i).at(j) == 'v' && grid.at(i + 1).at(j) == '.'){
                grid.at(i).at(j) = '.';
                grid.at(i + 1).at(j) = 'v';
                movement = true;
                i++;
            }
        }
        if(move_last){
            grid.at(0).at(j) = 'v';
            grid.at(n-1).at(j) = '.';
            movement = true;
        }
    }
    return movement;
}
void print(const std::vector<std::vector<char>> grid){
    int n = grid.size();
    int m = grid.back().size();

    for(int i=0; i< n; i++){
        for(int j=0; j< m; j++){
            std::cout<<grid.at(i).at(j);
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}
int main(){
    std::string line;
    auto grid = std::vector<std::vector<char>>();
    while(std::getline(std::cin,line)){
        if(line.back() == '\r'){line.pop_back();}
        grid.emplace_back();
        //grid.emplace_back(std::vector<char>());
        for(char c : line){
            assert(c == '>' || c == '.' || c == 'v');
            grid.back().emplace_back(c);
        }
    }
    int time = 0;
    do{
        //std::cout<<time<<std::endl;
        //print(grid);
        time++;
    }
    while(step(grid));
    std::cout<<time<<std::endl;
}
