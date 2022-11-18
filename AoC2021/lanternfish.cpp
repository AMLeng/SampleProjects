#include <iostream>
#include <cctype>
#include <array>

class FishCounts{
    public:
        explicit FishCounts(std::array<long,9> initial) : fish_counts(initial), index(0){}
        long total_fish(){
            long sum = 0;
            for(long num : this->fish_counts){
                sum += num;
            }
            return sum;
        }
        void time_step(){
            fish_counts[(7+index)%9] += fish_counts[index];
            index = (index+1)%9;
        }
    private:
        long index;
        std::array<long,9> fish_counts;
};

int main() {
    auto initial_fish = std::array<long,9>{};
    for(char c = std::cin.get();c != EOF; c=std::cin.get()){
        //Technicallity about how "isdigit" is always supposed to take an unsigned char
        if(std::isdigit(static_cast<unsigned char>(c))){
            initial_fish[c-'0']++;
        }
    }
    auto fish = FishCounts(initial_fish);
    /*for(int j=0; j<80; j++){
        fish.time_step();
    }*/
    for(int j=0; j<256; j++){
        fish.time_step();
    }
    std::cout << fish.total_fish() <<std::endl;
}
