#include <iostream>
#include <array>
#include <tuple>
#include <set>
#include <utility>
#include <cassert>
#include <climits>

enum class MoveType{ToHall, ToRoom};
class Position{
    public:
        //For part 1
        //const static int room_depth = 2;
        //Part 2
        const static int room_depth = 4;
        static int current_min;

        Position(){
            //Hard coded puzzle input
            //The top of each column comes first
            /*
            #############
            #...........#
            ###D#A#C#D###
              #D#C#B#A#
              #D#B#A#C#
              #C#A#B#B#
              #########
            */
            hall_spaces = std::array<char,7>{'.','.','.','.','.','.','.'};
            rooms = std::array<std::array<char, room_depth>,4>{{
                {'D','D','D','C'},
                {'A','C','B','A'},
                {'C','B','A','B'},
                {'D','A','C','B'}
                //Input to part one
                /*{'D','C'},
                {'A','A'},
                {'C','B'},
                {'D','B'}*/
            }};
            energy = 0;
        }
        Position(Position& prev, MoveType type, int room_num, int hall_pos) : Position(prev){
            this->apply_move(type, room_num, hall_pos);
        }
        std::set<Position> generate_moves(){
            this->move_in_if_possible();
            return move_out();
        }
        bool done(){
            for(int i=0; i<4; i++){
                for(int j=0; j<room_depth; j++){
                    if(rooms[i][j] != static_cast<char>('A'+i)){
                        return false;
                    }
                }
            }
            return true;
        }
        int get_energy(){
            return energy;
        }
        void print(){
            std::cout<<"Energy : "<<get_energy()<<std::endl;
            std::cout<<"#############\n";
            std::cout<<"#"<<hall_spaces[0]<<hall_spaces[1];
            for(int i=0; i<3; i++){
                std::cout<<'.'<<hall_spaces[2+i];
            }
            std::cout<<'.'<<hall_spaces[5]<<hall_spaces[6]<<"#\n";
            std::cout<<"##";
            for(int j=0; j<4; j++){
                std::cout<<"#"<<rooms[j][0];
            }
            std::cout<<"###\n";
            for(int i=1; i<room_depth; i++){
                std::cout<<"  ";
                for(int j=0; j<4; j++){
                    std::cout<<"#"<<rooms[j][i];
                }
                std::cout<<"#\n";
            }
            std::cout<<"  #########  \n"<<std::endl;;
        }
        bool operator<(const Position& rhs) const{
            return std::tie(hall_spaces,rooms,energy) < std::tie(rhs.hall_spaces,rhs.rooms,rhs.energy);
        }
    private:
        std::array<char,7> hall_spaces = {'.','.','.','.','.','.','.'};
        std::array<std::array<char, room_depth>,4> rooms;
        int energy;

