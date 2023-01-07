#ifndef _PARSER_
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
class Parser;

template <const grammar::Grammar* g>
class Item{//Represents an LR(0) item
    friend ItemSet<g>;
    friend Parser<g>;
    public:
    Item(Type lhs, std::vector<Type> rhs) : left(lhs), right(rhs), position(0) {
    }
    Type next() const{
        assert(!at_end());
        return right.at(position);
    }
    bool at_end() const{
        assert(position <= right.size());
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
    bool operator==(const Item<g>& rhs) const{
        return !(*this < rhs) && !(rhs < *this);
    }
    void print() const;
    private:
    Type left;
    std::vector<Type> right;
    int position;
};

template <const grammar::Grammar* g>
class ItemSet{//Represents a set of LR(1) items---in particular stores lookaheads!
    public:
    friend Parser<g>;
    ItemSet(std::map<Item<g>,std::set<Type>> kernel) : items(closure(kernel)){
    }
    ItemSet(std::set<Item<g>> kernel) : ItemSet(set_to_map(kernel)){
    }
    //Compare only the LR(0) item portions
    bool operator<(const ItemSet<g>& rhs) const{
        return lexicographical_compare(items.begin(),items.end(), 
            rhs.items.begin(), rhs.items.end(),
            [](auto lhs, auto rhs){return lhs.first < rhs.first;});
    }
    bool operator==(const ItemSet<g>& rhs) const{
        return !(*this < rhs) && !(rhs < *this);
    }
    //Takes every element, and shifts the item if possible, leaving the lookaheads the same
    ItemSet<g> shift(Type next_type) const{
        auto new_items = std::map<Item<g>,std::set<Type>>();
        for(const auto& prev_item_pair : this->items){
            if(!prev_item_pair.first.at_end() && prev_item_pair.first.next()==next_type){
                new_items.emplace(prev_item_pair.first.shift(),prev_item_pair.second);
            }
        }
        return ItemSet<g>(new_items);
    }
    void print() const;
    int size() const{
        return items.size();
    }
    std::set<Type> lookaheads(Item<g> item) const{
        return items.at(item);
    }

    private:
    //Must not be const since we will typically be adding lookaheads to an itemset after it has been created
    std::map<Item<g>,std::set<Type>> items;
    bool take_lookaheads(ItemSet<g>& other);
    static std::map<Item<g>, std::set<Type>> set_to_map(std::set<Item<g>> items){
        auto item_map = std::map<Item<g>,std::set<Type>>();
        for(auto i : items){
            if(i.left == "Start"){
                item_map.emplace(i,std::set<Type>{"$"});
            }else{
                item_map.emplace(i,std::set<Type>());
            }
        }
        return item_map;
    }
    static void closure_lookahead_helper(
        const std::map<Item<g>,std::set<Type>>& item_map, 
        const Item<g>& i, 
        std::map<Type,std::set<Type>>& lookaheads);
    static std::map<Item<g>,std::set<Type>> closure(std::map<Item<g>,std::set<Type>> item_map);
};


template <const grammar::Grammar* g>
class Parser{
    public:
    using State = const ItemSet<g>*;
    Parser() : internals(generate_item_sets()) {
    }
    Parser(Parser&) = delete;
    Parser(Parser&&) = delete;
    int state_count() const{
        return internals.first.size();
    }
    void print() const{
        for(const auto& set_pointer : internals.first){
            set_pointer->print();
            std::cout<<std::endl;
        }
    }
    State find(ItemSet<g> start_state) const{
        const ItemSet<g>* current = nullptr;
        for(auto& p : internals.first){
            if(*p == start_state){
                current = p.get();
                break;
            }
        }
        return current;
    }
    State shift(State start, Type next_type) const{
        assert(internals.second.find(start) != internals.second.end());
        if(internals.second.at(start).find(next_type) == internals.second.at(start).end()){
            return nullptr;
        }else{
            return internals.second.at(start).at(next_type);
        }
    }
    const Item<g>* reduce(State start, Type next_type) const{
        for(const auto& map_pair : start->items){
            if(map_pair.first.at_end() && map_pair.second.find(next_type) != map_pair.second.end()){
                return &map_pair.first;
            }
        }
        return nullptr;
    }
    //Returns true if successfully LALR(1) parses the input
    template <typename Iterator>
    bool parses(Iterator start, Iterator end);
    private:
    //Pair of the set of items and the map for transitions
    const std::pair<std::set<std::unique_ptr<const ItemSet<g>>>,std::map<State,std::map<Type,State>>> internals;

    static auto generate_item_sets();
};
} //namespace parser
#endif //_PARSER_
