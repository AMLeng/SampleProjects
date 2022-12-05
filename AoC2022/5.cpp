#include <iostream>
#include <string>
#include <deque>
#include <array>

void move(std::array<std::deque<char>,9>& stacks, int o, int t){
    char c = stacks[o].back();
    stacks[o].pop_back();
    stacks[t].push_back(c);
}

void reverse(std::array<std::deque<char>,9>& stacks, int s, int count){
    auto temp = std::deque<char>();
    for(int i=0; i<count; i++){
        temp.push_back(stacks[s].back());
        stacks[s].pop_back();
    }
    for(int i=0; i<count; i++){
        stacks[s].push_back(temp.front());
        temp.pop_front();
    }
}
int main(){
    std::string nextline;
    auto stacks = std::array<std::deque<char>,9>();
    std::getline(std::cin,nextline);
    while(nextline[1] != '1'){
        for(int i=0; i<9; i++){
            char c = nextline[1+4*i];
            if(c!=' '){
                stacks[i].push_front(c);
            }
        }
        std::getline(std::cin,nextline);
    }
    std::getline(std::cin,nextline);
    while(std::getline(std::cin,nextline)){
        nextline = nextline.substr(4);
        int count = std::stoi(nextline);
        nextline = nextline.substr(nextline.find("from")+5);
        int origin = std::stoi(nextline)-1;
        nextline = nextline.substr(nextline.find("to")+3);
        int target = std::stoi(nextline)-1;
        for(int j=0; j<count; j++){
            move(stacks,origin,target);
        }
        reverse(stacks,target,count);
    }
    for(int i=0; i<9; i++){
        std::cout<<stacks[i].back();
    }
    std::cout<<std::endl;
}
