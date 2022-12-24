#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <vector>
#include <map>
#include <set>

typedef std::pair<int,int> Point;
const std::map<char,std::pair<int,int>> directions = {{{'<',{0,-1}},{'^',{-1,0}},{'>',{0,1}},{'v',{1,0}}}};

class Map{
    public:
        Map(std::vector<std::string> lines) : rows(lines.size()), cols(lines.front().size()), time(0), reached_end(false), reached_start(false), reached_end_again(false){
            current = std::set<Point>{{{0,1}}};
            for(int i=0; i< rows; i++){
                for(int j=0; j<cols; j++){
                    if(lines.at(i).at(j) == '<'){
                        left.emplace(std::make_pair(i,j),true);
                    }
                    if(lines.at(i).at(j) == '>'){
                        right.emplace(std::make_pair(i,j),true);
                    }
                    if(lines.at(i).at(j) == '^'){
                        up.emplace(std::make_pair(i,j),true);
                    }
                    if(lines.at(i).at(j) == 'v'){
                        down.emplace(std::make_pair(i,j),true);
                    }
                }
            }
        }
        void update(){
            time++;
            std::set<Point> next;
            for(auto point : current){
                for(auto possible : possibilities(point)){
                    if(!blocked(possible)){
                        if(possible.first == rows-1){
                            if(!reached_start && !reached_end){
                                std::cout<<"Reached end at time "<<time<<std::endl;
                                reached_end = true;
                                next = std::set<Point>{{rows-1,cols-2}};
                                goto LoopEnd;
                            }
                            if(reached_start && reached_end){
                                std::cout<<"Reached end again at time "<<time<<std::endl;
                                reached_end_again = true;
                                next = std::set<Point>{{rows-1,cols-2}};
                                goto LoopEnd;
                            }
                        }
                        if(possible.first == 0){
                            if(!reached_start && reached_end){
                                std::cout<<"Reached start again at time "<<time<<std::endl;
                                reached_start = true;
                                next = std::set<Point>{{0,1}};
                                goto LoopEnd;
                            }
                        }
                        next.emplace(std::move(possible));
                    }else{
                    }
                }
            }
            LoopEnd:
            current = next;
        }
        int t(){
            return time;
        }
        bool done(){
            return reached_end;
        }
        bool double_done(){
            return reached_end_again;
        }
        void print(){
            for(int i=0; i<rows; i++){
                for(int j=0; j<cols; j++){
                    if(blocked(std::make_pair(i,j))){
                        std::cout<<"X";
                    }else{
                        std::cout<<".";
                    }
                }
                std::cout<<std::endl;
            }
            std::cout<<std::endl;
        }
    private:
        int rows;
        int cols;
        int time;
        std::map<Point,bool> left;
        std::map<Point,bool> up;
        std::map<Point,bool> right;
        std::map<Point,bool> down;
        bool reached_end;
        bool reached_start;
        bool reached_end_again;
        std::set<Point> current;
        bool blocked(Point point){
            if(point.first <= 0 && point.second != 1){
                return true;
            }
            if(point.first >= rows-1 && point.second != cols-2){
                return true;
            }
            if(point.second <= 0){
                return true;
            }
            if(point.second >= cols-1){
                return true;
            }
            int left_check = ((point.second + time - 1)%(cols - 2)) + 1;
            int right_check = ((point.second - time + 1)%(cols - 2)) - 1;
            while(right_check <= 0){
                right_check += (cols-2);
            }
            int up_check = ((point.first + time - 1)%(rows - 2)) + 1;
            int down_check = ((point.first - time + 1)%(rows-2)) - 1;
            while(down_check <= 0){
                down_check += (rows-2);
            }
            return left.find(std::make_pair(point.first,left_check)) != left.end()
                || right.find(std::make_pair(point.first,right_check)) != right.end()
                || up.find(std::make_pair(up_check,point.second)) != up.end()
                || down.find(std::make_pair(down_check,point.second)) != down.end();

        }
        std::vector<Point> possibilities(Point current_point){
            auto possible = std::vector<Point>{current_point};
            if(current_point.first == 0){
                assert(current_point.second == 1);
                possible.emplace_back(1,1);
            }else{
                if(current_point.first == rows-1){
                    assert(current_point.second == cols-2);
                    possible.emplace_back(rows-2,cols-2);
                }else{
                for(auto pair : directions){
                    possible.emplace_back(current_point.first + pair.second.first,current_point.second + pair.second.second);
                }
                }
            }
            return possible;
        }
};
int main(){
    std::string line;

    std::vector<std::string> lines;
    while(std::getline(std::cin,line)){
        if(line.back() == '\r'){line.pop_back();}
        lines.push_back(line);
    }
    auto map = Map(lines);

    //Part 1
    while(!map.done()){
        map.update();
    }
    //Part 2
    while(!map.double_done()){
        map.update();
    }
}
