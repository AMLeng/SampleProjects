#include<iostream>
#include<vector>
#include<climits>
#include<string>
#include<cassert>
#include<utility>
#include<queue>

//A tad slow but (in my opinion) not enough to warrant optimizing further
typedef std::pair<int,int> Point;
class ChitonMap{
    public:
    ChitonMap() = delete;
    explicit ChitonMap(std::vector<std::vector<int>> input_map) : 
    risk_map(input_map), rows(input_map.size()), cols(input_map.front().size()){
        for(int i=0; i< rows; i++){
            optimal_map.emplace_back(cols, INT_MAX);
        }
        optimize();
    }
    int optimal_path_length(){
        return optimal_map[rows-1][cols-1];
    }
    private:
    const std::vector<std::vector<int>> risk_map;
    std::vector<std::vector<int>> optimal_map;
    int rows;
    int cols;
    std::vector<Point> get_adjacent(Point p){
        auto adj_list = std::vector<Point>();
        if(p.first > 0){
            adj_list.emplace_back(p.first-1,p.second);
        }
        if(p.second > 0){
            adj_list.emplace_back(p.first,p.second-1);
        }
        if(p.first < rows - 1){
            adj_list.emplace_back(p.first+1,p.second);
        }
        if(p.second  < cols - 1){
            adj_list.emplace_back(p.first,p.second+1);
        }
        return adj_list;
    }
    void optimize(){
        auto to_consider = std::queue<Point>();
        to_consider.push(std::make_pair(0,0));
        optimal_map[0][0] = 0;
        while(to_consider.size() > 0){
            Point p = to_consider.front();
            to_consider.pop();
            auto adj_list = get_adjacent(p);
            for(Point adj : adj_list){
                if(optimal_map[p.first][p.second]+risk_map[adj.first][adj.second] < optimal_map[adj.first][adj.second]){
                    optimal_map[adj.first][adj.second] = optimal_map[p.first][p.second] + risk_map[adj.first][adj.second];
                    to_consider.push(adj);
                }
            }
        }
    }
};
void enlarge_map(std::vector<std::vector<int>>& map){
    for(int i=1, n = map.size(); i<5; i++){
        for(int j=0; j< n; j++){
            auto new_row = std::vector<int>(map.at((i-1)*n+j));
            for(int& i : new_row){
                i = (i%9)+1;
            }
            map.emplace_back(new_row);
        }
    }
    for(auto& row : map){
        int n = row.size();
        for(int i=1; i<5; i++){
            for(int j=0; j<n; j++){
                row.emplace_back((row[(i-1)*n+j]%9)+1);
            }
        }
    }
}

int main(){
    std::string nextline;
    auto input_map = std::vector<std::vector<int>>{};
    while(std::getline(std::cin, nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        input_map.emplace_back();
        for(char c : nextline){
            assert(c >= '0' && c <= '9');
            input_map.back().emplace_back(c-'0');
        }
    }
    //For part 2, enlarge the map
    enlarge_map(input_map);

    auto map = ChitonMap(input_map);
    std::cout<< "The optimal path length is "<< map.optimal_path_length()<<std::endl;
}
