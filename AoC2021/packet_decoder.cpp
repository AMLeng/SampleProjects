#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <climits>

int integer_value(const std::vector<bool>& input, int start_index, int length){
    int output = 0;
    for(int i=0; i<length; i++){
        output = output<<1;
        output += input[start_index+i];
    }
    return output;
}
class Packet{
    public:
    explicit Packet(std::vector<bool> input){
        version = integer_value(input,0,3);
        type_id = integer_value(input,3,3);
    }
    Packet(const Packet&) = delete;
    int size(){
        return length;
    }
    static std::unique_ptr<Packet> make_packet(const std::vector<bool>& input);
    virtual int version_sum() = 0;
    virtual long packet_value() = 0;
    protected:
    int length;
    int version;
    int type_id;
};

class Literal : public Packet{
    public:
    explicit Literal(const std::vector<bool>& input) : Packet(input), value(0){
        length = 6;
        int i=1;//Since the first bit of data will be at index 6
        do{
            length += 5;
            i += 5;
            value = value<<4;
            value += integer_value(input,i+1,4);
        }while(input[i] == 1);

    }
    int version_sum() override{
        return version;
    }
    long packet_value() override{
        return value;
    }
    private:
    long value;
};

class Operator : public Packet{
    public:
    explicit Operator(const std::vector<bool>& input) : Packet(input), subpackets(), length_type(input[6]){
        if(length_type == 0){
            length = 22;
            int packet_total_length = 22 + integer_value(input, 7, 15);
            while(length < packet_total_length){
                subpackets.emplace_back(make_packet(std::vector<bool>(input.begin() + length,input.end())));
                length += subpackets.back()->size();
            }
        }else{
            int number_of_packets = integer_value(input, 7, 11);
            length = 18;//Length of the package so far
            for(int i=0; i<number_of_packets; i++){
                subpackets.emplace_back(make_packet(std::vector<bool>(input.begin() + length,input.end())));
                length += subpackets.back()->size();
            }
        }
    }
    int version_sum() override{
        int sum = version;
        for(auto& subpacket : subpackets){
            sum += subpacket->version_sum();
        }
        return sum;
    }
    virtual long packet_value() = 0;
    static std::unique_ptr<Operator> make_operator(const std::vector<bool>& input);
    protected:
    std::vector<std::unique_ptr<Packet>> subpackets;
    private:
    bool length_type;
};

class Sum : public Operator{
    public:
    Sum(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        long sum = 0;
        for(auto& subpacket : subpackets){
            sum += subpacket->packet_value();
        }
        return sum;
    }
};
class Prod : public Operator{
    public:
    Prod(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        long prod = 1;
        for(auto& subpacket : subpackets){
            prod *= subpacket->packet_value();
        }
        return prod;
    }
};
class Min : public Operator{
    public:
    Min(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        long min = LONG_MAX;
        for(auto& subpacket : subpackets){
            long subpacket_value = subpacket->packet_value();
            if(min > subpacket_value){
                min = subpacket_value;
            }
        }
        return min;
    }
};
class Max : public Operator{
    public:
    Max(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        long max = LONG_MIN;
        for(auto& subpacket : subpackets){
            long subpacket_value = subpacket->packet_value();
            if(max < subpacket_value){
                max = subpacket_value;
            }
        }
        return max;
    }
};
class Greater : public Operator{
    public:
    Greater(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        return subpackets.front()->packet_value() > subpackets.back()->packet_value();
    }
};
class Less : public Operator{
    public:
    Less(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        return subpackets.front()->packet_value() < subpackets.back()->packet_value();
    }
};
class Equal : public Operator{
    public:
    Equal(const std::vector<bool>& input) : Operator(input){}
    long packet_value() override{
        return subpackets.front()->packet_value() == subpackets.back()->packet_value();
    }
};

std::unique_ptr<Operator> Operator::make_operator(const std::vector<bool>& input){
    int type = integer_value(input,3,3);
    switch(type){
        case 0:
            return std::make_unique<Sum>(input);
        case 1:
            return std::make_unique<Prod>(input);
        case 2:
            return std::make_unique<Min>(input);
        case 3:
            return std::make_unique<Max>(input);
        case 5:
            return std::make_unique<Greater>(input);
        case 6:
            return std::make_unique<Less>(input);
        case 7:
            return std::make_unique<Equal>(input);
        default:
            std::cout<<"Looks like there's a case missing"<<std::endl;
            return std::make_unique<Sum>(input);
    }
}
std::unique_ptr<Packet> Packet::make_packet(const std::vector<bool>& input){
    int type = integer_value(input,3,3);
    if(type == 4){
        return std::make_unique<Literal>(input);
    }else{
        return Operator::make_operator(input);
    }
}

std::vector<bool> hex_to_bits(const std::string hex_input){
    const std::map<char, std::vector<bool>> bit_value = {
        {'0',{0,0,0,0}},{'1',{0,0,0,1}},{'2',{0,0,1,0}},{'3',{0,0,1,1}},
        {'4',{0,1,0,0}},{'5',{0,1,0,1}},{'6',{0,1,1,0}},{'7',{0,1,1,1}},
        {'8',{1,0,0,0}},{'9',{1,0,0,1}},{'A',{1,0,1,0}},{'B',{1,0,1,1}},
        {'C',{1,1,0,0}},{'D',{1,1,0,1}},{'E',{1,1,1,0}},{'F',{1,1,1,1}}
        };
    auto bits = std::vector<bool>();
    for(char c : hex_input){
        auto bit_values = bit_value.at(c);
        for(bool b : bit_values){
            bits.emplace_back(b);
        }
    }
    return bits;

}
int main(){
    std::string input_str;
    std::getline(std::cin, input_str);
    if(input_str.back() == '\r'){
        input_str.pop_back();
    }
    std::vector<bool> input = hex_to_bits(input_str);
    std::unique_ptr<Packet> outer_packet = Packet::make_packet(input);
    std::cout<<"The sum of versions is "<<outer_packet->version_sum()<<std::endl;
    std::cout<<"The packet value is "<<outer_packet->packet_value()<<std::endl;
}
