#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <cctype>
#include <algorithm>
#include <memory>

//Would run slightly faster if, instead of using inheritance
//Just had a single "List" class with an int value field
//And let value = -1 for a "List", and value non-negative for a "Num"
int match_paren(std::string& s, int i){
    assert(s[i] == '[');
    int j=1;
    int k = 1;
    while(j>0){
        if(s[i+k] == '['){
            j++;
        }
        if(s[i+k] == ']'){
            j--;
        }
        k++;
    }
    return i+k;
}
class Item{
    public:
    virtual int compare(const Item& right) const = 0;
    virtual bool comp(const Item& right) const{
        return this->compare(right) >= 0;
    }
};

class Num : public Item{
    public:
        Num(int i) : value(i) {}
        Num(std::string line) : value(std::stoi(line)) {}
        int compare(const Item& right) const override{
            const Num* r = dynamic_cast<const Num*>(&right);
            if(r){
                if(value < r->value){
                    return 1;
                }
                if(value > r->value){
                    return -1;
                }
                return 0;
            }else{
                return -(right.compare(*this));
            }
        }
    private:
        int value;
};

class List : public Item{
    public:
    List(std::string s){
        assert(s.front() == '[');
        assert(s.back() == ']');
        s.erase(0,1);
        s.pop_back();
        while(s.size() > 0){
            if(std::isdigit(s[0])){
                int f = s.find(',');
                auto sub = s.substr(0,f);
                elements.push_back(std::make_unique<Num>(sub));
                s.erase(0,f);
            }else{
                int m = match_paren(s,0);
                auto sub = s.substr(0,m);
                assert(sub.back() == ']');
                elements.push_back(std::make_unique<List>(sub));
                s.erase(0,m);
            }
            if(s.front() ==','){
                s.erase(0,1);
            }
        }
    }
    List(Num n){
        elements.push_back(std::make_unique<Num>(std::move(n)));
    }
    List(List& other) = delete;
    List& operator=(List& other) = delete;
    List(List&& other) = default;
    List& operator=(List&& other) = default;
    int compare(const Item& right) const override{
        const List* r = nullptr;
        std::unique_ptr<const List> ptr = nullptr;
        if(dynamic_cast<const Num*>(&right)){
            ptr = std::make_unique<const List>(*dynamic_cast<const Num*>(&right));
            r = ptr.get();
        }else{
            r = dynamic_cast<const List*>(&right);
        }
        for(int i=0; i< elements.size(); i++){
            if(r->elements.size() <= i){return -1;}
            int comp = elements[i]->compare(*(r->elements[i]));
            if(comp != 0){return comp;}
        }
        if(elements.size() == r->elements.size()){
            return 0;
        }else{
            return 1;
        }
    }
    private:
    std::vector<std::unique_ptr<Item>> elements;

};

int decoder_key(std::vector<List>& lists){
    int a;
    int b;
    const auto two = List("[[2]]");
    const auto six = List("[[6]]");
    int n = lists.size();
    for(int i=0; i<n; i++){
        if(two.compare(lists[i]) == 0){
            a = i+1;
        }
        if(six.compare(lists[i]) == 0){
            b = i+1;
        }
    }
    return a*b;
}
int main(){
    std::string nextline;
    int sum=0;
    int pairindex = 1;
    auto lines = std::vector<List>();
    lines.emplace_back("[[2]]");
    lines.emplace_back("[[6]]");
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        lines.emplace_back(nextline);
        auto l1 = List(nextline);
        std::getline(std::cin,nextline);
        if(nextline.back() == '\r'){nextline.pop_back();}
        lines.emplace_back(nextline);
        auto l2 = List(nextline);
        
        if(l1.compare(l2)>0){
            sum+= pairindex;
        }
        std::getline(std::cin,nextline);
        pairindex++;
    }
    std::cout<<sum<<std::endl;

    std::sort(lines.begin(),lines.end(),[](List& a, List& b) {return a.comp(b);});
    std::cout<<decoder_key(lines)<<std::endl;
}