        void apply_move(MoveType type, int room_num, int hall_pos){
            switch(type){
                case MoveType::ToHall:
                {
                    int pos = 0;
                    while(rooms[room_num][pos] == '.'){
                        pos++;
                    }
                    assert(pos < room_depth);
                    char c = rooms[room_num][pos];
                    rooms[room_num][pos] = '.';
                    assert(hall_spaces[hall_pos] == '.');
                    hall_spaces[hall_pos] = c;
                    energy += calculate_cost(c, room_num, pos, hall_pos);
                }
                    break;
                case MoveType::ToRoom:
                {
                    int pos = room_depth - 1;
                    while(rooms[room_num][pos] != '.'){
                        pos--;
                    }
                    assert(pos >= 0);
                    char c = hall_spaces[hall_pos];
                    hall_spaces[hall_pos] = '.';
                    rooms[room_num][pos] = c;
                    energy += calculate_cost(c, room_num, pos, hall_pos);
                }
                    break;
            }
        }
        void move_in_if_possible(){
            //Generate moves from a room to hallway
            for(int room_num = 0; room_num < 4; room_num++){
                if(only_correct(room_num)){
                    //Generate moves from a hallway to room
                    //Hall positions to the left of the room
                    for(int hall_pos = room_num + 1; hall_pos>=0; hall_pos--){
                        if(hall_spaces[hall_pos] == static_cast<char>('A' + room_num)){
                            this->apply_move(MoveType::ToRoom, room_num, hall_pos);
                            return move_in_if_possible();
                        }
                        if(hall_spaces[hall_pos] != '.'){
                            break;
                        }
                    }
                    //Hall positions to the right of the room
                    for(int hall_pos = room_num + 2; hall_pos < 7; hall_pos++){
                        if(hall_spaces[hall_pos] == static_cast<char>('A' + room_num)){
                            this->apply_move(MoveType::ToRoom, room_num, hall_pos);
                            return move_in_if_possible();
                        }
                        if(hall_spaces[hall_pos] != '.'){
                            break;
                        }
                    }
                }
            }
            if(done() && this->energy <= current_min){
                current_min = this->energy;
            }
            return;
        }
        std::set<Position> move_out(){
            auto new_positions = std::set<Position>();
            for(int room_num = 0; room_num < 4; room_num++){
                if(!only_correct(room_num)){
                    //Hall positions to the left of the room
                    for(int hall_pos = room_num + 1; hall_pos>=0; hall_pos--){
                        if(hall_spaces[hall_pos] == '.'){
                            auto next = Position(*this, MoveType::ToHall, room_num, hall_pos);
                            next.move_in_if_possible();
                            new_positions.insert(next);
                        }else{
                            break;
                        }
                    }
                    //Hall positions to the right of the room
                    for(int hall_pos = room_num + 2; hall_pos < 7; hall_pos++){
                        if(hall_spaces[hall_pos] == '.'){
                            auto next = Position(*this, MoveType::ToHall, room_num, hall_pos);
                            next.move_in_if_possible();
                            new_positions.insert(next);
                        }else{
                            break;
                        }
                    }
                }
            }
            return new_positions;
        }
        bool only_correct(int room_number){//Returns whether or not only the correct type of amphipods are in the room
            for(int pos = 0; pos < room_depth; pos++){
                char c = rooms[room_number][pos];
                if(c != '.' && c != static_cast<char>(room_number + 'A')){
                    return false;
                }
            }
            return true;
        }
        int calculate_cost(char c, int room_num, int pos, int hall_pos){
            int scale = 1;
            for(int i = 'A'; i < c; i++){
                scale *= 10;
            }
            //Vertical distance from room spot to connection spot
            int distance = pos + 1;
            if(room_num + 1 >= hall_pos){ //to the left
                distance = distance + 2*room_num - 2* hall_pos + 3;
                if(hall_pos == 0){
                    distance--;
                }
            }else{//to the right
                distance = distance + 2*hall_pos - 2* room_num - 3;
                if(hall_pos == 6){
                    distance--;
                }
            }
            return scale * distance;
        }

};

void compute_energies(Position p){
    if(Position::current_min < p.get_energy()){
        return;
    }
    auto next = p.generate_moves();
    for(auto& x : next){
        compute_energies(x);
    }
}
int Position::current_min = INT_MAX;
int main(){
    auto starting_pos = Position();
    compute_energies(starting_pos);
    std::cout<<Position::current_min<<std::endl;
    //Hard coded solution to part 2 initially found by hand
    /*starting_pos = Position(starting_pos,MoveType::ToHall, 1,6);
    starting_pos = Position(starting_pos,MoveType::ToHall, 1,5);
    starting_pos = Position(starting_pos,MoveType::ToHall, 1,3);
    starting_pos = Position(starting_pos,MoveType::ToHall, 1,0);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 1,3);*/

    /*starting_pos = Position(starting_pos,MoveType::ToHall, 2,4);
    starting_pos = Position(starting_pos,MoveType::ToHall, 2,3);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 1,3);
    starting_pos = Position(starting_pos,MoveType::ToHall, 2,1);
    starting_pos = Position(starting_pos,MoveType::ToHall, 2,3);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 1,3);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 2,4);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 2,5);*/


    /*starting_pos = Position(starting_pos,MoveType::ToHall, 3,2);
    starting_pos = Position(starting_pos,MoveType::ToHall, 3,5);
    starting_pos = Position(starting_pos,MoveType::ToHall, 3,4);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 2,4);
    starting_pos = Position(starting_pos,MoveType::ToHall, 3,3);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 1,3);
    starting_pos = Position(starting_pos,MoveType::ToRoom, 3,2);*/
}

