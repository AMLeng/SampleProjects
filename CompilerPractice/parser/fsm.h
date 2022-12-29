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
class ItemSet;
class Item{
    friend ItemSet;
    public:
    Item(const grammar::Grammar* gp, Type lhs, std::vector<Type> rhs) : g(gp), left(lhs), right(rhs), position(0) {}
    bool operator<(const Item& rhs) const{
        if(this->left < rhs.left){
            return true;
        }
        if(this->right < rhs.right){
            return true;
        }
        if(this->position < rhs.position){
            return true;
        }
        return false;
    }
    void print(){
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
    const grammar::Grammar* g;
    Type left;
    std::vector<Type> right;
    int position;
};
class ItemSet{
    public:
    ItemSet(std::set<Item> kernel) : items(kernel){
        assert(kernel.size() > 0);
        g = kernel.begin()->g;
        this->closure();
    }
    bool operator<(const ItemSet& rhs) const{
        return lexicographical_compare(items.begin(),items.end(), rhs.items.begin(), rhs.items.end());
    }
    void print(){
        for(auto i : items){
            i.print();
        }
    }
    private:
    const grammar::Grammar* g;
    std::set<Item> items;
    void closure(){
        auto added = std::map<Type,bool>();
        auto to_add = std::map<Type,bool>();
        for(auto pair : g->derivations){
            added.emplace(pair.first,false);
            to_add.emplace(pair.first,false);
        }
        for(Item i : items){
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
                        items.emplace(g,pair.first,new_derivation);
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
