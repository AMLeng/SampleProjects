#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <deque>

long LCM = 1;
class Monkey{
    public:
        Monkey(std::deque<long> starting, int n, bool operation, int test, int target1, int target2, bool s) :
            items(starting), operation_is_add(operation), factor_for_operation(n), test_num(test), t1(target1), t2(target2), inspected(0), special(s) {}
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
        void print(){
            for(long i : items){
                std::cout<< i<<',';
            }
            std::cout<<std::endl;
        }
        long get_inspected(){
            return inspected;
        }
    private:
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
        assert(nextline[0] == 'M');
        assert(monkeys.size() == std::stoi(nextline.substr(7)));
        auto init_items = std::deque<long>();
        std::getline(std::cin,nextline);
        init_items.push_back(std::stoi(nextline.substr(nextline.find(':')+1)));
        while(nextline.find(',') < nextline.size()-1){
            nextline = nextline.substr(nextline.find(',')+1);
            init_items.push_back(std::stoi(nextline));
        }
        std::getline(std::cin,nextline);
        bool o = (nextline[23] == '+');
        int f = 0;
        bool special = false;
        if(nextline[25] == 'o'){
            special = true;
        }else{
            f = std::stoi(nextline.substr(25));
        }
        std::getline(std::cin,nextline);
        int t = std::stoi(nextline.substr(21));
        LCM *= t;
        std::getline(std::cin,nextline);
        int t1 = std::stoi(nextline.substr(29));
        std::getline(std::cin,nextline);
        int t2 = std::stoi(nextline.substr(30));
        monkeys.emplace_back(init_items,f,o,t,t1,t2, special);
        std::getline(std::cin,nextline);
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
