#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

class CrabPos{
    public:
    CrabPos(bool run_part1) : part1(run_part1){
        auto positions = std::vector<int>{};
        }
    //Force us to specify if we're doing part 1 or part 2
    CrabPos() = delete;
    void add_crab(int new_crab){
        positions.emplace_back(new_crab);
    }
    int fuel_needed(){
        int fuel = 0;
        if(part1){
        //For part 1,
        //Observe that if we are above the median, getting smaller will decrease the fuel needed
        //And similarly for if we are below the median
        //So the best position to line up at is the median
            int target_position = median();
            return fuel_needed(target_position);
        }else{
            //For part 2, we're minimizing the sum of the two functions
            // (x_i-x)^2, |x_i-x|; the first is minimized by the average
            // And the second is minimized by the median
            // So the minimizer of the sum is (by convex function theory)
            // In the interval between the median and average
            int a = average();
            int b = median();
            if(a>b){
                int temp = a;
                a=b;
                b=temp;
            }
            int min_cost = fuel_needed(b);
            int temp = 0;
            for(int i=a; i<b; i++){
                temp = fuel_needed(i);
                if(temp < min_cost){
                    min_cost = temp;
                }
            }
            return min_cost;
        }
    }
    private:
    bool part1;
    std::vector<int> positions;
    int median(){
        std::sort(positions.begin(),positions.end());
        return positions[positions.size()/2];
    }
    int average(){
        int sum = 0;
        for(int crab : positions){
            sum += crab;
        }
        if(sum%positions.size() < positions.size()/2){
            return sum/positions.size();
        }else{
            return (sum/positions.size())+1;
        }
    }
    int fuel_needed(int target_position){
        int fuel = 0;
        for(int crab : positions){
            if(target_position > crab){
                if(part1){
                    fuel += target_position - crab;
                }else{
                    fuel += ((target_position - crab)*(target_position - crab + 1))/2;
                }
            }else{
                if(part1){
                    fuel += crab - target_position;
                }else{
                    fuel += ((crab - target_position)*(crab - target_position + 1))/2;
                }
            }
        }
        return fuel;
    }
};
int main() {
    auto crab_positions = CrabPos(false);
    std::string input;
    while(std::getline(std::cin, input, ',')){
        //Should probably do a check here in case the input is not formatted perfectly nicely, but for this case we're fine
        crab_positions.add_crab(std::stoi(input));
    }
    std::cout << crab_positions.fuel_needed() << std::endl;
}
