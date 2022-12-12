#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <deque>
#include <utility>
#include <climits>

std::vector<std::pair<int,int>> adjacent(std::pair<int,int> p, int n, int m){
    auto result = std::vector<std::pair<int,int>>();
    int i = p.first;
    int j = p.second;
    if(i > 0){
        result.emplace_back(i-1,j);
    }
    if(j>0){
        result.emplace_back(i,j-1);
    }
    if(i <n-1){
        result.emplace_back(i+1,j);
    }
    if(j < m-1){
        result.emplace_back(i,j+1);
    }
    return result;
}

int main(){
    std::string nextline;
    auto map = std::vector<std::vector<int>>();
    auto visited = std::vector<std::vector<int>>();
    auto to_try = std::deque<std::pair<std::pair<int,int>,int>>();
    std::pair<int,int> end;
    int i=0;
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        map.emplace_back(std::vector<int>());
        visited.emplace_back(std::vector<int>());
        for(int j=0; j<nextline.size(); j++){
            //if(nextline[j] == 'S' || nextline[j] == 'a'){
            if(nextline[j] == 'S'){
                auto start = std::make_pair(i,j);
                to_try.emplace_back(std::make_pair(start,0));
                nextline[j] = 'a';
            }
            if(nextline[j] == 'E'){
                end = std::make_pair(i,j);
                nextline[j] = 'z';
            }
            map.back().push_back(nextline[j]-'a');
            visited.back().push_back(false);
        }
        i++;
    }
    int min = INT_MAX;
    while(to_try.size() > 0){
        auto p = to_try.front();
        if(p.second > min){
            break;
        }
        to_try.pop_front();
        for(auto a : adjacent(p.first,map.size(),map.front().size())){
            if(!visited[a.first][a.second] && map[a.first][a.second] <= map[p.first.first][p.first.second]+1){
                to_try.push_back(std::make_pair(a,p.second+1));
                visited[a.first][a.second] = true;
                if(a == end && p.second+1 < min){
                    min = p.second+1;
                }
            }
        }
    }
    std::cout<<min<<std::endl;
}
