#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>

template <typename T>
long long get_value(T x){
    return x.first;
}

template <typename T>
void mix(std::vector<T>& list, const std::vector<T> original_list){
    for(int i=0; i<original_list.size();i++){
        long long value = get_value(original_list.at(i));
        if(value == 0){
            continue;
        }
        int index = std::find(list.begin(),list.end(), original_list.at(i)) - list.begin();
        //The cast from the list size to a *SIGNED* int is necessary here
        //Or we run into some difficult bugs
        int target_index = ((index + value) % static_cast<int>(list.size()-1));
        //Different wrapping around behavior for the two cases
        //If positive, should never end up at size() -1
        //If negative, should never end up at 0
        if(value < 0){
            while(target_index <= 0){
                target_index += list.size()-1;
            }
        }

        if(target_index > index){
            std::rotate(list.begin() + index, list.begin() + index + 1, list.begin() + target_index + 1);
        }
        if(target_index < index){
            std::rotate(list.begin() + target_index, list.begin() + index, list.begin() + index + 1);
        }
    }
}

int main(){
    std::string line;
    //Part 1
    //int decryption_key = 1;
    //int mix_number = 1;
   
    //Part 2
    long long decryption_key = 811589153;
    int mix_number = 10;
    auto list = std::vector<std::pair<long long, int>>();
    int i=0;
    while(std::getline(std::cin, line)){
        list.emplace_back(std::stoi(line),i);
        i++;
    }
    //auto list = std::vector<long long>{{1,2,-3,3,-2,0,4}};

    for(int i=0; i<list.size(); i++){
        list.at(i).first *= decryption_key;
    }

    auto mix_list = std::vector<std::pair<long long,int>>(list);
    for(int i=0; i<mix_number; i++){
         mix(mix_list, list);
    }
    int n = list.size();
    int zero_pos = 0;
    for(; zero_pos<mix_list.size(); zero_pos++){
        if(get_value(mix_list.at(zero_pos)) == 0){
            break;
        }
    }
    long long a = get_value(mix_list.at((zero_pos + 1000)% n));
    long long b = get_value(mix_list.at((zero_pos + 2000)% n));
    long long c = get_value(mix_list.at((zero_pos + 3000)% n));
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<c<<std::endl;
    std::cout<<"Total sum is: "<<a+b+c<<std::endl;
}
