#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <cassert>

const static int WIDTH = 7;
typedef std::array<bool, WIDTH> Row;
typedef std::pair<int,int> Point;

struct Board{
    Board() : height(0){
        rows = std::vector<Row>(WIDTH,empty_row());
    }
    //the coordinates in points are relative (vertical,horizontal)
    //with vertical = 0 at vertical coordinate equal to board height
    bool invalid(const std::vector<Point>& points) const{
        for(const auto& p : points){
            if(p.second < 0 || p.second >= WIDTH || height + p.first < 0){
                return true;
            }
            if(rows.at(height+p.first).at(p.second) == true){
                return true;
            }
        }
        return false;
    }
    long long get_height() const{
        return height;
    }
        void insert(const std::vector<Point>& points){
            assert(!invalid(points));
            long long max_vert = -height;
            for(const auto& p : points){
                rows.at(height + p.first).at(p.second) = true;
                if(p.first + 1 > max_vert){
                    max_vert = p.first + 1;
                }
            }
            if(max_vert > 0){
                height += max_vert;
                for(long long i=0; i<max_vert; i++){
                    rows.emplace_back(empty_row());
                }
            }
        }
    private:
        std::vector<Row> rows;
        long long height;
        constexpr Row empty_row(){
            auto row = std::array<bool, WIDTH>();
            for(int i=0; i<WIDTH; i++){
                row[i] = false;
            }
            return row;
        }
};

struct Shape{
    explicit Shape(int i){
        switch(i){
            case 0:
                //####
                points = std::vector<Point>{{
                    {3,2},{3,3},{3,4},{3,5}
                }};
                break;
            case 1:
                /* .#.
                 * ###
                 * .#.   */
                points = std::vector<Point>{{
                    {4,2},{3,3},{4,3},{4,4}, {5,3}
                }};
                break;
            case 2:
                /* ..#
                 * ..#
                 * ###  */
                points = std::vector<Point>{{
                    {3,2},{3,3},{3,4},{4,4}, {5,4}
                }};
                break;
            case 3:
                /* #
                 * #
                 * #
                 * #  */
                points = std::vector<Point>{{
                    {3,2},{4,2},{5,2},{6,2}
                }};
                break;
            case 4:
                /* ##
                 * ## */
                points = std::vector<Point>{{
                    {3,2},{3,3},{4,2},{4,3}
                }};
                break;
            default:
                assert(false);
        }
    }

    bool fall(const Board& b){
        auto possible_next = new_shape(-1, 0);
        if(!b.invalid(possible_next)){
            points = possible_next;
            return true;
        }
        return false;
    }
    void shift(char c, const Board& b){
        assert(c == '>' || c == '<');
        int offset = 0;
        if(c == '>'){
            offset = 1;
        }
        if(c == '<'){
            offset = -1;
        }
        auto possible_next = new_shape(0, offset);
        if(!b.invalid(possible_next)){
            points = possible_next;
        }
    }
    std::vector<Point> get_points() const{
        return points;
    }
    void print(){
        for(auto p : points){
            std::cout<<p.first<<", "<<p.second<<std::endl;
        }
        std::cout<<std::endl;
    }
    private:
        std::vector<Point> points;
        std::vector<Point> new_shape(int vertical_movement, int horizontal_movement){
            auto new_points = std::vector<Point>();
            for(const auto& p : points){
                new_points.emplace_back(p.first + vertical_movement,p.second + horizontal_movement);
            }
            return new_points;
        }
};

auto add_shape(const std::string& line, int& time, Board& b, long long& shape_count){
    auto s = Shape(shape_count % 5);
    do{
        char c = line.at(time % line.length());
        time++;
        s.shift(c,b);
    }while(s.fall(b));
    //code to figure out reoccuring pattern with heights
    /*if(time % line.length() == 0){
      std::cout<<"Count diff: "<<shape_count-past_count<<std::endl;
      past_count = shape_count;
      std::cout<<"Height diff: "<<b.get_height() - past_height<<std::endl;
      past_height = b.get_height();
      }else{
      std::cout<<"time mod len: "<<time % line.length()<<std::endl;
      }*/
    b.insert(s.get_points());
    shape_count++;
    return s.get_points();
}

int main(){
    std::string line;
    std::getline(std::cin,line);
    if(line.back() == '\r'){
        line.pop_back();
    }
    auto b = Board();
    long long shape_count=0;
    int time = 0;
    //Part 1
    //long long THRESHOLD = 2022L;
    //Part 2
    long long THRESHOLD = 1000000000000L;
    
    //Code to figure out reoccuring pattern with heights
    long long INITIAL = 10000;

    if(THRESHOLD > INITIAL*10){
        //Part 2
        //An initial segment to ignore any weird/irregular behavior at the start
        long long computed_height = 0;
        for(int i=0; i< INITIAL; i++){
            add_shape(line, time, b, shape_count);
        }

        int index = 0;
        auto seen = std::vector<std::vector<Point>>{};
        long long start_height = b.get_height();
        long long start_shapes = shape_count;
        seen.emplace_back(add_shape(line, time, b, shape_count));

        //We wait until we start seeing shapes landing the exactly the same positions as before
        //And assume that once this starts happening, everything will repeat
        while(index*2 < seen.size()){
            auto temp = add_shape(line, time, b, shape_count);
            seen.emplace_back(temp);
            if(seen.at(index) == temp){
                index++;
            }else{
                index = 0;
            }
        }
        long long height_diff = b.get_height() - start_height;
        long long shape_diff = shape_count - start_shapes;

        long long number_of_loops = (THRESHOLD - shape_count) / shape_diff;
        long long remainder = (THRESHOLD - shape_count) % shape_diff;
        for(int i=0; i< remainder; i++){
            add_shape(line, time, b, shape_count);
        }
        std::cout<<b.get_height() + static_cast<long long>(number_of_loops*height_diff)<<std::endl;
    }else{
        //Part 1
        for(int i=0; i< THRESHOLD; i++){
            add_shape(line, time, b, shape_count);
        }
        std::cout<<b.get_height()<<std::endl;
    }
}
