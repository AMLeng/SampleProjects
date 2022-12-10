#include <iostream>
#include <string>
#include <cassert>
#include <vector>

class CRT{
    public:
        CRT() : cycle(1),X(1) {}
        void noop(){
            run();
            update();
        }
        void addx(int n){
            run();
            update();
            run();
            X += n;
            update();
        }
        void draw(){
            for(int i=1; i<=240; i++){
                if(drawing[i-1]){
                    std::cout<<'#';
                }else{
                    std::cout<<'.';
                }
                if(i%40 == 0){
                    std::cout<<std::endl;
                }
            }
        }
        int sum(){
            int s= 0;
            for(int i=0; i<6; i++){
                s += signal_strength[i];
            }
            return s;
        }
        private:
        int cycle;
        int X;
        std::vector<int> signal_strength;
        std::vector<bool> drawing;
        void run(){
            int pos = (cycle-1)%40;
            int diff = pos - X;
            if(diff == -1 || diff == 0 || diff == 1){
                drawing.push_back(true);
            }else{
                drawing.push_back(false);
            }
        }
        void update(){
            cycle++;
            if(cycle%40 == 20){
                std::cout<<"At "<<cycle<<" X = "<<X<<std::endl;
                signal_strength.push_back(cycle*X);
            }
        }
};
int main(){
    std::string nextline;
    auto tube = CRT();
    while(std::getline(std::cin,nextline)){
        if(nextline[0] == 'n'){
            tube.noop();
        }else{
            assert(nextline[0] == 'a');
            tube.addx(std::stoi(nextline.substr(5)));
        }
    }
    std::cout<<tube.sum()<<std::endl;
    tube.draw();
}
