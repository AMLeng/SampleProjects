#include <bitset>
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <cctype>
#include <algorithm>


typedef std::bitset<7> Digit;
typedef std::vector<Digit> UnorderedDigits;
typedef std::array<Digit, 10> OrderedDisplay;

//We store a digit in terms of which segments are lit up when it shows
//Without actually knowing what digit it is
//If we call this method on something that's not supposed to be a digit
//E.g. whitespace, then we'll just get an empty digit
Digit make_digit(std::string input){
    auto digit = std::bitset<7>{};
    for(char c : input){
        if(c >= 'a' && c <= 'g'){
            digit[c-'a'] = true;
        }
    }
    return digit;
}

UnorderedDigits read_digits(std::string segments){
    auto s = std::stringstream(segments);
    auto digits = std::vector<Digit>{};
    std::string next_digit;
    while(std::getline(s,next_digit, ' ')){
        //Strip out whitespace
        next_digit.erase(std::remove_if(next_digit.begin(),next_digit.end(),[](char c){return std::isspace(c);}),next_digit.end());
        if(next_digit.size() == 0){continue;}
        digits.emplace_back(make_digit(next_digit));
    }
    return digits;
}

//Given that to_find and known_reference fill all 7 segments,
//Finds the appropriate Digit in unknowns, removing it from unknowns
//And adding it to partly_known[to_find]
void unknown_from_known(OrderedDisplay& partly_known, UnorderedDigits& unknowns, int to_find, int segments, int known_reference){
    for(int i=0; i<unknowns.size();i++){
        if(unknowns[i].count () == segments && (unknowns[i] | partly_known[known_reference]).all()){
            partly_known[to_find] = unknowns[i];
            unknowns.erase(unknowns.begin() + i );
            break;
        }
    }
}

OrderedDisplay compute_digits(UnorderedDigits input){
    assert(input.size() == 10);
    OrderedDisplay ordered = std::array<Digit, 10>{};
    //We compute the digits in the order:
    //1,7,4,8
    for(int i=0; i<input.size();){
        switch(input[i].count()){
            case 2:
                ordered[1] = input[i];
                input.erase(input.begin() + i );
                break;
            case 3:
                ordered[7] = input[i];
                input.erase(input.begin() + i );
                break;
            case 4:
                ordered[4] = input[i];
                input.erase(input.begin() + i );
                break;
            case 7:
                ordered[8] = input[i];
                input.erase(input.begin() + i );
                break;
            default:
                i++;
                break;
        }
    }
    //2 (using that we know 4)
    unknown_from_known(ordered, input, 2, 5, 4);
    //5 (using that we know 2)
    unknown_from_known(ordered, input, 5, 5, 2);
    //3 (using that there's only one more unknown with 5 digits)
    unknown_from_known(ordered, input, 3, 5, 8);
    //6 (using 1)
    unknown_from_known(ordered, input, 6, 6, 1);
    //0 (using 5)
    unknown_from_known(ordered, input, 0, 6, 5);
    //9 (using that it's the only one left)
    unknown_from_known(ordered, input, 9, 6, 8);
    return ordered;
}

int main(){
    std::string nextline;
    int digitcounts = 0;
    int outputsum = 0;
    while(std::getline(std::cin, nextline, '|')){
        //Read in signal patterns
        auto all_ten_digits = read_digits(nextline);
        auto ordered = compute_digits(all_ten_digits);
        //Read in output digits
        std::getline(std::cin, nextline);
        auto output_digits = read_digits(nextline);
        for(Digit testdigit : output_digits){
            for(int i : {1,4,7,8}){
                if(ordered[i] == testdigit){
                    digitcounts++;
                }
            }
        }
        int scale = 1000;
        for(int i=0; i< 4; i++, scale = scale/10){
            for(int j=0; j<10; j++){
                if(ordered[j] == output_digits[i]){
                    outputsum += scale*j;
                }
            }
        }
    }
    std::cout<<"The answer to part 1 is " << digitcounts <<std::endl;
    std::cout<<"The answer to part 2 is " << outputsum <<std::endl;
}
