#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cassert>


int abs(int x){
    if (x>=0){
        return x;
    }else{
        return -x;
    }
}
int dist(int x1, int y1, int x2, int y2){
    return abs(x1-x2)+abs(y1-y2);
}

typedef std::pair<int,int> Range;

class Sensor{
    public:
        Sensor(std::string line){
            x = std::stoi(line.substr(line.find("x=")+2));
            y = std::stoi(line.substr(line.find("y=")+2));
            line = line.substr(line.find(":"));
            bx = std::stoi(line.substr(line.find("x=")+2));
            by = std::stoi(line.substr(line.find("y=")+2));
            r = dist(x,y,bx,by);
        }
        bool covers(int a, int b){
            return dist(x,y,a,b) <= r;
        }
        Range get_range(int y_coord){
            int remainder = r - abs(y_coord-y);
            if(remainder < 0){
                return std::make_pair(0,-1);
            }else{
                return std::make_pair(x-remainder,x+remainder);
            }
        }

        int r;
    private:
        int x;
        int y;
        int bx;
        int by;
};

class Ranges{
    public:
        Ranges() : ranges() , upper(0){}
        Ranges(int bound) : ranges() , upper(bound){}
        void add(Range r){
            if(upper != 0){
                if(r.first < 0){
                    r.first = 0;
                }
                if(r.second > upper){
                    r.second = upper;
                }
            }
            if(r.first > r.second){return;}
            for(auto& x : ranges){
                resolve_intersection(x,r);
                if(r.first > r.second){return;}
            }
            ranges.push_back(r);
        }
        int count(){
            int sum = 0;
            for(auto& x : ranges){
                sum += x.second-x.first + 1;
            }
            return sum;
        }
        void print(){
            for(auto& x : ranges){
                std::cout<<x.first<<','<<x.second<<"; ";
            }
            std::cout<<std::endl;
        }
    private:
        const int upper;
        std::vector<Range> ranges;
        void resolve_intersection(const Range x, Range& y){
            assert(x.first <= x.second);
            if(y.first > x.second || y.second < x.first){
                return;
            }
            if(y.first >=x.first && y.second <= x.second){
                y = std::make_pair(0,-1);
                return;
            }
            if(y.first <=x.first && y.second >= x.second){
                add(std::make_pair(y.first, x.first-1));
                y.first = x.second + 1;
                return;
            }
            if(y.first <= x.first && y.second < x.second){
                y.second = x.first-1;
                return;
            }
            if(y.first > x.first && y.second >= x.second){
                y.first = x.second+1;
                return;
            }
        }
};

int main(){
    const int YVAL = 2000000;
    const int BEACONS_IN_ROW = 1;
    const int MAX = 4000000;
    //const int MAX = 20;
    std::string line;
    auto sensors = std::vector<Sensor>();
    while(std::getline(std::cin,line)){
        sensors.emplace_back(line);
    }
    int j=0;
    for(; j<MAX; j++){
        auto ranges = Ranges(MAX);
        for(auto s : sensors){
            ranges.add(s.get_range(j));
        }
        if(ranges.count() < MAX + 1){
            assert(ranges.count() == MAX);
            break;
        }
    }
    std::cout<<"Row "<<j<<std::endl;
    /*auto ranges = Ranges();
    for(auto s : sensors){
        auto t = s.get_range(YVAL);
        if(t.first <= t.second){
            ranges.add(s.get_range(YVAL));
        }
    }
    std::cout<<ranges.count()- BEACONS_IN_ROW<<std::endl;
    ranges.print();*/

    bool seen;
    for(int i=0; i< MAX; i++){
        seen = true;
        for(auto s : sensors){
            if(s.covers(i,j)){
                seen = false;
                break;
            }
        }
        if(seen){
            std::cout<<i<<','<<j<<std::endl;
            std::cout<<static_cast<long long>(i)*4000000+j<<std::endl;
            return 0;
        }
    }
}
