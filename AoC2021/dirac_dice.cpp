#include<iostream>
#include <vector>
#include <array>
#include <cassert>

//After a little bit of math, we see that the rolls only matter mod 10,
//And that the ith triple roll will move us ahead by (6-i) mod 10, where we start at 0
void part_one(int pos1, int pos2){
    int pos1m1 = pos1 - 1;//For some arithmetic tricks, store the positions as one less than they actually are
    int pos2m1 =  pos2 - 1;
    int score1 = 0;
    int score2 = 0;
    int rolls = 0;
    int total_rolls = 0;
    while(score2 < 1000){
        pos1m1 = (pos1m1 + 16-rolls)%10;
        score1 += pos1m1+1;
        rolls = (rolls + 1)%10;
        total_rolls+= 3;
        if(score1 >= 1000){
            break;
        }
        pos2m1 = (pos2m1 + 16-rolls)%10;
        score2 += pos2m1+1;
        rolls = (rolls + 1)%10;
        total_rolls+= 3;
    }
    std::cout<<"For part 1, scores are:"<<score1<<','<<score2<<" and there were "<<total_rolls<<" total rolls"<<std::endl;
}

constexpr std::array<std::pair<int,int>,7> possible_t_rolls = {{{3,1},{4,3},{5,6},{6,7},{7,6},{8,3},{9,1}}};

struct History{
    std::vector<int> t_rolls; //Array of indices in [0,6]
    long frequency;
    int pos1m1;
    int pos2m1;
    int score1;
    int score2;
    bool p1_next;
    bool backtrack(){
        int i = t_rolls.back();
        if(i==6){
            bool temp = p1_next;
            while(i==6){
                t_rolls.pop_back();
                //frequency = frequency/posible_t_rolls[6].second; /*(note tt ts is division by 1)*/
                if(p1_next){
                    score2 -= (pos2m1+1);
                    pos2m1 =(pos2m1+1)%10;//Here, 1 = (10-possible_t_rolls[6])%10
                    p1_next = false;
                }else{
                    score1 -= (pos1m1+1);
                    pos1m1 =(pos1m1+1)%10;//Here, 1 = (10-possible_t_rolls[6])%10
                    p1_next = true;
                }
                i = t_rolls.back();
                if(t_rolls.size() == 0){return false;}
            }
            backtrack();
            return true;
        }else{
            frequency = frequency/possible_t_rolls[i].second;
            t_rolls.back()++;
            frequency = frequency*possible_t_rolls[i+1].second;
            if(p1_next){
                score2 -= pos2m1;
                pos2m1 =(pos2m1+1)%10;//Here, 1 = possible_t_rolls[i+1]-possible_t_rolls[i]
                score2 += pos2m1;
            }else{
                score1 -= pos1m1;
                pos1m1 =(pos1m1+1)%10;//Here, 1 = possible_t_rolls[i+1]-possible_t_rolls[i]
                score1 += pos1m1;
            }
            return true;
        }
    }
    void roll(){
        t_rolls.push_back(0);
        /*multiplication by 1*/
        //frequency *= possible_t_rolls[0].second;
        if(p1_next){
            pos1m1 = (pos1m1+possible_t_rolls[0].first)%10;
            score1 += pos1m1+1;
            p1_next = false;
        }else{
            pos2m1 = (pos2m1+possible_t_rolls[0].first)%10;
            score2 += pos2m1+1;
            p1_next = true;
        }
    }
};
//We use DFS
//Takes about 3 second on my computer
void part_two(int pos1, int pos2){
    int MAX_SCORE = 21;
    long first_wins = 0;
    long second_wins = 0;
    History game = {std::vector<int>(),1, pos1-1, pos2-1,0,0,true};
    bool keep_running = true;
    while(keep_running){
        if(game.score1 >= MAX_SCORE || game.score2 >= MAX_SCORE){
            if(game.p1_next){
                second_wins += game.frequency;
                keep_running = game.backtrack();
            }else{
                first_wins += game.frequency;
                keep_running = game.backtrack();
            }
        }else{
            game.roll();
        }
    }
    std::cout<<"For part 2, player 1 won "<<first_wins<<" times, and player 2 won "<<second_wins<<" times"<<std::endl;
    std::cout<<"Difference of "<<first_wins-second_wins<<std::endl;
}
int main(){
    part_one(10,3);
    part_two(10,3);
}
