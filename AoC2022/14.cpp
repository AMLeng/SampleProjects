#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <unordered_map>
#include <vector>

//Since the coordinates are always relatively small and positive, we could have used a vector for a considerable speedup
//Note that the majority of the runtime is spent in checking whether or not things are present in the unordered_map
typedef std::pair<int,int> Point;
namespace std{
    template<>
    struct hash<Point>{
        size_t operator()(const Point &p) const{
            return static_cast<unsigned int>(p.first + 10000*p.second);
        }
    };
}
class Grid{
    public:
        Grid() : unordered_map() , source(500,0), max(source),min(source), sand(0){
            unordered_map[source] = '+';
            }
        void add_rock(std::string line){
            Point next = std::make_pair(std::stoi(line),std::stoi(line.substr(line.find(",") + 1)));
            update_bounds(next);
            line = line.substr(line.find("->"));
            Point current;
            for(int index = line.find("->"); index <line.size(); index = line.find("->")){
                line = line.substr(index + 3);
                current = next;
                next = std::make_pair(std::stoi(line),std::stoi(line.substr(line.find(",") + 1)));
                add_line(current,next);
                update_bounds(next);
            }
        };
        void fill_sand(){
            int i = 0;
            while(add_sand()){
                i++;
            }
            std::cout<<i<<std::endl;
            std::cout<<max.first<<','<<max.second<<std::endl;
            std::cout<<min.first<<','<<min.second<<std::endl;
        }
    private:
        std::unordered_map<Point,char> unordered_map;
        Point source;
        Point max;
        Point min;
        int sand;
        void add_line(Point start, Point end){
            if(start.first == end.first){
                int min = start.second;
                int max = end.second;
                if(start.second > end.second){
                    min = end.second;
                    max = start.second;
                }
                for(int i = min;i<=max; i++){
                    unordered_map[std::make_pair(start.first,i)] = '#';
                }
            }else{
                int min = start.first;
                int max = end.first;
                if(start.first > end.first){
                    min = end.first;
                    max = start.first;
                }
                for(int i = min;i<=max; i++){
                    unordered_map[std::make_pair(i,start.second)] = '#';
                }
            }
        }
        void update_bounds(const Point& p){
            if(p.second > max.second){
                max.second = p.second;
            }
            if(p.first > max.first){
                max.first = p.first;
            }
            if(p.second < min.second){
                min.second = p.second;
            }
            if(p.first < min.first){
                min.first = p.first;
            }
        }
        
        std::vector<Point> next_loc(const Point& p){
            auto possible = std::vector<Point>();
            possible.emplace_back(p.first,p.second+1);
            possible.emplace_back(p.first-1,p.second+1);
            possible.emplace_back(p.first+1,p.second+1);
            return possible;
        }
        bool next_loc(Point& p){
            if(p.second == max.second + 1){
                return false;
            }
            auto q = std::make_pair(p.first,p.second+1);
            if(unordered_map.find(q) == unordered_map.end()){
                p = q;
                return true;
            }
            q.first = q.first - 1;
            if(unordered_map.find(q) == unordered_map.end()){
                p = q;
                return true;
            }
            q.first = q.first + 2;
            if(unordered_map.find(q) == unordered_map.end()){
                p = q;
                return true;
            }
            return false;
        }
        bool in_bounds(const Point& p){
            return p.second <= max.second
                && p.first <= max.first
                && p.first >= min.first
                && p.second >= min.second;
        }
        /*bool add_sand(){ // Part 1
            Point loc = source;
            while(next_loc(loc) && in_bounds(loc)){
            }
            if(in_bounds(loc)){
                unordered_map[loc] = 'o';
                return true;
            }else{
                return false; //Sand fell out of bounds so did not come to rest
            }
        }*/
        bool add_sand(){ //Part 2
            Point loc = source;
            while(next_loc(loc)){
            }
            if(unordered_map[loc] == 'o'){
                return false;
            }else{
                unordered_map[loc] = 'o';
                return true;
            }
        }
};
int main(){
    std::string line;
    auto grid = Grid();
    while(std::getline(std::cin,line)){
        grid.add_rock(line);
    }
    grid.fill_sand();
}
