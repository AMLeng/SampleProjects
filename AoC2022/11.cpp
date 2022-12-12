#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <deque>


class Monkey{
    public:
        Monkey() = default;
        void receive(long item){
            items.push_back(item);
        }
        void round(std::vector<Monkey>& monkeys){
            while(items.size() > 0){
                long current = items.front();
                this->items.pop_front();
                current = inspect(current);
                if(check_throw(current)){
                    monkeys.at(t1).receive(current);
                }else{
                    monkeys.at(t2).receive(current);
                }
            }
        }
        long get_inspected(){
            return inspected;
        }
        friend Monkey create_monkey(std::vector<std::string> input);
    private:
        static long LCM;
        int t1;
        int t2;
        bool check_throw(long item){
            if(item % test_num == 0){
                return true;
            }
            return false;
        }
        long inspect(long item){
            inspected++;
            /*if(special){
                return (item*item)/3;
            }
            if(operation_is_add){
                return (item + factor_for_operation)/3;
            }else{
                return (item * factor_for_operation)/3;
            }*/
            if(special){
                return (item*item) % LCM;
            }
            if(operation_is_add){
                return (item + factor_for_operation) % LCM;
            }else{
                return (item * factor_for_operation) % LCM;
            }
        }
        std::deque<long> items;
        bool operation_is_add;
        int factor_for_operation;
        int test_num;
        int inspected;
        bool special;

};
long Monkey::LCM = 1;

Monkey create_monkey(std::vector<std::string> input){
    auto m = Monkey();
    std::string current = input[1];
    m.items.push_back(std::stoi(current.substr(current.find(':')+1)));
    while(current.find(',') < current.size()-1){
        current = current.substr(current.find(',')+1);
        m.items.push_back(std::stoi(current));
    }
    m.operation_is_add = (input[2][23] == '+');
    if(input[2][25] == 'o'){
        m.special = true;
    }else{
        m.factor_for_operation = std::stoi(input[2].substr(25));
    }
    m.test_num = std::stoi(input[3].substr(21));
    Monkey::LCM *= m.test_num;
    m.t1 = std::stoi(input[4].substr(29));
    m.t2 = std::stoi(input[5].substr(30));
    return m;
}

long monkey_bis(std::vector<Monkey>& monkeys){
    long max = 0;
    long max2=0;
    for(auto m : monkeys){
        long v = m.get_inspected();
        if(v>max){
            max2=max;
            max = v;
        }else{
            if(v>max2){
                max2=v;
            }        }
    }
    return max*max2;
}


int main(){
    std::string nextline;
    auto monkeys = std::vector<Monkey>();
    while(std::getline(std::cin,nextline)){
        auto input = std::vector<std::string>();
        for(int i=0; i<6; i++){
            input.push_back(nextline);
            std::getline(std::cin,nextline);
        }
        monkeys.push_back(create_monkey(input));
    }
    //for(int i=0; i<20; i++){
    for(int i=0; i<10000; i++){
        for(auto& m : monkeys){
            m.round(monkeys);
        }
    }
    for(int i=0; i<monkeys.size(); i++){
        std::cout<<i<<":  "<<monkeys.at(i).get_inspected()<<std::endl;
    }

    std::cout<<monkey_bis(monkeys)<<std::endl;
}
