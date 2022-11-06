#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <string>
#include <algorithm>

namespace binary_diagnostic {
    constexpr unsigned int linelen = 12;
    std::array<int, linelen> count_bits(std::vector<std::string> inputs){
        auto bit_counts = std::array<int, linelen>();
        int next_char = 0;
        for(std:: string s : inputs){
            for(unsigned int i=0; i<linelen; i++){
                if(s[i] == '0'){
                    bit_counts[i]--;
                }else{
                    assert(s[i] == '1');
                    bit_counts[i]++;
                }
            }
        }
        return bit_counts;
    }
    int power_consumption(std::array<int, linelen> bit_counts){
        int epsilon = 0;
        int gamma = 0;
        //Careful, we're working with unsigned ints here so overflow behavior at 0
        for(unsigned int i=0; i<linelen; i++){
            if(bit_counts[i] > 0){
                gamma += 1<<(linelen-i-1);
            }else{
                epsilon += 1<<(linelen-i-1);
            }
        }
        return epsilon*gamma;
    }
    void prune_generator(std::vector<std::string>& inputs, unsigned int position){
        int bit_count = count_bits(inputs).at(position);
        char c = bit_count >= 0 ? '1':'0';
        inputs.erase(std::remove_if(inputs.begin(), inputs.end(), [=](std::string s){
                return s[position] != c;
            }),inputs.end());
    }
    int generator_rating(std::vector<std::string> inputs){
        for(unsigned int i=0; inputs.size() > 1; i++){
            assert(i < linelen);
            prune_generator(inputs,i);
        }
        int rating_number = 0;
        std::string rating = inputs[0];
        for(unsigned int i=0; i<linelen; i++){
            if(rating[i] == '1'){
                rating_number += 1<<(linelen-i-1);
            }
        }
        return rating_number;
    }
    void prune_scrubber(std::vector<std::string>& inputs, unsigned int position){
        int bit_count = count_bits(inputs).at(position);
        char c = bit_count >= 0 ? '0':'1';
        inputs.erase(std::remove_if(inputs.begin(), inputs.end(), [=](std::string s){
                return s[position] != c;
            }),inputs.end());
    }
    int scrubber_rating(std::vector<std::string> inputs){
        for(unsigned int i=0; inputs.size() > 1; i++){
            assert(i < linelen);
            prune_scrubber(inputs,i);
        }
        int rating_number = 0;
        std::string rating = inputs[0];
        for(unsigned int i=0; i<linelen; i++){
            if(rating[i] == '1'){
                rating_number += 1<<(linelen-i-1);
            }
        }
        return rating_number;
    }

    int binary_diagnostic_one(std::vector<std::string> inputs){
        auto bit_counts = count_bits(inputs);
        return power_consumption(bit_counts);
    }
    int binary_diagnostic_two(std::vector<std::string> inputs){
        return scrubber_rating(inputs)*generator_rating(inputs);
    }

} //namespace binary_diagnostic

int main(){
    auto inputs = std::vector<std::string>();
    std::string next_line;
    while(getline(std::cin,next_line)){
        inputs.emplace_back(next_line);
    }
    int support_rating = binary_diagnostic::binary_diagnostic_two(inputs);
    std::cout << support_rating <<std::endl;
    /*int power_consumption = binary_diagnostic::binary_diagnostic_one(inputs);
    std::cout << power_consumption <<std::endl;*/
}
