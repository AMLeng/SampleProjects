#include<iostream>
#include<map>
#include<utility>
#include<cassert>
#include<algorithm>

enum class Direction{Vertical, Horizontal};
typedef std::pair<int,int> Point;

class TransparentPaper{
    public:
    void fold(Direction d, int coordinate){
        switch(d){
            case Direction::Vertical:
                for(auto it = points.begin(); it != points.end(); ){
                    if(it->first.first > coordinate){
                        Point new_point = std::make_pair(2*coordinate - it->first.first, it->first.second);
                        if(points.find(new_point) == points.end()){
                            points.emplace(new_point, it->second);
                        }
                        it = points.erase(it);
                    }else{
                        it++;
                    }
                }
            break;
            case Direction::Horizontal:
                for(auto it = points.begin(); it != points.end(); ){
                    if(it->first.second > coordinate){
                        Point new_point = std::make_pair(it->first.first, 2*coordinate - it->first.second);
                        if(points.find(new_point) == points.end()){
                            points.emplace(new_point, it->second);
                        }
                        it = points.erase(it);
                    }else{
                        it++;
                    }
                }
            break;
        }
    }
    int point_count(){
        return points.size();
    }
    void add_point(std::string line){
        auto second_pos = line.find(',') + 1;
        points.emplace(std::make_pair(std::stoi(line),std::stoi(line.substr(second_pos))), 1);
    }
    void print(){
        int max_x = 0;
        int max_y = 0;
        for(auto x : points){
            Point p = x.first;
            max_x = std::max(max_x, p.first);
            max_y = std::max(max_y, p.second);
        }
        for(int i=0; i<=max_y; i++){
            for(int j=0; j<=max_x; j++){
                if(points.find(std::make_pair(j,i)) != points.end()){
                    std::cout<< '#';
                }else{
                    std::cout<< '.';
                }
            }
            std::cout<< std::endl;
        }
    }
    private:
    std::map<Point, int> points;
};

int main(){
    auto paper = TransparentPaper();
    std::string nextline;
    std::getline(std::cin, nextline);
    //Would be ">0", but this lets us uniformly deal with Windows inputs with CRLF 
    while(nextline.size() >1){
        paper.add_point(nextline);
        std::getline(std::cin, nextline);
    }
    {
        std::getline(std::cin, nextline);
        auto data_pos = nextline.find('=') - 1;
        assert(nextline[data_pos] == 'x' || nextline[data_pos] == 'y');
        if(nextline[data_pos] == 'x'){
            paper.fold(Direction::Vertical, std::stoi(nextline.substr(data_pos+2)));
            std::cout<<"Folding at x = "<<std::stoi(nextline.substr(data_pos+2))<<std::endl;
        }else{
            paper.fold(Direction::Horizontal, std::stoi(nextline.substr(data_pos+2)));
            std::cout<<"Folding at y = "<<std::stoi(nextline.substr(data_pos+2))<<std::endl;
        }
    }
    std::cout << "Answer to part 1 is: "<<paper.point_count() << std::endl;

    while(std::getline(std::cin, nextline)){
        auto data_pos = nextline.find('=') - 1;
        assert(nextline[data_pos] == 'x' || nextline[data_pos] == 'y');
        if(nextline[data_pos] == 'x'){
            paper.fold(Direction::Vertical, std::stoi(nextline.substr(data_pos+2)));
            std::cout<<"Folding at x = "<<std::stoi(nextline.substr(data_pos+2))<<std::endl;
        }else{
            paper.fold(Direction::Horizontal, std::stoi(nextline.substr(data_pos+2)));
            std::cout<<"Folding at y = "<<std::stoi(nextline.substr(data_pos+2))<<std::endl;
        }
    }
    paper.print();
}
