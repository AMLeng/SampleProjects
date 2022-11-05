#include <iostream>
#include <string>
#include <climits>
#include <array>

namespace sonar_sweep {
    int sonar_sweep_one(std::istream& input_stream){
        std::string next_line;
        int last_line = INT_MAX;
        int current_line = 0;
        int measurement_increases = 0;
        while(std::getline(input_stream, next_line)){
            current_line = std::stoi(next_line);
            if(current_line > last_line){measurement_increases++;}
            last_line = current_line;
        }
        return measurement_increases;
    }
    int sonar_sweep_two(std::istream& input_stream){
        constexpr int window_size = 3;
        std::string next_line;
        auto old_measurements = std::array<int, window_size>();
        int input_line = 0;
        for(; input_line<window_size; input_line++){
            std::getline(input_stream, next_line);
            old_measurements[input_line] = std::stoi(next_line);
        }
        int new_measurement = 0;
        int measurement_increases = 0;
        while(std::getline(input_stream, next_line)){
            new_measurement = std::stoi(next_line);
            if(new_measurement > old_measurements[input_line%window_size]){measurement_increases++;}
            old_measurements[input_line % window_size] = new_measurement;
            input_line++;
        }
        return measurement_increases;
    }
} //namespace sonar_sweep

int main(int argc, char* argv[]){
    //Debated between having the program call an argument giving the file name of the input
    //Or just reading from standard in
    //And decided that probably it's more correct to read from standard in, since one can easily run 
    //sonar_sweep < input_file_name
    //While reading from stdin opens up more possibilities in general
    if(argc == 2){
        int increases;
        switch (argv[1][0]){
            case '1':
                increases = sonar_sweep::sonar_sweep_one(std::cin);
                std::cout << "There were " << increases <<" increases in the measurements." <<std::endl;
                return 0;
            case '2':
                increases = sonar_sweep::sonar_sweep_two(std::cin);
                std::cout << "There were " << increases <<" increases in the sums." <<std::endl;
                return 0;
            default:
                std::cout<< "That is not a valid argument" <<std::endl;
        }
    }else{
        std::cout << "When running the program, please input '1' to run part 1, and '2' to run part 2." << std::endl;
    }
}
