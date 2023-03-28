#include <iostream>
#include <string>
#include <cassert>
#include <array>
#include <vector>

static int LIMIT = 24;
static int MAX_ORE_BOTS = 5;

struct Blueprint{
    int number;
    int ore_cost;
    int clay_cost;
    std::pair<int,int> obsidian_cost;
    std::pair<int,int> geode_cost;

    Blueprint(std::string line) {
        line = line.substr(line.find("Blueprint ")+10);
        number = std::stoi(line);
        line = line.substr(line.find("costs")+5);
        ore_cost = std::stoi(line);
        line = line.substr(line.find("costs")+5);
        clay_cost = std::stoi(line);
        line = line.substr(line.find("costs")+5);
        int temp = std::stoi(line);
        line = line.substr(line.find("and")+3);
        obsidian_cost = std::make_pair(temp,std::stoi(line));
        line = line.substr(line.find("costs")+5);
        temp = std::stoi(line);
        line = line.substr(line.find("and")+3);
        geode_cost = std::make_pair(temp,std::stoi(line));
    }
    bool can_produce(const std::array<int, 4>& current_resources, int bot_index) const{
        switch(bot_index){
            case 0:
                if(current_resources[0] >= ore_cost){
                    return true;
                }
                break;
            case 1:
                if(current_resources[0] >= clay_cost){
                    return true;
                }
                break;
            case 2:
                if(current_resources[1] >= obsidian_cost.second && current_resources[0] >= obsidian_cost.first){
                    return true;
                }
                break;
            case 3:
                if(current_resources[2] >= geode_cost.second && current_resources[0] >= geode_cost.first){
                    return true;
                }
                break;
            default:
                assert(false);
        }
        return false;
    }
    //The effect of creating "amount" of bots of type bot_index
    void modify(std::array<int, 4>& current_resources, int bot_index, int amount) const{
        switch(bot_index){
            case 0:
                current_resources[0] -= ore_cost * amount;
                assert(current_resources[0] >= 0);
                return;
            case 1:
                current_resources[0] -= clay_cost * amount;
                assert(current_resources[0] >= 0);
                return;
            case 2:
                current_resources[0] -= obsidian_cost.first * amount;
                current_resources[1] -= obsidian_cost.second * amount;
                assert(current_resources[0] >= 0);
                assert(current_resources[1] >= 0);
                return;
            case 3:
                current_resources[0] -= geode_cost.first * amount;
                current_resources[2] -= geode_cost.second * amount;
                assert(current_resources[0] >= 0);
                assert(current_resources[2] >= 0);
                return;
            default:
                assert(false);
        }
        return;
    }
};
struct State{
    int time;
    std::array<int, 4> resources;
    std::array<int, 4> bots;
    const Blueprint* b;
    std::array<bool, 4> could_have_built;

    State(const Blueprint* b) : b(b), time(0){
        resources = std::array<int, 4>{0,0,0,0};
        bots = std::array<int, 4>{1,0,0,0};
        could_have_built = std::array<bool, 4>{false, false, false, false};
    }
    State(const State& s) = default;

    void print() const{
        std::cout<<"At time "<<time<<std::endl;;
        std::cout<<"Resources: ";
        for(int i=0; i<4; i++){
            std::cout<<resources.at(i)<<", ";
        }
        std::cout<<std::endl<<"Bots: ";
        for(int i=0; i<4; i++){
            std::cout<<bots.at(i)<<", ";
        }
        std::cout<<std::endl<<std::endl;
    }
    void add(int bot_type){
        b->modify(resources, bot_type, 1);
        bots[bot_type]++;
    }

    State advance_time(std::array<bool, 4> b = std::array<bool, 4>{false, false, false, false}) const{
        auto s = State(*this);
        s.time++;
        for(int bot_type = 0; bot_type < 4; bot_type++){
            s.resources.at(bot_type) += bots.at(bot_type);
        }
        s.could_have_built = b;
        return s;
    }
    int geodes() const{
        return resources.at(3);
    }

    std::vector<State> next_states() const{
        auto next = std::vector<State>();
        auto do_not_build = this->could_have_built;
        if(time < LIMIT){
            for(int bot_type = 1; bot_type < 4; bot_type++){
                auto s = advance_time();
                if(!do_not_build.at(bot_type) && b->can_produce(resources, bot_type)){
                    s.add(bot_type);
                    do_not_build.at(bot_type) = true;
                    next.push_back(s);
                }
            }
            if(bots[0] < MAX_ORE_BOTS){
                int bot_type = 0;
                auto s = advance_time();
                if(!do_not_build.at(bot_type) && b->can_produce(resources, bot_type)){
                    s.add(bot_type);
                    do_not_build.at(bot_type) = true;
                    next.push_back(s);
                }
            }
            next.push_back(advance_time(do_not_build));
        }
        return next;
    }

    int geode_upper_bound() const{
        int time_rem = LIMIT - time;
        return resources[3] + time_rem*(time_rem/2 + bots[3]);
    }

};

int calculate(const Blueprint& b){
    int max_geodes = 0;
    auto start_state = State(&b);
    auto states = std::vector<State>{start_state};
    while(states.size() > 0){
        auto s = states.back();
        states.pop_back();
        if(s.time == LIMIT){
            //s.print();
            if(s.geodes() > max_geodes){
                max_geodes = s.geodes();
                //std::cout<<"New max: "<<max_geodes<<std::endl;
                continue;
            }
        }
        for(auto& next : s.next_states()){
            if(next.geode_upper_bound() > max_geodes){
                states.push_back(std::move(next));
            }
        }
    }
    return max_geodes;
}

int main(){
    std::string line;
    auto blueprints = std::vector<Blueprint>();
    while(std::getline(std::cin, line)){
        blueprints.emplace_back(line);
    }

    bool part_two = true;
    if(part_two){
        std::cout<<"Running part two"<<std::endl;
        LIMIT = 32;
        int value = 1;
        for(int i=0; i<3; i++){
            auto bp = blueprints.at(i);
            int geodes = calculate(bp);
            std::cout <<"Blueprint "<<bp.number<<", max geodes: "<<geodes<<std::endl;;
            value *= geodes;
        }
        std::cout<<"Total value "<<value<<std::endl;
    }else{
        std::cout<<"Running part one"<<std::endl;
        int value = 0;
        for(const auto& bp : blueprints){
            int geodes = calculate(bp);
            std::cout <<"Blueprint "<<bp.number<<", max geodes: "<<geodes<<std::endl;;
            value += bp.number*geodes;
        }
        std::cout<<"Total value "<<value<<std::endl;
    }
}


