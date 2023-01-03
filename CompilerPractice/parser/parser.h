#ifndef _PARSER
#define _PARSER_
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
namespace parser{

typedef std::string Type;
template <const grammar::Grammar* g>
class ItemSet;
template <const grammar::Grammar* g>
class Item{//Represents an LR(0) item
    friend ItemSet<g>;
    public:
    Item(Type lhs, std::vector<Type> rhs) : left(lhs), right(rhs), position(0) {
    }
    Type next() const{
        assert(!at_end());
        return right.at(position);
    }
    bool at_end() const{
        return position == right.size();
    }
    Item<g> shift() const{
        assert(!at_end());
        auto copy = Item(*this);
        copy.position++;
        return copy;
    }
    bool operator<(const Item<g>& rhs) const{
        if(this->left != rhs.left){
            return this->left < rhs.left;
        }
        if(this->right != rhs.right){
            return this->right < rhs.right;
        }
        if(this->position != rhs.position){
            return this->position < rhs.position;
        }
        return false;
    }
    void print() const{
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
    private:
    Type left;
    std::vector<Type> right;
    int position;
};

template <const grammar::Grammar* g>
class ItemSet{//Represents a set of LR(1) items---in particular stores lookaheads!
    public:
    ItemSet(std::set<Item<g>> kernel) : items(generate_lookaheads(kernel)){
        this->closure();
    }
    bool operator<(const ItemSet<g>& rhs) const{
        return lexicographical_compare(items.begin(),items.end(), rhs.items.begin(), rhs.items.end());
    }
    bool operator==(const ItemSet<g>& rhs) const{
        return !(*this < rhs) && !(rhs < *this);
    }
    //Codewise is simple to do but there are actually two (or three) things going on here
    //We take a LALR(1) item set, forget the lookaheads (by only looking at the first element of the pairs in the map)
    //And then, for each item, shift
    //This thus produces a LR(0) kernel
    //Which the constructor turns into the corresponding closed LALR(1) item set
    ItemSet<g> shift(Type next_type) const{
        auto items = std::set<Item<g>>();
        for(const auto& prev_item_pair : this->items){
            if(!prev_item_pair.first.at_end() && prev_item_pair.first.next()==next_type){
                items.insert(prev_item_pair.first.shift());
            }
        }
        return ItemSet<g>(items);
    }
    void print() const{
        for(auto i : items){
            std::cout<<"Item: ";
            i.print();
        }
    }
    int size() const{
        return items.size();
    }
    private:
    std::map<Item<g>,std::set<Type>> items;
    //Need to define this
    static std::map<Item<g>, std::set<Type>> generate_lookaheads(std::set<Item<g>> items){
        auto item_map = std::map<Item<g>,std::set<Type>>();
        for(auto i : items){
            item_map.emplace(i,std::set<Type>());
        }
        return item_map;
    }
    //Need to update this for LR(1) itemsets
    void closure(){
        auto already_seen = std::set<Item<g>>();
        while(already_seen.size() != items.size()){
            for(auto pair : items){
                Item<g> i = pair.first;
                std::set<Type> previous_lookaheads = pair.second;
                if(already_seen.find(i) == already_seen.end()){
                    already_seen.insert(i);
                    if(!i.at_end()){
                        auto next_type = i.right.at(i.position);
                        //Compute the possible lookaheads;
                        //Importantly, note that the lookaheads are the same
                        //for every derivation of next_type
                        auto lookaheads = std::set<Type>();
                        int index = i.position+1;
                        for(; index < i.right.size(); index++){
                            for(auto t : g->first_set(i.right.at(index))){
                                lookaheads.emplace(t);
                            }
                            if(!g->produces_epsilon(i.right.at(index))){
                                break;
                            }
                        }
                        if(index == i.right.size()){
                            for(auto t : previous_lookaheads){
                                lookaheads.emplace(t);
                            }
                        }
                        //Add in each possible derivation
                        for(auto derivation : g->der(next_type)){
                            items.emplace(Item<g>(next_type,derivation),lookaheads);
                        }
                    }
                }
            }
        }
    }
};

/*
template <const grammar::Grammar* g>
class Parser{
    public:
    Parser() : internals(generate_item_sets()) {
    }
    Parser(Parser&) = delete;
    Parser(Parser&&) = delete;
    int state_count(){
        return internals.first.size();
    }
    template <typename TypeIterator>
    const ItemSet<g>* eval(ItemSet<g> start_state, TypeIterator start, TypeIterator end){
        const ItemSet<g>* current = nullptr;
        for(auto& p : internals.first){
            if(*p == start_state){
                current = p.get();
                break;
            }
        }
        if(!current){
            return nullptr;//Initial state was not in the Parser
        }
        for(auto it = start; it != end; it++){
            current = transition(current, *it);
        }
        return current;
    }
    //The below assumes that the start_pointer is already one of the pointers managed by the unique_ptr in
    //the set internals.first
    template <typename TypeIterator>
    const ItemSet<g>* eval(const ItemSet<g>* start_pointer, TypeIterator start, TypeIterator end){
        for(auto it = start; it != end; it++){
            start_pointer = transition(start_pointer, *it);
        }
        return start_pointer;
    }
    private:
    //Pair of the set of items and the map for transitions
    const std::pair<std::set<std::unique_ptr<const ItemSet<g>>>,std::map<const ItemSet<g>*,std::map<Type,const ItemSet<g>*>>> internals;

    const ItemSet<g>* transition(const ItemSet<g>* start, Type next_type) const{
        assert(internals.second.find(start) != internals.second.end());
        if(internals.second.at(start).find(next_type) == internals.second.at(start).end()){
            return nullptr;
        }else{
            return internals.second.at(start).at(next_type);
        }
    }

    static auto generate_item_sets(){
        auto return_set = std::set<std::unique_ptr<const ItemSet<g>>>();
        auto transition_map = std::map<const ItemSet<g>*,std::map<Type,const ItemSet<g>*>>();
        auto to_add = std::deque<std::unique_ptr<const ItemSet<g>>>();
        to_add.push_back(std::make_unique<const ItemSet<g>>(std::set<Item<g>>{Item<g>("Start",g->der("Start").at(0))}));
        while(to_add.size() > 0){
            auto current = to_add.front().get();
            return_set.insert(std::move(to_add.front()));
            to_add.pop_front();
            transition_map.emplace(current,std::map<Type,const ItemSet<g>*>());
            //Loop over possible transitions
            for(auto t : g->types){
                auto possible_next = current->shift(t);
                if(possible_next.size() == 0){
                    continue;
                }
                const ItemSet<g>* next_pointer = nullptr;
                //See if the state after transitioning to t is already in our return_set
                for(auto& p : return_set){
                    if(*p == possible_next){
                        next_pointer = p.get();
                        break;
                    }
                }
                //Or is already dequed up to be added
                for(auto& p : to_add){
                    if(*p == possible_next){
                        next_pointer = p.get();
                        break;
                    }
                }
                if(!next_pointer){
                    to_add.push_back(std::make_unique<const ItemSet<g>>(possible_next));
                    next_pointer = to_add.back().get();
                }
                //At this point, next_pointer is non-null, holding a pointer to an ItemSet managed by a unique_ptr
                assert(transition_map.at(current).find(t) == transition_map.at(current).end());
                transition_map.at(current).emplace(t,next_pointer);
            }
        }
        return std::make_pair(std::move(return_set),std::move(transition_map));
    }
};*/
} //namespace parser
#endif //_PARSER_
