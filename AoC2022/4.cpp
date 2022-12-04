#include <iostream>
#include <string>

bool contains(int a, int b, int c, int d){
    return (a <= c && b >= d) || (a>= c && b <=d);
}
bool l_in_r(int a, int b, int c, int d){
    return (a <= c && b >= c) || (a>= d && b <=d);
}
int main(){
    std::string nextline;
    int sum = 0;
    int sum2 = 0;
    while(std::getline(std::cin,nextline)){
        int a = std::stoi(nextline);
        nextline = nextline.substr(nextline.find('-')+1);
        int b = std::stoi(nextline);
        nextline = nextline.substr(nextline.find(',')+1);
        int c = std::stoi(nextline);
        nextline = nextline.substr(nextline.find('-')+1);
        int d = std::stoi(nextline);
        if(contains(a,b,c,d)){
            sum++;
        }
        if(l_in_r(a,b,c,d) || l_in_r(c,d,a,b)){
            sum2++;
        }
    }
    std::cout<<sum<<std::endl;
    std::cout<<sum2<<std::endl;
}
