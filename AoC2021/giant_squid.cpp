#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cctype>
#include <cassert>
namespace giant_squid{
    class Bingo_Board{
        std::array<std::array<bool, 5>, 5> marked;
        std::array<std::array<int, 5>, 5> board;
        //Checks for a bingo in row i or in column j
        bool bingo(int i, int j){
            assert(marked[i][j]);
            bool row = true;
            bool col = true;
            for(int k=0; k<5; k++){
                row &= marked[i][k];
                col &= marked[k][j];
            }
            return row || col;
        }
        public: 
        Bingo_Board(std::array<std::array<int,5>,5> b) : board(b){
            marked.fill({});
        }
        int score(int call){
            int board_score = 0;
            for(int i=0; i<5; i++){
                for(int j=0; j<5; j++){
                    if(!marked[i][j]){
                        board_score += board[i][j];
                    }
                }
            }
            return board_score*call;
        }
        bool call_num(int call){
            for(int i=0; i<5; i++){
                for(int j=0; j<5; j++){
                    if(board[i][j] == call){
                        marked[i][j] = true;
                        return bingo(i,j);
                    }
                }
            }
            return false;
        }
        
    }; //class Bingo_Board

    int giant_squid_one(std::istream& input_stream){
        std::string next_line;
        auto calls = std::vector<int>();
        int c;
        do{
            input_stream >> c;
            calls.emplace_back(c);
        }while(input_stream.get() == ',');
        std::array<std::array<int, 5>, 5> board;
        std::vector<Bingo_Board> boards = {};
        while(std::getline(input_stream, next_line)){
            assert(next_line.size() == 0);
            for(int i=0; i<5; i++){
                for(int j=0; j<5; j++){
                input_stream >> board[i][j];
                }
            }
            boards.emplace_back(std::move(board));
            input_stream.ignore();
        }
        //Now finished reading input
        for(int call: calls){
            for(Bingo_Board &board : boards){
                if(board.call_num(call)){
                    return board.score(call);
                }
            }
        }
        //Should never get here
        return -1;
    }
    int giant_squid_two(std::istream& input_stream){
        std::string next_line;
        auto calls = std::vector<int>();
        int c;
        do{
            input_stream >> c;
            calls.emplace_back(c);
        }while(input_stream.get() == ',');
        std::array<std::array<int, 5>, 5> board;
        std::vector<Bingo_Board> boards = {};
        while(std::getline(input_stream, next_line)){
            assert(next_line.size() == 0);
            for(int i=0; i<5; i++){
                for(int j=0; j<5; j++){
                input_stream >> board[i][j];
                }
            }
            boards.emplace_back(std::move(board));
            input_stream.ignore();
        }
        //Now finished reading input
        for(int call: calls){
            for(auto it = boards.begin(); it != boards.end();){
                if(it->call_num(call)){
                    if(boards.size() > 1){
                        it = boards.erase(it);
                    }else{
                        return it->score(call);
                    }
                }else{
                    it++;
                }
            }
        }
        //Should never get here
        return -1;
    }


} //namespace giant_squid

int main(){
    int result = giant_squid::giant_squid_two(std::cin);
    std::cout << result << std::endl;
}
