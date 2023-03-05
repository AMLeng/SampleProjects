#include <array>
#include <iostream>
#include <set>
#include <vector>
#include <cassert>

typedef std::array<int, 3> Point;

struct Cube{
    Cube(std::string line){
        p[0] = std::stoi(line);
        line = line.substr(line.find(",")+1);
        p[1] = std::stoi(line);
        line = line.substr(line.find(",")+1);
        p[2] = std::stoi(line);
    }
    bool operator<(const Cube& rhs) const{
        return p < rhs.p;
    }
    std::vector<Cube> neighbors() const{
        auto neighbors = std::vector<Cube>();
        neighbors.emplace_back(construct_diff(1,0,0));
        neighbors.emplace_back(construct_diff(-1,0,0));
        neighbors.emplace_back(construct_diff(0,0,-1));
        neighbors.emplace_back(construct_diff(0,0,1));
        neighbors.emplace_back(construct_diff(0,-1,0));
        neighbors.emplace_back(construct_diff(0,1,0));
        return neighbors;
    }
    //Here, we use that the input has no negative cubes,
    //And we allow for a buffer of size 1 in every direction
    bool in_bounds(int x, int y, int z) const{
        return p[0] >= -1 && p[0] <= x+1
            && p[1] >= -1 && p[1] <= y+1
            && p[2] >= -1 && p[2] <= z+1;
    }
    Point p;
    private:
    Cube construct_diff(int xdiff, int ydiff, int zdiff) const{
        Cube c = Cube(*this);
        c.p[0] += xdiff;
        c.p[1] += ydiff;
        c.p[2] += zdiff;
        return c;
    }
};
int main(){
    std::string line;
    auto cubes = std::set<Cube>();
    int maxx = 0;
    int maxy = 0;
    int maxz = 0;
    while(std::getline(std::cin,line)){
        auto c = Cube(line);
        if(c.p.at(0) > maxx){
            maxx = c.p.at(0);
        }
        if(c.p.at(1) > maxy){
            maxy = c.p.at(1);
        }
        if(c.p.at(2) > maxz){
            maxz = c.p.at(2);
        }
        cubes.insert(c);
    }
    int faces = 0;

    //Part 2
    auto outside = std::set<Cube>{Cube("0,0,0")};
    auto to_consider = std::vector<Cube>{Cube("0,0,0")};
    while(to_consider.size() > 0){
        auto neighbor_list = to_consider.back().neighbors();
        to_consider.pop_back();
        for(const auto& cube : neighbor_list){
            if(!cube.in_bounds(maxx,maxy,maxz)){
                continue;
            }
            if(cubes.find(cube) != cubes.end()){
                continue;
            }
            //If successfully inserted a new outside cube
            //We need to also consider its neighbors
            if(outside.insert(cube).second){
                to_consider.emplace_back(cube);
            }
        }
    }
    for(const auto& cube : cubes){
        for(const auto& n : cube.neighbors()){
            faces += outside.count(n);
        }
    }

    //Part 1
    /*
    faces += cubes.size()* 6;
    for(const auto& cube : cubes){
        for(const auto& n : cube.neighbors()){
            faces -= cubes.count(n);
        }
    }*/

    std::cout<<faces<<std::endl;

}
