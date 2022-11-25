#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <utility>

typedef std::pair<int,int> Point;
class Image{
    public:
        Image(std::map<Point,bool> input) : image(input), outside_light(false){}
        Image(Image original, const std::bitset<512>& enhancement_algo){
            for(auto pair : original.image){
                auto adjacent = neighbors(pair.first);
                for(Point adj : adjacent){
                    if(this->image.find(adj) == this->image.end()){
                        this->image.insert(std::make_pair(adj,original.filter(enhancement_algo,adj)));
                    }
                }
            }
            if(original.outside_light){
                outside_light = enhancement_algo[511];
            }else{
                outside_light = enhancement_algo[0];
            }
        }
        bool at(Point p) const{
            if(image.find(p) != image.end()){
                return image.at(p);
            }else{
                return outside_light;
            }
        }
        int count_light() const{
            int sum = 0;
            for(auto pair : image){
                if(pair.second){
                    sum++;
                }
            }
            return sum;
        }
    private:
        bool outside_light;
        bool filter(const std::bitset<512>& algorithm, Point p) const{
            int index = 0;
            auto adjacent = neighbors(p);
            for(Point adj : adjacent){
                index = index<<1;
                index += static_cast<int>(this->at(adj));
            }
            return algorithm[index];
        }
        static std::vector<Point> neighbors(Point p){
            auto adjacent = std::vector<Point>();
            for(int i=-1; i<2; i++){
                for(int j=-1; j<2; j++){
                    adjacent.emplace_back(std::make_pair(p.first+i,p.second+j));
                }
            }
            return adjacent;
        }
        std::map<Point,bool> image;
};
int main(){
    std::bitset<512> algorithm = std::bitset<512>();
    std::string nextline;
    std::getline(std::cin,nextline);
    for(int i=0; i<512; i++){
        algorithm.set(i,nextline[i]=='#');
    }
    std::getline(std::cin,nextline);
    std::map<Point,bool> input_image = std::map<Point,bool>();
    int x=0;
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        for(int y=0; y<nextline.size(); y++){
            input_image.insert(std::make_pair<Point,bool>(std::make_pair(x,y),nextline[y]=='#'));
        }
        x++;
    }
    auto original = Image(input_image);
    auto odd_enhance = Image(original, algorithm);
    original = Image(odd_enhance, algorithm);
    std::cout<<"After enhancing twice, the number of light pixels is "<<original.count_light()<<std::endl;
    for(int i=0; i<24; i++){ 
        odd_enhance = Image(original, algorithm);
        original = Image(odd_enhance, algorithm);
    }
    std::cout<<"After enhancing fifty times, the number of light pixels is "<<original.count_light()<<std::endl;
}
