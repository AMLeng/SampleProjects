#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <array>
#include <cctype>

typedef std::array<int,3> Loc;

Loc cube_wrap(Loc current){
    if(current[0] < 50 && current[1] < 100){
        switch(current[2]){
            case 2:
                return std::array<int,3>{{149-current[0],0,0}};
                break;
            case 3:
                return std::array<int,3>{{current[1]+100,0,0}};
                break;
            default:
                assert(false);
        }
    }
    if(current[0] < 50){
        switch(current[2]){
            case 0:
                return std::array<int,3>{{149-current[0],99,2}};
                break;
            case 1:
                return std::array<int,3>{{current[1]-50,99,2}};
                break;
            case 3:
                return std::array<int,3>{{199,current[1]-100,3}};
                break;
            default:
                assert(false);
        }
    }
    if(current[0] < 100){
        switch(current[2]){
            case 0:
                return std::array<int,3>{{49,current[0] + 50,3}};
                break;
            case 2:
                return std::array<int,3>{{100,current[0]-50,1}};
                break;
            default:
                assert(false);
        }
    }
    if(current [0] < 150 && current[1] < 50){
        switch(current[2]){
            case 2:
                return std::array<int,3>{{149-current[0],50,0}};
                break;
            case 3:
                return std::array<int,3>{{current[1]+50,50,0}};
                break;
            default:
                assert(false);
        }
    }
    if(current [0] < 150){
        switch(current[2]){
            case 0:
                return std::array<int,3>{{149-current[0],149,2}};
                break;
            case 1:
                return std::array<int,3>{{current[1]+100,49,2}};
                break;
            default:
                assert(false);
        }
    }
    if(current [0] < 200){
        switch(current[2]){
            case 0:
                return std::array<int,3>{{149,current[0]-100,3}};
                break;
            case 1:
                return std::array<int,3>{{0,current[1]+100,1}};
                break;
            case 2:
                return std::array<int,3>{{0,current[0]-100,1}};
                break;
            default:
                assert(false);
        }
    }
    assert(false);
}

class Grid{
    public:
        Grid(std::vector<std::vector<char>> g) : map(g) {}
        Loc alt_next(Loc current){
            int i = current[0];
            int j = current[1];
            int n = map.size();
            int m = map[i].size();
            switch(current[2]){
                case 0:
                    j++;
                    break;
                case 1:
                    i++;
                    break;
                case 2:
                    j--;
                    break;
                case 3:
                    i--;
                    break;
                default:
                    assert(false);
            }
            Loc pos = std::array<int,3>{{i,j,current[2]}};
            if(i >= 0 && i < n && j >= 0 && j < m && map.at(i).at(j) != ' '){
            }else{
                pos = cube_wrap(current);
            }
            if(map.at(pos[0]).at(pos[1]) == '#'){
                return current;
            }else{
                map.at(pos[0]).at(pos[1]) = '!';
                return pos;
            }
        }
        Loc next(Loc current){
            int i = current[0];
            int j = current[1];
            int n = map.size();
            int m = map[i].size();
            switch(current[2]){
                case 0:
                    while(map.at(i).at((j+1)%m) == ' '){
                        j++;
                    }
                    if(map.at(i).at((j+1)%m) == '#'){
                        return current;
                    }else{
                        j = (j+1)%m;
                    }
                    break;
                case 1:
                    while(map.at((i+1)%n).at(j) == ' '){
                        i++;
                    }
                    if(map.at((i+1)%n).at(j) == '#'){
                        return current;
                    }else{
                        i = (i+1)%n;
                    }
                    break;
                case 2:
                    while(map.at(i).at((j-1 + m)%m) == ' '){
                        j--;
                    }
                    if(map.at(i).at((j-1 + m)%m) == '#'){
                        return current;
                    }else{
                        j--;
                        if(j < 0){
                            j += m;
                        }
                        assert(j>=0);
                    }
                    break;
                case 3:
                    while(map.at((i-1+n)%n).at(j) == ' '){
                        i--;
                    }
                    if(map.at((i-1+n)%n).at(j) == '#'){
                        return current;
                    }else{
                        i--;
                        if(i<0){
                            i += n;
                        }
                    }
                    break;
                default:
                    assert(false);
            }
            map.at(i).at(j) = '!';
            return std::array<int,3>{{i,j,current[2]}};
        }
        Loc turn(Loc current, char c){
            int o = current[2];
            if(c == 'L'){
                o = (o+3) % 4;
            }else{
                assert(c == 'R');
                o = (o+1) % 4;
            }
            return std::array<int,3>{{current[0],current[1],o}};
        }
        void print(){
            for(int i=0; i<map.size(); i++){
                for(int j=0; j<map.at(i).size(); j++){
                    std::cout<<map.at(i).at(j);
                }
                std::cout<<std::endl;
            }
        }
    private:
        std::vector<std::vector<char>> map;
};
int value(Loc current){
    return 1000*(current[0]+1) + 4*(current[1]+1) + current[2];
}
int main(){
    std::string line;
    auto input = std::vector<std::vector<char>>();
    std::getline(std::cin,line);
    while(line.size() > 1){
        if(line.back() == '\r'){line.pop_back();}
        input.emplace_back();
        for(char c : line){
            input.back().emplace_back(c);
        }
        int rem = 150 - input.back().size();
        for(int i=0; i<rem; i++){
            input.back().emplace_back(' ');
        }
        std::getline(std::cin,line);
    }
    auto grid = Grid(input);
    //Now get line of instructions
    std::getline(std::cin,line);
    Loc current = std::array<int, 3>{{0,50,0}};

    if(line.back() == '\r'){line.pop_back();}
    while(line.size() > 0){
        if(std::isdigit(line.front())){
            int n = std::stoi(line);
            for(int i=0; i<n; i++){
                //current = grid.next(current); //This alternates between parts 1 and 2
                current = grid.alt_next(current);
            }
            while(std::isdigit(line.front())){
                line.erase(0,1);
            }
        }else{
            current = grid.turn(current, line.front());
            line.erase(0,1);
        }
    }
    std::cout<<value(current)<<std::endl;
}
