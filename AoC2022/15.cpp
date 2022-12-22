#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <cassert>
#include <map>
#include <iterator>

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

struct Sensor{
    Sensor(std::string line){
        x = std::stoi(line.substr(line.find("x=")+2));
        y = std::stoi(line.substr(line.find("y=")+2));
        line = line.substr(line.find(":"));
        int bx = std::stoi(line.substr(line.find("x=")+2));
        int by = std::stoi(line.substr(line.find("y=")+2));
        r = dist(x,y,bx,by);
    }
    int r;
    int x;
    int y;
};
int count_nobeacon(const std::vector<Sensor>& sensors, const int linenum){
    auto intervals = std::map<int,int>();

    for(const auto& s : sensors){
        int remainder = s.r - abs(linenum-s.y);
        if(remainder < 0){
            continue;
        }
        auto r = std::make_pair(s.x-remainder,s.x+remainder);
        auto ret = intervals.insert(r);
        if(ret.second == false){
            if(intervals.at(r.first)< r.second){
                intervals.at(r.first)= r.second;
            }
        }
    }
    {
        auto it = intervals.begin();
        auto next = std::next(it,1);
        for(; next != intervals.end() && it != intervals.end(); it++, next = std::next(it,1)){
            assert(it->second>= it->first);
            while(next != intervals.end() && next->second <= it->second){
                next = intervals.erase(next);
            }
            if(next != intervals.end() && next->first <= it->second){
                it->second = next->first - 1;
            }
        }
    }
    int sum = 0;
    for(auto i : intervals){
        sum += i.second - i.first + 1;
    }
    return sum;
}

long long check_covered(const std::vector<Sensor>& sensors, int& linenum, int bound){
    auto intervals = std::map<int,std::pair<int,int>>();
    long long leewayleft = 0;
    long long leeway = 0;
    for(const auto& s : sensors){
        int remainder = s.r - abs(linenum-s.y);
        if(remainder < 0){
            continue;
        }
        auto r = std::make_pair(s.x-remainder,std::make_pair(s.x+remainder,s.y));
        long long x = r.second.first - bound;
        if(x > 0){
            r.second.first = bound;
            if(x > leeway){
                leeway= x;
            }
        }
        if(r.first < 0){
            if(-r.first > leewayleft){
                leewayleft = -r.first;
            }
            r.first = 0;
        }
        auto ret = intervals.insert(r);
        if(ret.second == false){
            if(intervals.at(r.first).first < r.second.first){
                intervals.at(r.first).first = r.second.first;
            }
        }
    }
    if(leeway > leewayleft){
        leeway = leewayleft;
        assert(leeway >= 0);
    } 
    {
        auto it = intervals.begin();
        auto next = std::next(it,1);
        for(; next != intervals.end() && it != intervals.end(); it = next, next = std::next(it,1)){
            assert(it->second.first >= it->first);
            while(next != intervals.end() && (next->second.first <= it->second.first)){
                next = intervals.erase(next);
            }
            if(next != intervals.end()){
                long long overlap = (it->second.first - next->first)/2;
                if(overlap < 0){
                    long long i=0;
                    for(auto r : intervals){
                        if(i>= r.first && i < r.second.first){
                            i = r.second.first+1;
                        }
                    }
                    return i*4000000+linenum;
                }
                long long forward = next->second.second - linenum;
                {
                    long long temp = it->second.second - linenum;
                    if((temp > 0 && temp  < forward) || forward < 0){
                        forward = temp;
                    }
                }
                if(forward > overlap){
                    overlap = forward;
                }
                if(overlap < leeway){
                    leeway = overlap;
                }
            }
        }
    }
    assert(leeway >= 0);
    linenum += leeway;
    return 0;
}
int main(){
    std::string line;
    auto sensors = std::vector<Sensor>();
    while(std::getline(std::cin,line)){
        sensors.emplace_back(line);
    }

    //Part 1
    const int YVAL = 2000000;
    const int BEACONS_IN_ROW = 1;
    std::cout<<"Part 1: "<<count_nobeacon(sensors,YVAL)-BEACONS_IN_ROW<<std::endl;

    const int MAX = 4000000;
    int j=0;
    for(; j<MAX; j++){
        long long x = check_covered(sensors,j,MAX);
        if(x != 0){
            std::cout<<"Part 2: "<<x<<std::endl;
            break;
        }
    }

}


