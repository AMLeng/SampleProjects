#include <iostream>
#include <string>
#include <cassert>
#include <map>
#include <deque>

const std::map<char,int> digits = {{{'2',2},{'1',1},{'0',0},{'-',-1},{'=',-2}}};
const std::map<int,char> convert = {{{4,'2'},{3,'1'},{2,'0'},{1,'-'},{0,'='}}};
long long my_stod(std::string snafu){
    long long num = 0;
    long long place = 1;
    while(snafu.size() > 0){
        num += digits.at(snafu.back())*place;
        snafu.pop_back();
        place *= 5;
    }
    return num;
}
std::string dtos(long long num){
    auto base_5 = std::deque<int>();
    long long place = 1;
    while(place < num){
        place *= 5;
    }
    while(place > 0){
        num += 2*place;
        place /= 5;
    }
    while(num > 0){
        base_5.push_front(num%5);
        num = num/5;
    }
    std::string snafu;
    for(int x : base_5){
        snafu.push_back(convert.at(x));
    }
    return snafu;
}

int main(){
    std::string line;
    long long sum = 0;
    while(std::getline(std::cin,line)){
        //std::cout<<my_stod(line)<<std::endl;
        sum += my_stod(line);
    }
    std::cout<<"Sum: "<<sum<<std::endl;
    std::cout<<dtos(sum)<<std::endl;
    std::cout<<my_stod(dtos(sum))<<std::endl;
}
