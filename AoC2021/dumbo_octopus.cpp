#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include <cassert>

class OctopusGrid {
    public:
    OctopusGrid(std::array<std::array<int,10>,10> input) : grid(input), time(0), flashes(0){}
    bool timestep(){
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                grid[i][j]++;
            }
        }
        bool go_again;
        do{
            go_again = false;
            for(int i=0; i<10; i++){
                for(int j=0; j<10; j++){
                    if(grid[i][j] >9 && grid[i][j] <= 17){
                        go_again |= flash(i,j);
                    }
                }
            }
        }while(go_again);
        time++;
        return reset();
    }
    int current_time(){return time;}
    int total_flashes(){return flashes;}
    void print_grid(){
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                std::cout <<grid[i][j];
            }
            std::cout<<std::endl;
        }
    }
    private:
    std::array<std::array<int, 10>,10> grid;
    int time;
    int flashes;
    bool flash(int x, int y){
        bool new_flash = false;
        flashes++;
        //An energy value of 18 is inaccessible by normal means
        grid[x][y] = 18;
        for(int i = std::max(0,x-1); i <= std::min(9,x+1); i++){
            for(int j = std::max(0,y-1); j <= std::min(9,y+1); j++){
                grid[i][j]++;
                if(grid[i][j] == 10){new_flash = true;}
            }
        }
        return new_flash;
    }
    //Reset the octopi that have flashed, returning true if the all have
    bool reset(){
        bool all_flashed = true;
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                if(grid[i][j] >9){
                    assert(grid[i][j] >17);
                    grid[i][j] = 0;
                }else{
                    all_flashed = false;
                }
            }
        }
        return all_flashed;
    }
};

int main(){
    std::string nextline;
    auto input_array = std::array<std::array<int, 10>,10>();
    for(int i=0; i<10; i++){
        std::getline(std::cin, nextline);
        if(nextline.back() == '\r'){nextline.pop_back();}
        for(int j=0; j<10; j++){
            input_array[i][j] = nextline[j]-'0';
            assert(input_array[i][j] >=0);
            assert(input_array[i][j] < 9);
        }
    }
    auto grid = OctopusGrid(input_array);
    for(int i=0; i<100; i++){
        grid.timestep();
    }
    std::cout<<"At time "<<grid.current_time()<<", we have had a total of "<<grid.total_flashes()<<" flashes\n";
    while(!grid.timestep()){}
    std::cout<<"At time "<<grid.current_time()<<" all the octopi flashed\n";
}
