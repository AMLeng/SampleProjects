#include <iostream>
#include <string>
#include <cassert>
#include <map>
#include <utility>
#include <climits>

//Currently takes 17-18 seconds to run
//In addition to the very poorly structured code
//And the use of an assert to terminate the code when finished

bool has_neighbor(const std::map<std::pair<int,int>,int>& grid, const std::pair<int,int> point){
    assert(grid.at(point) == 0);
    for(int i=-1; i<= 1; i++){
        for(int j=-1; j<= 1; j++){
            if(i == 0 && j == 0){
                continue;
            }
            if(grid.find(std::make_pair(point.first + i,point.second + j)) != grid.end()){
                return true;
            }
        }
    }
    return false;
}
int movement_direction(const std::map<std::pair<int,int>,int>& grid, const std::pair<int,int> point, int round){
    assert(grid.at(point) == 0);
    bool none = true;
    for(int k=0; k<4; k++){
        switch((k+round)%4){
        case 0:
            for(int i=-1; i<=1; i++){
                if(grid.find(std::make_pair(point.first - 1,point.second + i))!= grid.end()){
                    none = false;
                    break;
                }
            }
            if(none){
                return -3;
            }
            none = true;
            break;
        case 1:
            for(int i=-1; i<=1; i++){
                if(grid.find(std::make_pair(point.first + 1,point.second + i))!= grid.end()){
                    none = false;
                    break;
                }
            }
            if(none){
                return 3;
            }
            none = true;
            break;
        case 2:
            for(int i=-1; i<=1; i++){
                if(grid.find(std::make_pair(point.first + i,point.second - 1))!= grid.end()){
                    none = false;
                    break;
                }
            }
            if(none){
                return -4;
            }
            none = true;
            break;
        case 3:
            for(int i=-1; i<=1; i++){
                if(grid.find(std::make_pair(point.first + i,point.second + 1))!= grid.end()){
                    none = false;
                    break;
                }
            }
            if(none){
                return 4;
            }
            none = true;
            break;
        }
    }
    return 0;
}
bool available(const std::map<std::pair<int,int>,int>& grid, const std::pair<int,int> point){
    /*if(grid.find(point) != grid.end()){
        assert(grid.at(point) == 0);
        return true;
    }*/
    int count = 0;
    for(int i=-1; i<=1; i++){
        for(int j=-1; j<=1; j++){
            auto p = std::make_pair(point.first+i,point.second+j);
            if(grid.find(p) != grid.end()){
                int move_dir = grid.at(p);
                if(move_dir % 2 == -i && move_dir % 3 == -j){
                    count++;
                }
            }
        }
    }
    return count < 2;
}
std::map<std::pair<int,int>,int> update(std::map<std::pair<int,int>,int>& grid, int round){
    bool none_moved = true;
    for(const auto pair : grid){
        if(has_neighbor(grid,pair.first)){
            grid.at(pair.first) = movement_direction(grid,pair.first, round);
        }
    }
    auto new_grid = std::map<std::pair<int,int>,int>();
    for(const auto pair : grid){
        auto p = std::make_pair(pair.first.first+(pair.second %2),pair.first.second+(pair.second %3));
        if(available(grid,p)){
            new_grid.emplace(p,0);
            if(p != pair.first){
                none_moved = false;
            }
        }else{
            new_grid.emplace(pair.first,0);
        }
    }
    if(none_moved){
        std::cout<<"ON ROUND "<<round+1<<std::endl;
        assert(false);
    }
    return new_grid;
}

int main(){
    std::string line;
    auto grid = std::map<std::pair<int,int>,int>();
    int i=0;
    while(std::getline(std::cin,line)){
        if(line.back() == '\r'){line.pop_back();}
        int j=0;
        for(char c : line){
            if(c == '#'){
                grid.emplace(std::make_pair(i,j),0);
            }
            j++;
        }
        i++;
    }
    //for(int i=0; i<10; i++){
    for(int i=0; i< INT_MAX; i++){
        /*std::cout<<i<<std::endl;
        for(const auto pair : grid){
            std::cout<<pair.first.first<<','<<pair.first.second<<":  "<<pair.second<<std::endl;
        }
        std::cout<<std::endl;*/
        grid = update(grid, i);
    }
    int minx = INT_MAX;
    int miny = INT_MAX;
    int maxx = INT_MIN;
    int maxy = INT_MIN;
    for(const auto pair : grid){
        std::cout<<pair.first.first<<','<<pair.first.second<<":  "<<pair.second<<std::endl;
        if(pair.first.first < minx){
            minx = pair.first.first;
        }
        if(pair.first.first > maxx){
            maxx = pair.first.first;
        }
        if(pair.first.second < miny){
            miny = pair.first.second;
        }
        if(pair.first.second > maxy){
            maxy = pair.first.second;
        }
    }
    std::cout<<minx<<','<<maxx<<std::endl;
    std::cout<<miny<<','<<maxy<<std::endl;
    int total = (maxx - minx + 1)*(maxy - miny + 1);
    std::cout<<total-grid.size()<<std::endl;
}
