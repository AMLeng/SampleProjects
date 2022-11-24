#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <memory>

#include <map>
#include <sstream>
#include <cctype>

//It doesn't seem like map can be made constexpr, and according to
//Callgrind, the lookups for corresponding values was taking a substantial amount of time
//So I went and replaced things like placeholder_value.at('[') with the corresponding int
//It probably would have been better to just use a c-type enum, but
//The original code already ran in <5 seconds
//Of course, it would be much faster without the many memory allocations this solution leads to;
//For a large enough input, it would probably be better to have an object-oriented solution
const std::map<char,int> placeholder_value = {{'[',-3},{',',-2},{']',-1}};
const std::map<int,char> placeholder_to_value = {{-3,'['},{-2,','},{-1,']'}};

class SnailNum{
    public:
        explicit SnailNum(std::string input){
            auto s = std::stringstream(input);
            while(s.peek(),!s.eof()){
                data.push_back(read_next(s));
            }
        }
        SnailNum& operator+=(const SnailNum& number){
            data.insert(data.begin(),-3);
            data.emplace_back(-2);
            for(int c : number.data){
                data.emplace_back(c);
            }
            data.emplace_back(-1);
            reduce();
            return *this;
        }
        int magnitude(){
            std::vector<int> scaling_factors;
            int sum = 0;
            for(int x : data){
                if(x == -3){
                    scaling_factors.push_back(3);
                }
                if(x == -2){
                    scaling_factors.back() = 2;
                }
                if(x == -1){
                    scaling_factors.pop_back();
                }
                if(x > 0){
                    int value = x;
                    for(int factor : scaling_factors){
                        value *= factor;
                    }
                    sum += value;
                }
            }
            return sum;
        }
        void print(){
            for(int i : data){
                if(i>=0){
                    std::cout<<i;
                }else{
                    std::cout<<placeholder_to_value.at(i);
                }
            }
            std::cout<<std::endl;
        }
    private:
        std::vector<int> data;
        int read_next(std::istream& input_stream){
            char next = input_stream.peek();
            int result = 0;
            if(std::isdigit(next)){
                input_stream >> result;
                return result;
            }else{
                return placeholder_value.at(input_stream.get());
            }
        }
        bool explode(){
            int d = 0;
            //Find bounds for leftmost depth 4 pair
            int left_index = -1;
            for(int i=0; i<data.size(); i++){
                if(data[i] == -3){
                    d++;
                    if(d==5){
                        left_index = i;
                        break;
                    }
                }
                if(data[i] == -1){
                    d--;
                }
            }
            if(left_index == -1){
                return false;
            }
            int right_index = std::find(data.begin()+left_index,data.end(),-1)-data.begin();
            //Add pair's values to adjacent numbers
            auto rit = data.rbegin() + data.size() - left_index; //Reverse iterator starting at left_index
            for(;rit!=data.rend();rit++){
                if(*rit >= 0){
                    *rit += data[left_index+1];
                    break;
                }
            }
            auto it = data.begin() + right_index;
            for(;it!=data.end();it++){
                if(*it >= 0){
                    *it += data[right_index-1];
                    break;
                }
            }
            //Replace pair with 0
            data.erase(data.begin()+left_index,data.begin()+right_index);
            data[left_index] = 0;
            return true;
        }
        bool split(){
            int insert_index = -1;
            for(int i=0; i<data.size(); i++){
                if(data[i]>=10){
                    insert_index = i;
                    break;
                }
            }
            if(insert_index == -1){
                return false;
            }
            int value = data[insert_index];
            data[insert_index] = -1;
            data.insert(data.begin()+insert_index,{-3,value/2,-2,(value+1)/2});
            return true;
        }
        void reduce(){
            while(explode() || split()){
            }
        }
};

int maximal_magnitude(std::vector<SnailNum>& numbers){
    int max = 0;
    for(int i=0; i<numbers.size(); i++){
        for(int j=0; j<numbers.size(); j++){
            if(j == i){continue;}
            auto num = numbers[i];
            num += numbers[j];
            int magnitude = num.magnitude();
            if(magnitude > max){
                max = magnitude;
            }
        }
    }
    return max;
}
int main(){
    std::string nextline;
    auto numbers = std::vector<SnailNum>();
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        numbers.emplace_back(nextline);
    }
    auto number = numbers.front();
    for(int i=1; i<numbers.size(); i++){
        number += numbers[i];
    }
    std::cout<<"Result after summing all numbers is ";
    number.print();
    std::cout<<"Magnitude of the result is "<<number.magnitude()<<std::endl;
    std::cout<<"Maximal magnitude of the sum of any two is "<<maximal_magnitude(numbers)<<std::endl;
}
