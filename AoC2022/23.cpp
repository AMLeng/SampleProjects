#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cassert>
#include <unordered_map>
#include <utility>
#include <climits>

//Currently takes 4-5 seconds to run

namespace std{
    template<>
    struct hash<std::pair<int,int>>{
        size_t operator()(const std::pair<int,int> &p) const{
            return static_cast<unsigned int>(p.first + 10000*p.second);
        }
    };
}
class Grid{
    public:
    Grid(std::vector<std::string> lines) : none_moved(false){
        int i = 0;
        for(auto line : lines){
            int j=0;
            for(char c : line){
                if(c == '#'){
                    grid.emplace(std::make_pair(i,j),0);
                }
                j++;
            }
            i++;
        }
    }
    void update(int round){
        if(none_moved){
            return;
        }
        none_moved = true;
        for(const auto pair : grid){
            grid.at(pair.first) = movement_direction(pair.first, round);
        }
        auto new_grid = std::unordered_map<std::pair<int,int>,int>();
        for(const auto pair : grid){
            if(pair.second == 0){
                new_grid.insert(std::move(pair));
            }else{
                auto p = std::make_pair(pair.first.first+(pair.second %2),pair.first.second+(pair.second %3));
                if(available(p)){
                    new_grid.emplace(p,0);
                    none_moved = false;
                }else{
                    new_grid.emplace(pair.first,0);
                }
            }
        }
        grid = new_grid;
    }
    bool finished(){
        return none_moved;
    }
    int empty_squares() const{
        int minx = INT_MAX;
        int miny = INT_MAX;
        int maxx = INT_MIN;
        int maxy = INT_MIN;
        for(const auto pair : grid){
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
        return total - grid.size();
    }
    void print(){
        for(const auto pair : grid){
            std::cout<<pair.first.first<<','<<pair.first.second<<":  "<<pair.second<<std::endl;
        }
        std::cout<<std::endl;
    }
    private:
    std::unordered_map<std::pair<int,int>,int> grid;
    bool none_moved;
    int movement_direction(const std::pair<int,int> point, int round) const{
        assert(grid.at(point) == 0);
        auto no_elf = std::bitset<9>();
        //std::cout<<"Checking neighbors of "<<point.first<<','<<point.second<<std::endl;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                bool has_elf = grid.find(std::make_pair(point.first + i - 1,point.second + j - 1)) != grid.end();
                //std::cout<<has_elf<<std::endl;
                no_elf.set(3*i+j,!has_elf);
            }
        }
        no_elf.set(3*1+1,true);
        if(no_elf.all()){
            return 0;
        }
        for(int k=0; k<4; k++){
            switch((k+round)%4){
                case 0:
                    if(no_elf[0] && no_elf[1] && no_elf[2]){
                        return -3;
                    }
                    break;
                case 1:
                    if(no_elf[6] && no_elf[7] && no_elf[8]){
                        return 3;
                    }
                    break;
                case 2:
                    if(no_elf[0] && no_elf[3] && no_elf[6]){
                        return -4;
                    }
                    break;
                case 3:
                    if(no_elf[2] && no_elf[5] && no_elf[8]){
                        return 4;
                    }
                    break;
            }
        }
        return 0;
    }
    bool available(const std::pair<int,int> point) const{
        int count = 0;
        auto p = std::make_pair(point.first+1,point.second);
        if(grid.find(p) != grid.end()){
            int move_dir = grid.at(p);
            if(move_dir % 2 == -1 && move_dir % 3 == 0){
                count++;
            }
        }
        p = std::make_pair(point.first-1,point.second);
        if(grid.find(p) != grid.end()){
            int move_dir = grid.at(p);
            if(move_dir % 2 == 1 && move_dir % 3 == 0){
                count++;
            }
        }
        p = std::make_pair(point.first,point.second+1);
        if(grid.find(p) != grid.end()){
            int move_dir = grid.at(p);
            if(move_dir % 2 == 0 && move_dir % 3 == -1){
                count++;
            }
        }
        p = std::make_pair(point.first,point.second-1);
        if(grid.find(p) != grid.end()){
            int move_dir = grid.at(p);
            if(move_dir % 2 == 0 && move_dir % 3 == 1){
                count++;
            }
        }
        return count < 2;
    }
};

int main(){
    std::string line;
    auto lines = std::vector<std::string>();

    while(std::getline(std::cin,line)){
        if(line.back() == '\r'){line.pop_back();}
        lines.emplace_back(line);
    }
    auto grid = Grid(lines);
    for(int i=0; i<10; i++){
        grid.update(i);
    }
    std::cout<<grid.empty_squares()<<std::endl;
    int i=10;
    while(!grid.finished()){
        grid.update(i);
        i++;
    }
    std::cout<<"FINISHED ON ROUND "<<i<<std::endl;
}
