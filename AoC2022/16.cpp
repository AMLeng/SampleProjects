#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <utility>
#include <map>
#include <set>
#include <cassert>

class State; 

class Caves{
    friend State;
    public:
        Caves(const std::vector<std::string>& lines) : max_rate(0){
            for(auto line : lines){
                add(line);
            }
            auto starts = std::vector<std::string>(positive_rate);
            starts.emplace_back("AA");
            //Compute minimum distances from start
            for(auto start : starts){
                auto q = std::queue<std::pair<std::string,int>>();
                q.emplace(start,0);
                while(q.size() > 0){
                    auto end_cave = q.front();
                    q.pop();
                    min_dist.emplace(std::make_pair(start,end_cave.first),end_cave.second);
                    for(auto x : connected.at(end_cave.first)){
                        if(min_dist.find(std::make_pair(start,x)) == min_dist.end()){
                            //If already in the queue this will do nothing
                            //So we don't need to check if it's already in the queue
                            q.emplace(x,end_cave.second + 1);
                        }
                    }
                    
                }
            }
            for(auto c : positive_rate){
                max_rate += rates.at(c);
            }
        }
        void print(){
            for(auto pair : connected){
                std::string name = pair.first;
                std::cout<<"Added "<<name<<" connected to ";
                  for(auto c : connected.at(name)){
                  std::cout<<c<<", ";
                  }
                  std::cout<<std::endl;
            }
        }
    private:
        std::vector<std::string> positive_rate;
        std::map<std::string,int> rates;
        std::map<std::string,std::vector<std::string>> connected;
        std::map<std::pair<std::string,std::string>,int> min_dist;
        int max_rate;

        void add(std::string line){
            std::string name = line.substr(6,2);
            int rate = std::stoi(line.substr(line.find("rate=")+5));
            auto connected_to = std::vector<std::string>();
            if(line.find("valves") == std::string::npos){
                line = line.substr(line.find("to valve")+9);
                connected_to.emplace_back(line.substr(0,2));
            }else{
                line = line.substr(line.find("valves")+7);
                connected_to.emplace_back(line.substr(0,2));
                while(line.find(",")<line.size()){
                    line = line.substr(line.find(",")+2);
                    connected_to.emplace_back(line.substr(0,2));
                }
            }
            rates.emplace(name,rate);
            if(rate > 0){
                positive_rate.emplace_back(name);
            }
            connected.emplace(name,connected_to);
        }
};

class State{
    public:
        // Part 1
        //const static int total_time = 30;
        //constexpr static int actors = 1;
        //Part 2
        const static int total_time = 26;
        constexpr static int actors = 2;
        State(const Caves* c) : time(0), flow(0), rate(0), caves(c), open(){
            current.fill("AA");
            dist_to_current.fill(0);
            for(auto name : c->positive_rate){
                open.emplace(name,false);
            }
        }
        State(const State& former,std::string next_cave, int index_to_update) : State(former){
            //assert(open.at(next_cave) == false);
            assert(time <= total_time);
            assert(dist_to_current.at(index_to_update) == 0);

            //Setup
            //The +1 is for the time to open the valve
            dist_to_current.at(index_to_update) = caves->min_dist.at(std::make_pair(current.at(index_to_update),next_cave)) + 1; 
            current.at(index_to_update) = next_cave;
        }
        std::vector<State> next_states(){
            auto states = std::vector<State>{*this};
            for(int i=0; i<actors; i++){
                if(dist_to_current.at(i) != 0){
                    continue;
                }
                auto next_states = std::vector<State>();
                auto next_caves = std::vector<std::string>();
                for(const auto& s : states){
                    bool added = false;
                    for(auto pair : open){
                        if(s.valid_next(pair.first, i)){
                            added = true;
                            next_states.emplace_back(s,pair.first, i);
                        }
                    }
                    if(!added){
                        //Add in an arbitrary next cave to move to
                        //Just to make sure that we actually continue moving up until total_time
                        //So that other actors can continue to move and open caves
                        next_states.emplace_back(s,open.begin()->first, i);
                    }
                }
                states = next_states;
            }
            for(auto& s : states){
                s.step_forward();
            }
            return states;
        }
        void print() const{
            std::cout<<"Distance ";
            for(int i=0; i<actors; i++){
                std::cout<<dist_to_current.at(i)<<" from "<<current.at(i)<<", ";
            }
            std::cout<<" at time "<<time<<" with total flow >= "<<get_total_flow()<<" and max potential <= "<<get_total_flow()+max_additional()<<std::endl;
        }
        int get_time() const{
            return time;
        }

        int get_total_flow() const{
            return flow + (total_time-time)*rate;
        }
        int max_additional() const{
            int extra_flow = 0;
            if(time >= total_time){return extra_flow;}
            if(time < total_time - 1){
                for(const auto& map_pair : open){
                    if(map_pair.second == false){
                        extra_flow += caves->rates.at(map_pair.first);
                    }
                }
                extra_flow *= (total_time - 2 - time);
            }else{
                extra_flow = 0;
            }
            //extra_flow *= (total_time - 1 - time);
            for(int i=0; i<actors; i++){
                //dist_to_current is 1 more than physical distance
                //to account for time to open
                if(dist_to_current.at(i) <=1 && open.at(current.at(i)) == false){
                    extra_flow += caves->rates.at(current.at(i));
                }
            }
            return extra_flow;
        }
    private:
        int time;
        int flow;
        int rate;
        const Caves* caves;
        std::map<std::string,bool> open;
        std::array<std::string, actors> current;
        std::array<int, actors> dist_to_current;

        void step_forward(){
            int t = dist_to_current.at(0);
            for(int i=1; i< actors; i++){
                if(dist_to_current.at(i) < t){
                    t = dist_to_current.at(i);
                }
            }

            //Move forward in time
            assert(t > 0);
            if(time + t > total_time){
                t = total_time - time;
            }
            assert(time + t <= total_time);
            for(int i=0; i< actors; i++){
                dist_to_current.at(i) -= t;
            }
            flow += t*rate;
            time += t;
            for(int i=0; i< actors; i++){
                if(dist_to_current.at(i) == 0 && open.at(current.at(i)) == false){
                    open.at(current.at(i)) = true;
                    rate += caves->rates.at(current.at(i));
                }
            }
        }
        bool valid_next(std::string next_cave, int index) const{
            if(open.at(next_cave)){
                return false;
            }
            //The +1 is for the time to open the valve
            int dist_to_next = caves->min_dist.at(std::make_pair(current.at(index),next_cave)) + 1;
            if(time + dist_to_next > total_time){
                return false;
            }
            for(int i=0; i<actors; i++){
                if(i == index){
                    continue;
                }
                //dist_to_current already includes the time to open the valve current.at(i)
                if(next_cave == current.at(i) &&
                    dist_to_current.at(i) < dist_to_next){
                    return false;
                }
            }
            return true;
        }
};

int main(){
    std::string line;
    auto lines = std::vector<std::string>();
    while(std::getline(std::cin,line)){
        lines.emplace_back(line);
    }
    auto caves = std::make_unique<const Caves>(lines);

    auto initial_state = State(caves.get());
    auto states = std::vector<State>{initial_state};
    int max = 0;
    while(states.size() > 0){
        auto next = states.back().next_states();
        states.pop_back();
        for(auto& s : next){
            if(s.get_total_flow() + s.max_additional() > max && s.get_time() < State::total_time){
                if(max < s.get_total_flow()){
                    max = s.get_total_flow();
                    s.print();
                }
                states.push_back(std::move(s));
            }
        }
    }
    std::cout<<max<<std::endl;
}
