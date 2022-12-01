#include <iostream>
#include <string>
#include <vector>

int max(std::vector<int>& list){
    int max=0;
    for(int v:list){
        if(v>max){
            max = v;
        }
    }
    return max;
}

int max3(std::vector<int>& list){
    int max = 0;
    int max2=0;
    int max3=0; 
    for(int v : list){
        if(v>max){
            max3=max2;
            max2=max;
            max = v;
        }else{
            if(v>max2){
                max3=max2;
                max2=v;
            }else{
                if(v>max3){
                    max3=v;
                }
            }
        }
    }
    return max+max2+max3;
}
int main(){
    std::string nextline;
    std::vector<int> calories = std::vector<int>();
    calories.emplace_back(0);

    while(std::getline(std::cin,nextline)){
        if(nextline.size() != 0){
            calories.back()+= std::stoi(nextline);
        }else{
            calories.emplace_back(0);
        }
    }
    std::cout<<max(calories)<<std::endl;
    std::cout<<max3(calories)<<std::endl;
}
