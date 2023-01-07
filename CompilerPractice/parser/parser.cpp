#include <iostream>
#include <cassert>
#include <deque>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <memory>
#include "grammar.h"
#include "parser.h"
namespace parser{

template <const grammar::Grammar* g>
void Item<g>::print() const{
    std::cout<<left;
    std::cout<<" ->";
    for(int i=0; i<right.size(); i++){
        if(i == position){
            std::cout<<" .";
        }
        std::cout<<" ";
        std::cout<<right.at(i);
    }
    if(right.size() == position){
        std::cout<<" .";
    }
    std::cout<<std::endl;
}

template <const grammar::Grammar* g>
void ItemSet<g>::print() const{
    std::cout<<"Size: "<<items.size()<<std::endl;
    for(auto pair : items){
        auto i = pair.first;
        std::cout<<"Item: ";
        i.print();
        std::cout<<"Lookaheads: ";
        for(auto la : pair.second){
            std::cout<<la<<',';
        }
        std::cout<<std::endl;
    }
}

template <const grammar::Grammar* g>
bool ItemSet<g>::take_lookaheads(ItemSet<g>& other){
    bool lookahead_taken = false;
    //Must have same LR(0) item sets with different lookaheads
    assert(*this == other);
    for(auto& pair : items){
        auto lookaheads = other.items.at(pair.first);
        for(auto la : lookaheads){
            auto insert_result = pair.second.emplace(la);
            if(insert_result.second){
                lookahead_taken = true;
            }
        }
    }
    return lookahead_taken;
}

template <const grammar::Grammar* g>
void ItemSet<g>::closure_lookahead_helper(
    const std::map<Item<g>,std::set<Type>>& item_map, 
    const Item<g>& i, 
    std::map<Type,std::set<Type>>& lookaheads)
{
    if(i.position < i.right.size()){
        auto next_type = i.right.at(i.position);
        int index = i.position+1;
        for(; index < i.right.size(); index++){
            for(auto t : g->first_set(i.right.at(index))){
                lookaheads.at(next_type).emplace(t);
            }
            if(!g->produces_epsilon(i.right.at(index))){
                break;
            }
        }
        if(index == i.right.size()){
            for(auto t : item_map.at(i)){
                lookaheads.at(next_type).emplace(t);
            }
            //Adds a nonterminal to the lookahead set
            //which must be expanded later
            lookaheads.at(next_type).emplace(i.left);
        }
    }
}
template <const grammar::Grammar* g>
std::map<Item<g>,std::set<Type>> ItemSet<g>::closure(std::map<Item<g>,std::set<Type>> item_map){
    auto added = std::map<Type,bool>();
    auto to_add = std::map<Type,bool>();
    auto lookaheads = std::map<Type,std::set<Type>>();
    for(auto pair : g->derivations){
        added.emplace(pair.first,false);
        to_add.emplace(pair.first,false);
        lookaheads.emplace(pair.first,std::set<Type>());
    }
    for(auto pair : item_map){
        auto i = pair.first;
        if(!i.at_end()){
            to_add.at(i.right.at(i.position)) = true;
        }
        //Every time a given item creates additional items for the closure
        //We also need to keep track of the relevant additional lookaheads
        closure_lookahead_helper(item_map,i,lookaheads);
    }
    //Add LR(0) items
    while(added != to_add){
        for(auto pair : to_add){
            if(pair.second && !added.at(pair.first)){
                for(auto new_derivation : g->derivations.at(pair.first)){
                    auto i = Item<g>(pair.first,new_derivation);
                    item_map.emplace(i,std::set<Type>());
                    if(!i.at_end()){
                        to_add.at(i.right.at(i.position)) = true;
                    }
                    closure_lookahead_helper(item_map,i,lookaheads);
                }
                added.at(pair.first) = true;
            }
        }
    }
    //Expand the nonterminals in our stored lookaheads
    bool lookahead_added;
    do{
        lookahead_added = false;
        for(auto& pair : lookaheads){
            for(auto nonterminal_lookahead : pair.second){
                if(!g->is_terminal(nonterminal_lookahead)){
                    for(auto lookahead : lookaheads.at(nonterminal_lookahead)){
                        if(g->is_terminal(lookahead) && pair.second.find(lookahead) == pair.second.end()){
                            pair.second.emplace(lookahead);
                            lookahead_added = true;
                        }
                    }
                }
            }
        }
    }while(lookahead_added);

    //Now that we've compiled the lookaheads produced, go back and add them to our item map
    for(auto pair : lookaheads){
        if(added.at(pair.first)){
            for(auto new_derivation : g->derivations.at(pair.first)){
                auto i = Item<g>(pair.first,new_derivation);
                for(auto lookahead : pair.second){
                    //Only add terminals
                    if(g->is_terminal(lookahead)){
                        item_map.at(i).emplace(lookahead);
                    }
                }
            }
        }
    }

    return item_map;
}

template <const grammar::Grammar* g>
auto Parser<g>::generate_item_sets(){
    auto item_sets = std::set<std::unique_ptr<ItemSet<g>>>();
    auto to_add = std::deque<std::unique_ptr<ItemSet<g>>>();
    to_add.push_back(std::make_unique<ItemSet<g>>(std::set<Item<g>>{Item<g>("Start",g->der("Start").at(0))}));
    while(to_add.size() > 0){
        auto current = std::move(to_add.front());
        to_add.pop_front();
        //Loop over possible transitions
        //First, deal with all transitions from current to itself
        for(auto it = g->types.cbegin(); it != g->types.cend();){
            auto t = *it;
            auto possible_next = current->shift(t);
            if(possible_next == *current && current->take_lookaheads(possible_next)){
                it = g->types.cbegin();
            }else{
                it++;
            }
        }
        for(const auto& t : g->types){
            auto possible_next = current->shift(t);
            if(possible_next.size() == 0 || possible_next == *current){
                continue;
            }
            bool found = false;
            //See if the state after transitioning to t is already dequed up to be added
            for(auto& p : to_add){
                if(*p == possible_next){
                    assert(found == false);
                    found = true;
                    p->take_lookaheads(possible_next);
                    //break;
                }
            }
            //Or is already in our item_sets
            //In which case we will add the return set's lookaheads to our current object
            //And then delete the copy in the return set
            //for(auto& p : item_sets)
            for(auto it = item_sets.cbegin(); it != item_sets.cend(); it++){
                if(**it == possible_next){
                    assert(found == false);
                    found = true;
                    //Even if it is present, lookaheads might be different
                    if((*it)->take_lookaheads(possible_next)){
                        possible_next = **it;
                        //Delete the old itemset from the set of unique_ptr
                        it = item_sets.erase(it);
                        found = false;
                    }
                    //break;
                }
            }
            if(!found){
                to_add.push_back(std::make_unique<ItemSet<g>>(possible_next));
            }
        }
        item_sets.insert(std::move(current));
    }
    //Create const copies
    auto correspondence = std::map<ItemSet<g>*, State>();
    auto return_set = std::set<std::unique_ptr<const ItemSet<g>>>();
    for(auto& p : item_sets){
        auto insert_pair = return_set.insert(std::make_unique<const ItemSet<g>>(*p));
        const ItemSet<g>* new_p = insert_pair.first->get();
        correspondence.emplace(p.get(),new_p);
    }
    assert(return_set.size() == item_sets.size());
    //End by constructing the transition map
    auto return_map = std::map<State,std::map<Type,State>>();
    for(auto&p : return_set){
        auto current = p.get();
        return_map.emplace(current,std::map<Type,State>());
        for(auto t : g->types){
            auto possible_next = current->shift(t);
            if(possible_next.size() == 0){
                continue;
            }
            //See if the state after transitioning to t is already dequed up to be added
            for(auto& p_next : return_set){
                if(*p_next == possible_next){
                    return_map.at(current).emplace(t,p_next.get());
                    break;
                }
            }
        }
    }
    return std::make_pair(std::move(return_set),std::move(return_map));
}

template <const grammar::Grammar* g>
template <typename Iterator>
bool Parser<g>::parses(Iterator start, Iterator end){
    State start_state = find(ItemSet<g>(std::set<Item<g>>{Item<g>("Start",g->der("Start").at(0))}));
    auto state_stack = std::vector<State>{start_state};
    auto type_stack = std::vector<Type>{};
    while(start != end){
        Type next_type = *start;
        assert(state_stack.back() != nullptr);
        State shift_state = shift(state_stack.back(),next_type);
        auto reduction = reduce(state_stack.back(),next_type);
        if(!shift_state && !reduction){
            /*std::cout<<"Current states: \n";
            for(auto x : state_stack){
                x->print();
                std::cout<<std::endl;
            }
            std::cout<<"Current type: ";
            for(auto x : type_stack){
                std::cout<<x<<" ";
            }
            std::cout<<std::endl;
            std::cout<<"Remaining input: ";
            while(start != end){
                std::cout<<*start<<" ";
                start++;
            }
            std::cout<<std::endl;*/
            return false;
        }
        if(shift_state){
            assert(!reduction);
            //std::cout<<"Shift "<<next_type<<std::endl;
            state_stack.push_back(shift_state);
            type_stack.push_back(next_type);
            start++;
        }
        if(reduction){
            //std::cout<<"Reduce ";
            //reduction->print();
            for(int i = reduction->right.size() - 1; i>=0; i--){
                assert(reduction->right.at(i) == type_stack.back());
                type_stack.pop_back();
                state_stack.pop_back();
            }
            type_stack.push_back(reduction->left);
            assert(shift(state_stack.back(), reduction->left) != nullptr);
            state_stack.push_back(shift(state_stack.back(), reduction->left));
        }
    }
    //Have now read entire input, so remains to reduce as much as possible
    for(auto reduction = reduce(state_stack.back(),"$");reduction != nullptr; reduction = reduce(state_stack.back(),"$")){
        for(int i = reduction->right.size() - 1; i>=0; i--){
            assert(reduction->right.at(i) == type_stack.back());
            type_stack.pop_back();
            state_stack.pop_back();
        }
        type_stack.push_back(reduction->left);
        if(reduction->left == "Start"){
            return true;
        }
        assert(shift(state_stack.back(), reduction->left) != nullptr);
        state_stack.push_back(shift(state_stack.back(), reduction->left));
    }
    //Even after reducing as much as possible, did not get start symbol
    return false;
}
} //namespace parser
