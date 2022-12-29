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
            for(auto derivation : pair.second){
                for(auto term : derivation){
                    types.insert(term);
                }
            }
        }
    }
    bool has_type(std::string type){
        return types.count(type) == 1;
    }
    std::map<std::string,std::vector<std::vector<std::string>>> derivations;
    std::set<std::string> types;
};
auto decaf_grammar = Grammar(std::map<std::string,std::vector<std::vector<std::string>>>{{
    //Insert grammar rules for decaf here
}});
} //namespace grammar
#endif //_GRAMMAR_
