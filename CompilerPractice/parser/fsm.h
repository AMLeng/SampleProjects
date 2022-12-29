#ifndef _FSM_
#define _FSM_
#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
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
    Item<g> shift(Type input) const{
        assert(right.at(position) == input);
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
    void print(){
        for(auto i : items){
            std::cout<<"Item: ";
            i.print();
        }
    }
    int size(){
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
} //namespace fsm
#endif //_FSM_
