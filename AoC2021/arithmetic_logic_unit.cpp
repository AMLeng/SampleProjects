#include <iostream>
#include <cctype>
#include <cassert>
#include <array>
#include <string>

class BlockInputs{
    public:
        BlockInputs(bool div26_z, int add_x, int add_y):
            div26_z(div26_z), add_x(add_x), add_y(add_y)
        {}
        bool div26_z;
        int add_x;
        int add_y;
};

//After analyzing the input by hand, we have 14 almost identical blocks with only 4 differences in the code,
//Plus one additional difference from the different possible input values
void evaluate_input(long long& z, BlockInputs i, int input_w){
    int w = input_w;
    int x = (z%26L);
    int y = 25;
    if(i.div26_z){
        z = z/26; //div z 26; this is the only step that can possibly make z *smaller*
    }
    x += i.add_x; //negative if div26_z is true, >=10 otherwise
    x = 1-(x == w); 
    y = (y*x)+1; //either y = 1 or y = 26
    z *= y; //mul z y
    y = w + i.add_y; //add_y is always positive, at most 16, so this is at most 25
    y *= x;
    z += y;
}

class ModelNum{
    public:
        ModelNum(long long x) : num(x){
        }
        int at(int index){
            long long scale = 1L;
            for(int i=index; i<13; i++){
                scale *= 10L;
            }
            return static_cast<int>((num % (10L*scale))/scale);
        }
        void print(){
            std::cout<<num;
        }
    private:
        long long num;
};
bool check_valid(ModelNum m, std::array<BlockInputs, 14> inputs){
    long long z = 0;
    std::cout<<"Value of z in base 26 at each step, least significant first"<<std::endl;
    for(int i=0; i<14; i++){
        evaluate_input(z,inputs.at(i),m.at(i));
        std::cout<<"Step "<<i<<": ";
        for(long long temp = z; temp > 0; temp = temp/26){
            std::cout<<temp%26<<", ";
        }
        std::cout<<std::endl;
    }
    m.print();
    std::cout<<" -> "<<z<<std::endl;
    return (z == 0L);
}

int main(){
    std::array<BlockInputs, 14> inputs = {{
        {false, 15,4}, //0
        {false, 14,16}, //1
        {false,11,14}, //2
        {true, -13, 3}, //3
        {false, 14,11}, //4
        {false, 15,13}, //5
        {true, -7, 11}, //6
        {false, 10, 7}, //7
        {true, -12, 12}, //8
        {false, 15,15}, //9
        {true, -16, 13}, //10
        {true, -9, 1}, //11
        {true, -8, 15}, //12
        {true, -8, 4} //13
    }};
    //Since the x input is always >= 10 if div26 is false, 
    //On steps 0,1,2,4,5,7,9, z will always get an extra "digit"
    //Since there are 7 of these steps, we need the 7 steps where div26 is true
    //To be able to remove a digit
    //We pair up the steps, (2,3), (5,6), (7,8), (9,10), (4,11), (1,12), (0,13)
    //Where for a pair (a,b) we must have, b.digit = a.digit + a.add_y +b.add_x
    //This lets us compute
    auto m = ModelNum(91897399498995L);
    check_valid(m,inputs);
    std::cout<<std::endl;
    m = ModelNum(51121176121391L);
    check_valid(m,inputs);
}
