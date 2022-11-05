#include <iostream>
#include <string>
#include <map>
#include <utility>

namespace dive{
    const std::map<std::string, std::pair<int,int> > commands = 
        {{"forward",std::pair<int,int>(1,0)}, 
        {"down",std::pair<int,int>(0,1)}, 
        {"up",std::pair<int,int>(0,-1)}};

    int dive_one(std::istream& input_stream){
        std::string next_command;
        int horizontal = 0;
        int depth = 0;
        int magnitude = 0;
        while(std::cin >> next_command){
            std::cin >> magnitude;
            horizontal += commands.at(next_command).first * magnitude;
            depth += commands.at(next_command).second * magnitude;
        }
        return horizontal*depth;
    }
    int dive_two(std::istream& input_stream){
        std::string next_command;
        int aim = 0;
        int horizontal = 0;
        int depth = 0;
        int magnitude = 0;
        while(std::cin >> next_command){
            std::cin >> magnitude;
            aim += commands.at(next_command).second * magnitude;
            horizontal += commands.at(next_command).first * magnitude;
            depth += commands.at(next_command).first * magnitude * aim;
        }
        return horizontal*depth;
    }

} //namespace dive

int main(){
    //int result = dive::dive_one(std::cin);
    int result = dive::dive_two(std::cin);
    std::cout << result << std::endl;

}
