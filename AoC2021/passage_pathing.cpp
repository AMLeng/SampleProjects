#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>


class CaveSystem{
    public:
    CaveSystem() : caves(){}
    void add_connection(std::string connection){
        while(std::isspace(connection.back())){connection.pop_back();}
        auto delim_pos = connection.find('-'); 
        std::string cave_one = connection.substr(0,delim_pos);
        std::string cave_two = connection.substr(delim_pos+1);
        if(caves.find(cave_one)==caves.end()){
            caves.emplace(cave_one, std::make_unique<Cave>(cave_one));
        }
        if(caves.find(cave_two)==caves.end()){
            caves.emplace(cave_two, std::make_unique<Cave>(cave_two));
        }
        Cave::connect(*caves.at(cave_one),*caves[cave_two]);
    }
    int count_paths(bool visit_twice = false){
        int count = 0;
        Path current_path = Path(visit_twice);
        current_path.visited.emplace_back("start");
        while(current_path.visited.size() > 0){
            if(current_path.visited.back() == "end"){
                count++;
            }
            if(!current_path.extend_path(*this)){
                while(!current_path.backtrack(*this)){}
            }
        }
        return count;
    }
    private:
    class Path{
        public:
        Path(bool part2) : visited(), visit_twice(part2), double_visited() {}
        std::vector<std::string> visited;
        bool visit_twice;
        std::string double_visited;
        //returns true if path was successfully extended
        bool extend_path(CaveSystem& system){
            if(visited.back() == "end"){return false;}
            for(std::string s : system.caves.at(visited.back())->connected_caves){
                if(add_if_allowed(system, s)){
                    return true;
                }
            }
            return false;
        }
        bool add_if_allowed(CaveSystem& system, std::string cave_name){
            if(cave_name == "start"){return false;}
            if(system.caves.at(cave_name)->small){
                auto it = std::find(visited.begin(),visited.end(), cave_name);
                if(it == visited.end()){
                    visited.emplace_back(cave_name);
                    return true;
                }else{
                    if(visit_twice && std::find(it+1,visited.end(), cave_name) == visited.end() && double_visited == ""){
                        visited.emplace_back(cave_name);
                        double_visited = cave_name;
                        return true;
                    }else{
                        return false;
                    }
                }
            }else{
                visited.emplace_back(cave_name);
                return true;
            }
        }
        bool backtrack(CaveSystem& system){
            auto former_last = visited.back();
            visited.pop_back();
            if(double_visited == former_last){
                double_visited = "";
            }
            if(visited.size() == 0){return true;}
            auto candidates = system.caves.at(visited.back())->connected_caves;
            auto it = std::find(candidates.begin(),candidates.end(), former_last);
            for(it++;it < candidates.end(); it++){
                if(add_if_allowed(system, *it)){
                    return true;
                }
            }
            return false;
        }
    };
    class Cave{
        public:
            Cave(std::string cave_name) : name(cave_name), small(std::islower(static_cast<unsigned char>(cave_name[0]))),connected_caves(){}
            bool small;
            std::string name;
            std::vector<std::string> connected_caves;
            static void connect(Cave& one, Cave& two){
                if(std::find(one.connected_caves.begin(),one.connected_caves.end(),two.name) == one.connected_caves.end()){
                    one.connected_caves.emplace_back(two.name);
                }
                if(std::find(two.connected_caves.begin(),two.connected_caves.end(),one.name) == two.connected_caves.end()){
                    two.connected_caves.emplace_back(one.name);
                }
            }
    };
    std::map<std::string, std::unique_ptr<Cave>> caves;
};

int main(){
    auto cave_system = CaveSystem();
    std::string nextline;
    while(std::getline(std::cin, nextline)){
        cave_system.add_connection(nextline);
    }
    std::cout<< "Part 1: "<<cave_system.count_paths() <<std::endl;
    std::cout<< "Part 2: "<<cave_system.count_paths(true) << std::endl;
}
