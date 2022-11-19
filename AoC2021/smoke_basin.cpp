#include <vector>
#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>



typedef std::pair<int,int> Point;
class HeightMap{
    public:
        HeightMap(std::vector<std::vector<int>> input_map) : map(input_map), rows(input_map.size()), cols(input_map.front().size()) {}

        int risk_factor(const std::vector<Point>& low_points){
            int risk_sum = 0;
            for(Point p : low_points){
                risk_sum += map[p.first][p.second]+1;
            }
            return risk_sum;
        }

        int basin_size(Point point){
            auto points = std::vector<Point>{};
            points.emplace_back(point);
            int basin_size = 0;
            for(int i=0; i<points.size(); i++){
                Point p = points[i];
                auto adj_points = adjacent_points(p);
                for(Point adj : adj_points){
                    if(map[adj.first][adj.second] == 9){continue;}
                    if(map[adj.first][adj.second] > map[p.first][p.second] && std::find(points.begin(),points.end(),adj) == points.end()){
                        points.emplace_back(adj);
                    }
                }
            }
            return points.size();
        }

        std::vector<Point> find_low_points(){
            auto points = std::vector<Point>();
            for(int i=0; i<rows; i++){
                for(int j=0; j<cols; j++){
                    auto p = Point(i,j);
                    if(check_low_point(p)){
                        points.emplace_back(p);
                    }
                }
            }
            return points;
        }
    private:
        int rows;
        int cols;
        std::vector<std::vector<int>> map;
        std::vector<Point> adjacent_points(Point p){
            auto points = std::vector<Point>{};
            if(p.first > 0){points.emplace_back(p.first-1,p.second);}
            if(p.second > 0){points.emplace_back(p.first,p.second-1);}
            if(p.first < rows-1){points.emplace_back(p.first+1,p.second);}
            if(p.second < cols-1){points.emplace_back(p.first,p.second+1);}
            return points;
        }
        bool check_low_point(Point p){
            auto points = adjacent_points(p);
            bool result = true;
            for(Point adj : points){
                result &= map[p.first][p.second] < map[adj.first][adj.second];
            }
            return result;
        }
};


int main(){
    //Read Input
    std::string newline;
    std::getline(std::cin, newline);
    if(newline.back() == '\r'){newline.pop_back();}
    int n = newline.size();
    auto input_map = std::vector<std::vector<int>>{};
    do{
        if(newline.back() == '\r'){newline.pop_back();}
        input_map.emplace_back(std::vector<int>(n,9));
        for(int i=0; i<n;i++){
            if(std::isdigit(newline[i])){
                input_map.back()[i] = newline[i] - '0';
            }
        }
    }while(std::getline(std::cin, newline));
    auto map = HeightMap(input_map);

    //Part 1
    auto low_points = map.find_low_points();
    std::cout << "Part 1:" <<map.risk_factor(low_points) <<std::endl;

    //Part 2
    auto basin_sizes = std::vector<int>{};
    for(Point p : low_points){
        basin_sizes.emplace_back(map.basin_size(p));
    }
    std::sort(basin_sizes.begin(),basin_sizes.end(),[](int a,int b){return a>b;});
    std::cout << "Part 2:"<<basin_sizes[0]*basin_sizes[1]*basin_sizes[2]<<std::endl;
}
