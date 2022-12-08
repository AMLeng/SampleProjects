#include <iostream>
#include <string>
#include<vector>
#include <utility>
#include <cassert>

class Forest{
    public:
        Forest(std::vector<std::vector<int>> h) : heights(h), n(h.size()), m(h.front().size()){}
        int count_visible(){
            int sum = 0;
            for(int i=0; i<n; i++){
                for(int j=0; j<m; j++){
                    if(check_visible(i,j)){
                        sum++;
                    }
                }
            }
            return sum;
        }
        int best_score(){
            int max = 0;
            for(int i=0; i<n; i++){
                for(int j=0; j<m; j++){
                    int temp = score(i,j);
                    if(temp > max){
                        max = temp;
                    }
                }
            }
            return max;
        }
    
        static const std::vector<std::pair<int,int>> directions;
    private:
        std::vector<std::vector<int>> heights;
        int n;
        int m;
        bool in_bounds(int a, int b){
            return a >= 0 && a < n && b >= 0 && b < m;
        }
        bool check_visible(int a, int b){
            for(auto d : directions){
                bool this_direction = true;
                int i = a+d.first;
                int j = b+d.second;
                while(in_bounds(i,j)){
                    if(heights.at(i).at(j)>= heights.at(a).at(b)){
                        this_direction = false;
                    }
                    i+= d.first;
                    j+= d.second;
                }
                if(this_direction){return true;}
            }
            return false;
        }
        int score(int a, int b){
            int prod = 1;
            for(auto d : directions){
                int i = a+d.first;
                int j = b+d.second;
                int visible_this_direction = 0;
                while(in_bounds(i,j)){
                    visible_this_direction++;
                    if(heights.at(i).at(j)>= heights.at(a).at(b)){
                        break;
                    }
                    i+= d.first;
                    j+= d.second;
                }
                prod *= visible_this_direction;
            }
            return prod;
        }
};
const std::vector<std::pair<int,int>> Forest::directions = 
    std::vector<std::pair<int,int>>{std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};

int main(){
    std::string nextline;
    auto trees = std::vector<std::vector<int>>();
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){
            nextline.pop_back();
        }
        trees.emplace_back(std::vector<int>());
        for(char c : nextline){
            trees.back().push_back(c-'0');
        }
    }
    auto myforest = Forest(trees);
    std::cout<<myforest.count_visible()<<std::endl;
    std::cout<<myforest.best_score()<<std::endl;
}
