#ifndef _FSM_
#define _FSM_
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
namespace fsm{

typedef std::string Type;
template <const grammar::Grammar* g>
class ItemSet;
template <const grammar::Grammar* g>
class Item{
    friend ItemSet<g>;
    public:
    Item(Type lhs, std::vector<Type> rhs) : left(lhs), right(rhs), position(0) {
        }
    bool next(Type input) const{
        return position < right.size() && right.at(position) == input;
    }
    Item<g> shift() const{
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
class ItemSet{
    public:
    ItemSet(std::set<Item<g>> kernel) : items(kernel){
        this->closure();
    }
    bool operator<(const ItemSet<g>& rhs) const{
        return lexicographical_compare(items.begin(),items.end(), rhs.items.begin(), rhs.items.end());
    }
    bool operator==(const ItemSet<g>& rhs) const{
        return !(*this < rhs) && !(rhs < *this);
    }
    ItemSet<g> shift(Type next_type) const{
        auto items = std::set<Item<g>>();
        for(const auto& prev_item : this->items){
            if(prev_item.next(next_type)){
                items.insert(prev_item.shift());
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
    std::set<Item<g>> items;
    void closure(){
        auto added = std::map<Type,bool>();
        auto to_add = std::map<Type,bool>();
        for(auto pair : g->derivations){
            added.emplace(pair.first,false);
            to_add.emplace(pair.first,false);
        }
        for(Item<g> i : items){
            if(i.position < i.right.size()){
                auto next_type = i.right.at(i.position);
                if(to_add.find(next_type) != to_add.end()){
                    to_add.at(next_type) = true;
                }
            }
        }
        while(added != to_add){
            for(auto pair : to_add){
                if(pair.second && !added.at(pair.first)){
                    for(auto new_derivation : g->derivations.at(pair.first)){
                        items.emplace(pair.first,new_derivation);
                        auto possible_next = new_derivation.at(0);
                        if(to_add.find(possible_next) != to_add.end()){
                            to_add.at(possible_next) = true;
                        }
                    }
                    added.at(pair.first) = true;
                }
            }
        }

    }
};


template <const grammar::Grammar* g>
class FSM{
    public:
    FSM() : transitions(generate_item_sets().second), item_sets(generate_item_sets().first) {
    }
    FSM(FSM&) = delete;
    FSM(FSM&&) = delete;
    //const ItemSet<g>* eval(ItemSet<g> start_state, SOME ITERATOR NONSENSE)
    const std::set<std::unique_ptr<const ItemSet<g>>> item_sets;
    const std::map<const ItemSet<g>*,std::map<Type,const ItemSet<g>*>> transitions;
    private:
    //const ItemSet<g>* transition(const ItemSet<g>* start, Type next_type) const{}
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
};
} //namespace fsm
#endif //_FSM_
