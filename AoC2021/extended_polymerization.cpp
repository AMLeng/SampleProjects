#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <cctype>
#include <climits>
#include <algorithm>
#include <cassert>

typedef std::pair<char, char> Pair;
class InsertionRules{
    friend class Polymer;
    public:
    void add_rule(std::string new_rule){
        while(std::isspace(new_rule.back())){
            new_rule.pop_back();
        }
        insertion_rules.emplace(std::make_pair(new_rule[0],new_rule[1]),new_rule.back());
        if(std::find(elements.begin(),elements.end(),new_rule[0]) == elements.end()){
            elements.emplace_back(new_rule[0]);
        }
        if(std::find(elements.begin(),elements.end(),new_rule[1]) == elements.end()){
            elements.emplace_back(new_rule[1]);
        }
        if(std::find(elements.begin(),elements.end(),new_rule.back()) == elements.end()){
            elements.emplace_back(new_rule.back());
        }
    }
    private:
    std::map<Pair,char> insertion_rules;
    std::vector<char> elements;

};
//We store a polymer by counting how many times each pair appears
//Since longer structure than that doesn't matter to us
class Polymer{
    public:
    Polymer(std::string polymer_str, InsertionRules rules) : front(polymer_str.front()), back(polymer_str.back()){
        for(char c1 : rules.elements){
            for(char c2 : rules.elements){
                occurances.emplace(std::make_pair(c1,c2),0LL);
            }
        }
        for(int i=1; i<polymer_str.size(); i++){
            occurances.at(std::make_pair(polymer_str[i-1],polymer_str[i]))++;
        }
    }
    void polymerize(InsertionRules rules){
        auto new_occurances = std::map<Pair, long long>();
        for(auto map_pair : occurances){
            new_occurances.emplace(map_pair.first,0LL);
        }
        for(auto map_pair : occurances){
            //For pairs without an insertion rule, they will carry over
            Pair char_pair = map_pair.first;
            if(rules.insertion_rules.find(char_pair) == rules.insertion_rules.end()){
                new_occurances.at(char_pair) += map_pair.second;
            }else{
            //Otherwise, we replace the pair with the two pairs it turns into
                new_occurances.at(std::make_pair(char_pair.first,rules.insertion_rules.at(char_pair))) += map_pair.second;
                new_occurances.at(std::make_pair(rules.insertion_rules.at(char_pair),char_pair.second)) += map_pair.second;
            }
        }
        occurances = new_occurances;
    }
    long long element_count(){
        auto counts = element_counts();
        long long min = LLONG_MAX;
        long long max = 0LL;
        for(auto pair : counts){
            if(min > pair.second){
                min = pair.second;
            }
            if(max < pair.second){
                max = pair.second;
            }
        }
        return max - min;
    }
    private:
    std::map<Pair,long long> occurances;
    char front;
    char back;
    std::map<char,long long> element_counts(){
        auto counts = std::map<char,long long>();
        for(auto map_pair: occurances){
            if(counts.find(map_pair.first.first) == counts.end()){
                counts.emplace(map_pair.first.first,0LL);
            }
        }
        for(auto map_pair : occurances){
            counts.at(map_pair.first.first) += map_pair.second;
            counts.at(map_pair.first.second) += map_pair.second;
        }
        counts.at(front)++;
        counts.at(back)++;
        for(auto map_pair : counts){
            assert(map_pair.second %2 == 0LL);
            counts.at(map_pair.first) = map_pair.second/2;
        }
        for(auto map_pair : counts){
            std::cout << map_pair.first<<":"<<map_pair.second<<std::endl;
        }
        return counts;
    }
};

int main(){
    //Read input
    InsertionRules rules = InsertionRules();
    std::string polymer_str;
    std::getline(std::cin, polymer_str);
    while(std::isspace(polymer_str.back())){
        polymer_str.pop_back();
    }
    std::string nextline;
    std::getline(std::cin, nextline);
    while(std::getline(std::cin, nextline)){
        rules.add_rule(nextline);
    }
    auto polymer = Polymer(polymer_str,rules);

    //Compute polymers
    long long count;
    for(int i=0; i<10; i++){
        polymer.polymerize(rules);
    }
    count = polymer.element_count();
    std::cout <<"After 10 steps, the difference between most and least common is " <<count <<std::endl;
    for(int i=10; i<40; i++){
        polymer.polymerize(rules);
    }
    count = polymer.element_count();
    std::cout <<"After 40 steps, the difference between most and least common is " <<count <<std::endl;
}
