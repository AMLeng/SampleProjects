#include <iostream>
#include <string>
#include <deque>

bool duplicate_check(std::deque<char> last){
    for(int i=0; i<last.size();i++){
        for(int j=i+1;j<last.size();j++){
            if(last[i]==last[j]){return true;}
        }
    }
    return false;
}
int main(){
    std::string nextline;
    std::getline(std::cin,nextline);
    auto last = std::deque<char>();
    int i=0;
    //for(;i<4;i++){
    for(;i<14;i++){
        last.push_back(nextline[i]);
    }
    while(duplicate_check(last)){
        last.pop_front();
        last.push_back(nextline[i]);
        i++;
    }
    std::cout<<i<<std::endl;
    
}


