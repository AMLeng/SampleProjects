#include <iostream>
#include <string>
#include <cctype>
#include <cassert>

int priority(char c){
    if(std::islower(c)){
        return c-'a'+1;
    }else{
        return c-'A'+27;
    }
}

char common(std::string x, std::string y){
    for(char c : x){
        if(y.find(c) < y.size()){
            return c;
        }
    }
    assert(false);
    return '0';
}
char common(std::string x, std::string y, std::string z){
    for(char c : x){
        if(y.find(c) < y.size() && z.find(c)<z.size()){
            return c;
        }
    }
    assert(false);
    return '0';
}
int main(){
    std::string nextline;
    std::string nextline2;
    std::string nextline3;
    int sum = 0;
    int sum2 = 0;
    while(std::getline(std::cin,nextline)){
        /*std::string s1 = nextline.substr(0,nextline.size()/2);
        std::string s2 = nextline.substr(nextline.size()/2);*/
        std::getline(std::cin,nextline2);
        std::getline(std::cin,nextline3);
        //sum+=priority(common(s1,s2));
        sum2+=priority(common(nextline,nextline2,nextline3));
    }
    //std::cout<<sum<<std::endl;
    std::cout<<sum2<<std::endl;
}
