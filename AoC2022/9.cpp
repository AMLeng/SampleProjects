#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <cassert>
#include <vector>

//Change this to 2 for part 1
const int NUMBER_OF_KNOTS = 10;

class RopeMap{
    public:
        RopeMap() : current(NUMBER_OF_KNOTS,std::make_pair(0,0)), visited(){
        }
        int count_marked(){
            return visited.size();
        }
        void move(char d){
            current.at(0).first += commands.at(d).first;
            current.at(0).second += commands.at(d).second;
            for(int i=1; i<current.size(); i++){
                auto dir = movement_direction(i);
                current[i].first += dir.first;
                current[i].second += dir.second;
            }
            mark_tail();
        }
        static const std::map<char,std::pair<int,int>> commands;
    private:
        std::map<std::pair<int,int>,bool> visited;
        std::vector<std::pair<int,int>> current;
        std::pair<int,int> movement_direction(int i){
            assert(i>0);
            int fdif = current[i-1].first-current[i].first;
            int sdif = current[i-1].second-current[i].second;
            if(fdif*sign(fdif) > 1 || sdif*sign(sdif) > 1){
                return std::make_pair(sign(fdif),sign(sdif));
            }
            else{
                return std::make_pair(0,0);
            }
        }
        void mark_tail(){
            visited[current.back()] = true;
        }
        int sign(int a){
            if(a< 0){
                return -1;
            }
            if(a > 0){
                return 1;
            }
            return 0;
        }
            
};
const std::map<char,std::pair<int,int>> RopeMap::commands = {{{'L',{-1,0}},{'U',{0,1}},{'R',{1,0}},{'D',{0,-1}}}};
int main(){
    std::string nextline;
    auto map = RopeMap();
    while(std::getline(std::cin,nextline)){
        int n = std::stoi(nextline.substr(2));
        for(int i=0; i<n; i++){
            map.move(nextline[0]);
        }
    }
    std::cout<<map.count_marked()<<std::endl;
}
