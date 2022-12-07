#include <iostream>
#include <string>
#include <memory>
#include <cassert>
#include <vector>

class Object{
    public:
    virtual int size() = 0;
};
class File : public Object{
    public:
    File(int s) : Object(), filesize(s){}
    int size() override{
        return filesize;
    }
    private:
    int filesize;
};

class Dir : public Object{
    public:
        Dir() : Object(), subobjects(), parent(nullptr), name("/") {}
        Dir(std::string n, Dir* p) : subobjects(), parent(p), name(n) {}
        void add_object(std::string nextobj){
            assert(this);
            if(nextobj[0] == 'd'){
                auto x = std::make_unique<Dir>(nextobj.substr(4),this);
                subobjects.push_back(std::move(x));
            }else{
                subobjects.push_back(std::move(std::make_unique<File>(std::stoi(nextobj))));
            }
        }
        Dir* find(std::string subdir){
            for(std::unique_ptr<Object>& p_o : subobjects){
                Dir* p = dynamic_cast<Dir*>(p_o.get());
                if(p && p->name == subdir){
                    return p;
                }
            }
            return nullptr;
        }
        int size() override{
            int sum=0;
            for(std::unique_ptr<Object>& p_o : subobjects){
                if(p_o){
                    sum += p_o->size();
                }
            }
            return sum;
        }
        int part_1(){
            int sum = this->size();
            sum = sum > 100000 ? 0 : sum;
            for(std::unique_ptr<Object>& p_o : subobjects){
                auto p = dynamic_cast<Dir*>(p_o.get());
                if(p){
                    sum += p->part_1();
                }
            }
            return sum;
        }
        int part_2(int thresh){
            int current_best = this->size();
            if(current_best < thresh){return -1;}
            for(std::unique_ptr<Object>& p_o : subobjects){
                auto p = dynamic_cast<Dir*>(p_o.get());
                if(p){
                    int n = p->part_2(thresh);
                    if(n > 0 && n<current_best){
                        current_best = n;
                    }
                }
            }
            return current_best;
        }
    Dir* parent;
    std::vector<std::unique_ptr<Object>> subobjects;
    std::string name;
    private:
};

int main(){
    std::string nextline;
    auto homedir = Dir();
    Dir* currentdir = &homedir;
    std::getline(std::cin, nextline);
    while(std::getline(std::cin,nextline)){
        assert(nextline[0] == '$');
        if(nextline[2] == 'l'){
            do{
                if(std::cin.eof()){break;}
                std::getline(std::cin, nextline);
                currentdir->add_object(nextline);
            }while(std::cin.peek() != '$');
        }else{
            if(nextline[2] == 'c'){
                if(nextline[5] == '.'){
                    currentdir = currentdir->parent;
                    assert(currentdir);
                }else{
                    currentdir = currentdir->find(nextline.substr(5));
                    assert(currentdir);
                }
            }else{
                assert(false);
            }
        }
    }
    std::cout<<homedir.part_1()<<std::endl;
    int size = homedir.size();
    const int TOTAL = 70000000;
    const int NEED = 30000000;
    const int threshold = NEED-(TOTAL-size);
    std::cout<<homedir.part_2(threshold)<<std::endl;
}
