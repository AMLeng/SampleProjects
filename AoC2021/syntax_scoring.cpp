#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>

std::map<char,int> corrupt_score = {{')',3},{']',57},{'}',1197},{'>',25137}};
std::map<char,int> complete_score = {{')',1},{']',2},{'}',3},{'>',4}};
std::map<char,char> matching = {{'(',')'},{'[',']'},{'{','}'},{'<','>'}};

//Returns the appropriate score if the line is corrupted, and 0 otherwise
int score_corrupted(std::string line){
    auto to_match = std::stack<char>();
    for(char c : line){
        if(matching.find(c)!= matching.end()){
            to_match.push(c);
        }
        if(corrupt_score.find(c) != corrupt_score.end()){
            if(matching.at(to_match.top()) == c){
                to_match.pop();
            }else{
                return corrupt_score.at(c);
            }
        }
    }
    //Line was valid
    return 0;
}
//Returns the appropriate score if the line is incomplete, and 0 if complete
long score_incomplete(std::string line){
    auto to_match = std::stack<char>();
    for(char c : line){
        if(matching.find(c)!= matching.end()){
            to_match.push(c);
        }
        if(complete_score.find(c) != complete_score.end()){
            if(matching.at(to_match.top()) == c){
                to_match.pop();
            }
        }
    }
    long score = 0;
    while(to_match.size()>0){
        score = score*5 + complete_score.at(matching.at(to_match.top()));
        to_match.pop();
    }
    return score;
}

int main(){
    int corrupted_score = 0;
    std::vector<long> incomplete_scores = {};
    std::string nextline;
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        int corrupted_check = score_corrupted(nextline);
        if(corrupted_check){
            corrupted_score += score_corrupted(nextline);
        }else{
            incomplete_scores.emplace_back(score_incomplete(nextline));
        }
    }
    std::sort(incomplete_scores.begin(),incomplete_scores.end());
    std::cout<<"Part 1: "<<corrupted_score<<std::endl;
    std::cout<<"Part 2: "<<incomplete_scores[incomplete_scores.size()/2]<<std::endl;
}
