#ifndef _GRAMMAR_
#define _GRAMMAR_
#include <vector>
#include <string>
#include <set>
namespace grammar{
struct Grammar{
    Grammar(std::map<std::string,std::vector<std::vector<std::string>>> map) : derivations(map), types(){
        for(auto pair : derivations){
            types.insert(pair.first);
            nonterminals.insert(pair.first);
            for(auto derivation : pair.second){
                for(auto term : derivation){
                    types.insert(term);
                }
            }
        }
    }
    bool has_type(std::string type) const{
        return types.count(type) == 1;
    }
    std::vector<std::vector<std::string>> der(std::string left) const{
        return derivations.at(left);
    }
    std::set<std::string> types;
    std::set<std::string> nonterminals;
    //private:
    std::map<std::string,std::vector<std::vector<std::string>>> derivations;
};
auto decaf_grammar = Grammar(std::map<std::string,std::vector<std::vector<std::string>>>{{
    //Insert grammar rules for decaf here
}});
} //namespace grammar
#endif //_GRAMMAR_
