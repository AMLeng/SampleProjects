#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <optional>


//Must compile with c++17 onwards in order to use "optional"
class Cuboid{
    public:
    Cuboid(std::string input){
        std::string sub = input.substr(input.find("x=")+2);
        minx = std::stoi(sub);
        sub = sub.substr(sub.find("..")+2);
        maxx = std::stoi(sub);
        sub = input.substr(input.find("y=")+2);
        miny = std::stoi(sub);
        sub = sub.substr(sub.find("..")+2);
        maxy = std::stoi(sub);
        sub = input.substr(input.find("z=")+2);
        minz = std::stoi(sub);
        sub = sub.substr(sub.find("..")+2);
        maxz = std::stoi(sub);
    }
    static std::optional<Cuboid> construct(int a1, int a2, int b1, int b2, int c1, int c2){
        if(a2 >= a1 && b2 >= b1 && c2 >= c1){
            return std::optional<Cuboid>(Cuboid(a1,a2,b1,b2,c1,c2));
        }else{
            return std::optional<Cuboid>();
        }
    }
    bool contains(int a, int b, int c){
        return a >= minx &&
        a <= maxx &&
        b >= miny &&
        b <= maxy &&
        c >= minz &&
        c <= maxz;
    }
    std::optional<Cuboid> intersect(const Cuboid& second) const{
        return construct(std::max(minx,second.minx),std::min(maxx,second.maxx),std::max(miny,second.miny),std::min(maxy,second.maxy),std::max(minz,second.minz),std::min(maxz,second.maxz));
    }
    bool check_intersect(const Cuboid& second) const{
        return intersect(second).has_value();
    }
    long size() const{
        long prod = maxx-minx+1;
        prod *= maxy - miny + 1;
        prod *= maxz - minz + 1;
        return prod;
    }
    void print() const{
        std::cout<<"x="<<minx<<".."<<maxx<<",y="<<miny<<".."<<maxy<<",z="<<minz<<".."<<maxz;
    }
    private:
    Cuboid(int a1, int a2, int b1, int b2, int c1, int c2) : minx(a1), maxx(a2), miny(b1), maxy(b2), minz(c1), maxz(c2){
        }
    int minx;
    int miny;
    int minz;
    int maxx;
    int maxy;
    int maxz;
};

typedef std::pair<bool,Cuboid> Instruction;

class Region{
    public:
    long count_on() const{
        long sum1 = 0;
        for(Cuboid c : lit){
            sum1 += c.size();
        }
        long sum = 0;
        for(Cuboid c : dark){
            sum -= c.size();
        }

        return sum1 + sum;
    }
    void add(Cuboid c, bool on){
        int n = lit.size();
        int m = dark.size();
        for(int i=0; i<n; i++){
            auto temp = lit[i].intersect(c);
            if(temp){
                dark.push_back(temp.value());
            }
        }
        for(int i=0; i<m; i++){
            auto temp = dark[i].intersect(c);
            if(temp){
                lit.push_back(temp.value());
            }
        }
        if(on){
            lit.push_back(c);
        }
    }
    private:
    std::vector<Cuboid> lit;
    std::vector<Cuboid> dark;
};
int main(){
    std::vector<Instruction> instructions = std::vector<Instruction>();
    std::string nextline;
    while(std::getline(std::cin,nextline)){
        instructions.emplace_back(std::make_pair<bool,Cuboid>(nextline[1] == 'n',nextline));
    }
    
    Cuboid init_region = Cuboid::construct(-50,50,-50,50,-50,50).value();
    auto region = Region();
    for(auto instruct : instructions){
        //if(!init_region.check_intersect(instruct.second)){continue;}
        region.add(instruct.second, instruct.first);
    }
    std::cout<<"Now there are "<<region.count_on()<<" cubes lit"<<std::endl;
}
