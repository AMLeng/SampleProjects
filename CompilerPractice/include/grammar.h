#ifndef _GRAMMAR_
#define _GRAMMAR_
#include <vector>
#include <string>
#include <set>
#include <map>
//Special symbols:
//"Start" for start
//"$" for input end
//Empty symbol (Epsilon) is not represented; e.g. the derivation "A -> Epsilon" would be
//{{A},{}}
namespace grammar{
struct Grammar{
    public:
    Grammar(std::map<std::string,std::vector<std::vector<std::string>>> map) : 
        types(compute_types(map)), derivations(compute_derivations(map,types))
    {
        for(auto type : types){
            if(is_terminal(type)){
                produces_epsilon_map.emplace(type,false);
            }
        }
        for(auto type : types){
            set_produces_epsilon(type);
        }
    }
    bool is_terminal(std::string type) const{
        return derivations.at(type).size() == 0;
    }
    bool produces_epsilon(std::string type) const{
        return produces_epsilon_map.at(type);
    }
    //Returns the first set, except for the empty type
    /*std::set<std::string> first_without_epsilon(std::string type) const{
        if(is_terminal(type)){
            return std::set<std::string>{type};
        }else{
            auto seen = std::set<std::string>{type};
            for(auto derivation : derivations.at(type)){
                while(derivation.size() == 0
                auto next_type = derivation.at(0);
            }
        }
    }*/
    std::vector<std::vector<std::string>> der(std::string left) const{
        return derivations.at(left);
    }
    const std::set<std::string> types;
    const std::map<std::string,std::vector<std::vector<std::string>>> derivations;
    private:
    std::map<std::string, bool> produces_epsilon_map;

    void set_produces_epsilon(std::string type){
        if(produces_epsilon_map.find(type) != produces_epsilon_map.end()){
            return;
        }
        set_produces_epsilon(type,std::set<std::string>{type});
    }

    void set_produces_epsilon(std::string type, std::set<std::string> prev){
        if(produces_epsilon_map.find(type) != produces_epsilon_map.end()){
            return;
        }
        for(auto derivation : derivations.at(type)){
            //Make a copy so that we can whittle it down
            while(derivation.size() > 0){
                //makes no difference if we work from front or back
                auto current = derivation.back();
                if(prev.find(current) != prev.end()){
                    break; 
                }
                prev.insert(current);
                set_produces_epsilon(current,prev);
                if(produces_epsilon_map.find(current) != produces_epsilon_map.end() && !produces_epsilon_map.at(current)){
                    break; //This derivation can't produce epsilon
                }
                //Put last so the size exiting the loop is 0 if and only if
                //Every symbol in the derivation could derive epsilon
                derivation.pop_back();
            }
            if(derivation.size() == 0){
                produces_epsilon_map.emplace(type,true);
                return;
            }
        }
        produces_epsilon_map.emplace(type,false);
    }
    static std::set<std::string> compute_types(const std::map<std::string,std::vector<std::vector<std::string>>>& map){
        auto typeset = std::set<std::string>();
        for(auto pair : map){
            typeset.insert(pair.first);
            for(auto derivation : pair.second){
                for(auto term : derivation){
                    typeset.insert(term);
                }
            }
        }
        return typeset;
    }
    static std::map<std::string,std::vector<std::vector<std::string>>> compute_derivations(
        const std::map<std::string,std::vector<std::vector<std::string>>>& map, 
        const std::set<std::string>& types
        ){
        auto derivation_map = std::map<std::string,std::vector<std::vector<std::string>>>(map);
        for(auto type: types){
            if(derivation_map.find(type) == derivation_map.end()){
                //default assignment
                derivation_map[type];
            }
        }
        return derivation_map;
    }
};
auto decaf_grammar = Grammar(std::map<std::string,std::vector<std::vector<std::string>>>{{
    //Insert grammar rules for decaf here
}});
} //namespace grammar
#endif //_GRAMMAR_
