#include <iostream>
#include <string>
#include <vector>
#include <array>

std::array<char,9> get_vals(std::string line){
    auto result = std::array<char,9>();
    for(int i=0; i<9; i++){
        if( line[1+4*i] == ' '){
            result[i] = 'a';
        }else{
            result[i] = line[1+4*i];
        }
    }
    return result;
}

void move(std::vector<std::array<char,9>>& stacks, int first, int second){
    char crate = 'a';
    for(int i=0; i<stacks.size(); i++){
        if(stacks[i][first-1]!= 'a'){
            crate = stacks[i][first-1];
            stacks[i][first - 1] = 'a';
            break;
        }
    }
    for(int i=0; i<stacks.size(); i++){
        if(stacks[i][second-1]!= 'a'){
            if(i>0){
                stacks[i-1][second-1] = crate;
            }else{
                stacks.insert(stacks.begin(),std::array<char,9>{'a','a','a','a','a','a','a','a','a'});
                stacks[0][second-1] = crate;
            }
            return;
        }
    }
    stacks[stacks.size()-1][second-1] = crate;
}
void reverse(std::vector<std::array<char,9>>& stacks, int stacknum, int crates_to_reverse){
    int i=0;
    for(; i<stacks.size();i++){
        if(stacks[i][stacknum-1] != 'a'){
            break;
        }
    }
    std::vector<char> crates = std::vector<char>();
    for(int j=0; j<crates_to_reverse; j++){
        crates.push_back(stacks[i+j][stacknum-1]);
    }
    for(int j=0; j<crates_to_reverse; j++){
        stacks[i+j][stacknum-1]=crates.back();
        crates.pop_back();
    }
}

int main(){
    std::string nextline;
    auto stacks = std::vector<std::array<char,9>>();
    std::getline(std::cin,nextline);
    while(nextline[1]!='1'){
        stacks.push_back(get_vals(nextline));
        std::getline(std::cin,nextline);
    }
    std::getline(std::cin,nextline);
    while(std::getline(std::cin,nextline)){
        std::cout<<nextline<<std::endl;
        nextline = nextline.substr(4);
        int N = std::stoi(nextline);
        nextline=nextline.substr(nextline.find("from")+5);
        int a = std::stoi(nextline);
        nextline=nextline.substr(nextline.find("to")+3);
        int b = std::stoi(nextline);
        for(int i=0; i<N; i++){
            move(stacks,a,b);
        }
        //Omit this line for part 1
        reverse(stacks,b,N);
    }
    std::cout<<"Answer"<<std::endl;
    for(int j=0; j<9; j++){
        for(int i=0; i<stacks.size(); i++){
            if(stacks[i][j]!= 'a'){
                std::cout<<stacks[i][j];
                break;
            }
        }
    }
    std::cout<<std::endl;
}
