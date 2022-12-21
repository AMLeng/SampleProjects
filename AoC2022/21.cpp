#include <iostream>
#include <string>
#include <cassert>
#include <map>
#include <climits>
#include <stdexcept>

long long apply_op(long long a, long long b, char c){
    if(c == '*'){
        return a*b;
    }
    if(c == '-'){
        return a - b;
    }
    if(c == '/'){
        return a/b;
    }
    assert(c == '+');
    return a+b;
}

class MonkeyTable{
    public:
        MonkeyTable() = default;
        long long at(std::string value){
            return monkeys.at(value);
        }
        void add(std::string line, bool part_two){
            if(std::isdigit(line[6])){
                if(line.substr(0,4) != "humn" || !part_two){
                    monkeys.insert(std::make_pair(line.substr(0,4),std::stoi(line.substr(6))));
                }
            }else{
                if(line.substr(0,4) == "root" && part_two){
                    line[11] = reverse.at(line.at(11));
                    monkey_inputs.insert(std::make_pair(line.substr(0,4),line.substr(6)));
                    monkeys.insert(std::make_pair(line.substr(0,4),0));
                }else{
                    monkey_inputs.insert(std::make_pair(line.substr(0,4),line.substr(6)));
                }
                reverse_inputs.insert(std::make_pair(line.substr(6,4),line.substr(0,4)+ " "+ reverse.at(line[11])+" "+line.substr(13,4)));
                if(line[11] == '-' || line[11] == '/'){
                    reverse_inputs.insert(std::make_pair(line.substr(13,4),line.substr(6,4)+ " "+ line[11]+" "+line.substr(0,4)));
                }else{
                    reverse_inputs.insert(std::make_pair(line.substr(13,4),line.substr(0,4)+ " "+ reverse.at(line[11])+" "+line.substr(6,4)));
                }
            }
        }
        void compute(std::string value, bool rev = false){
            if(monkeys.find(value) != monkeys.end()){
                return;
            }
            auto input = monkey_inputs;
            if(value != "root" && rev){
                input = reverse_inputs;
            }
            std::string command = input.at(value); //This is the line that ends up being "caught" by the try catch
            std::string key1 = command.substr(0,4);
            std::string key2 = command.substr(7,4);
            char op = command[5];
            if(monkeys.find(key1) == monkeys.end()){
                //Bounds validation for this line happens up above
                //Since outside of "root", everything non-constant has both a forward and reverse command
                if(value == input.at(key1).substr(0,4) || value == input.at(key1).substr(7,4)){
                    try{
                        compute(key1);
                    }catch(const std::out_of_range& e){
                    }
                }else{
                    compute(key1, rev);
                }
            }
            if(monkeys.find(key2) == monkeys.end()){
                if(value == input.at(key2).substr(0,4) || value == input.at(key2).substr(7,4)){
                    try{
                        compute(key2);
                    }catch(const std::out_of_range& e){
                    }
                }else{
                    compute(key2, rev);
                }
            }
            monkeys.emplace(value,apply_op(monkeys.at(key1), monkeys.at(key2), op));
        }
    private:
        static const std::map<char,char> reverse;
        std::map<std::string,std::string> monkey_inputs;
        std::map<std::string,std::string> reverse_inputs;
        std::map<std::string,long long> monkeys;
};

const std::map<char,char> MonkeyTable::reverse = {{{'*','/'},{'+','-'},{'/','*'},{'-','+'}}};

int main(){
    std::string line;
    std::map<std::string,std::string> reverse_inputs;
    auto table = MonkeyTable();
    bool part_two = true; //Just change this line to swap between parts 1 and 2
    while(std::getline(std::cin,line)){
        table.add(line, part_two);
    }
    
    std::string test = "root";
    if(part_two){
        test = "humn";
    }
    table.compute(test, part_two);
    std::cout<<table.at(test)<<std::endl;
    
}
