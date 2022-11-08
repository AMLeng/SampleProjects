#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>

namespace hydrothermal_venture{
    class Line{
        public:
            Line(int x1,int y1, int x2, int y2) : p1(x1,y1),p2(x2,y2){
                }
            //Assuming a string is of the form "x1,y1 -> x2,y2", turns it into the corresponding line
            Line(std::string s){
                p1.first = std::stoi(s);
                p1.second =std::stoi(s.substr(s.find(",")+1));
                s = s.substr(s.find("->")+2);
                p2.first = std::stoi(s);
                p2.second = std::stoi(s.substr(s.find(",")+1));
            }
            bool contains(int x,int y) const{
                if(x> p1.first && x>p2.first ||
                   x<p1.first && x <p2.first ||
                   y>p1.second && y>p2.second||
                   y<p1.second && y<p2.second){return false;}
                auto slope1 = std::pair<int,int>(x-p1.first,y-p1.second);
                auto slope2 = std::pair<int,int>(x-p2.first,y-p2.second);
                return slope1.first*slope2.second == slope2.first*slope1.second;
            }
            static void only_horizontal_vertical(std::vector<Line>& lines){
                lines.erase(std::remove_if(lines.begin(),lines.end(),[](Line l){
                    return l.p1.first != l.p2.first && l.p1.second != l.p2.second;
                }), lines.end());
            }
            static std::pair<int,int> max_coords(const std::vector<Line>& lines){
                auto max = std::pair<int,int>(0,0);
                for(Line l : lines){
                    max.first = std::max(max.first,l.p1.first);
                    max.first = std::max(max.first,l.p2.first);
                    max.second = std::max(max.second,l.p1.second);
                    max.second = std::max(max.second,l.p2.second);
                }
                return max;
            }

        private:
            std::pair<int,int> p1;
            std::pair<int,int> p2;
    }; //class Line

    int count_grid(std::vector<std::vector<int>> grid, int cutoff){
        int count = 0;
        for(int i=0; i<grid.size();i++){
            for(int j=0; j<grid[i].size();j++){
                if(grid[i][j]>=cutoff){count++;}
            }
        }
        return count;
    }

    int hydrothermal_venture_one(std::vector<Line>& lines){
        Line::only_horizontal_vertical(lines);
        std::pair<int,int> max_coords = Line::max_coords(lines);
        auto grid = std::vector<std::vector<int>>(max_coords.first+1,std::vector<int>(max_coords.second+1));
        for(int i=0; i<=max_coords.first; i++){
            for(int j=0; j<=max_coords.second; j++){
                for(Line l : lines){
                    if(l.contains(i,j)){
                        grid[i][j]++;
                    }
                }
            }
        }
        return count_grid(grid, 2);
    }
    int hydrothermal_venture_two(std::vector<Line>& lines){
        std::pair<int,int> max_coords = Line::max_coords(lines);
        auto grid = std::vector<std::vector<int>>(max_coords.first+1,std::vector<int>(max_coords.second+1));
        for(int i=0; i<=max_coords.first; i++){
            for(int j=0; j<=max_coords.second; j++){
                for(Line l : lines){
                    if(l.contains(i,j)){
                        grid[i][j]++;
                    }
                }
            }
        }
        return count_grid(grid, 2);
    }
}//namespace hydrothermal_venture


int main(){
    //Read in input
    std::vector<hydrothermal_venture::Line> lines = {};
    std::string next_line;
    while(std::getline(std::cin, next_line)){
        lines.emplace_back(next_line);
    }
    //int result = hydrothermal_venture_one(lines);
    int result = hydrothermal_venture_two(lines);
    std::cout << result;
}
