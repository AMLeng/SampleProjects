#include<iostream>
#include<string>
#include<array>
#include<vector>
#include<cassert>
#include<set>
#include<algorithm>

class Vector{
    public:
        explicit Vector(int a, int b, int c){
            coordinates[0] = a;
            coordinates[1] = b;
            coordinates[2] = c;
        }
        void print() const{
            std::cout<<"("<<coordinates[0]<<','<<coordinates[1]<<','<<coordinates[2]<<')'<<std::endl;
        }
        Vector& operator+=(const Vector& summand){
            for(int i=0; i<3; i++){
                coordinates[i] += summand.coordinates[i];
            }
            return *this;
        }
        friend Vector operator+(Vector lhs, const Vector& rhs){
            lhs+= rhs;
            return lhs;
        }
        Vector& operator-=(const Vector& summand){
            for(int i=0; i<3; i++){
                coordinates[i] -= summand.coordinates[i];
            }
            return *this;
        }
        friend Vector operator-(Vector lhs, const Vector& rhs){
            lhs-= rhs;
            return lhs;
        }
        //If using C++20, could replace this all with the spaceship operator
        //And would also save some headaches about whether or not < defines a strict weak ordering
        bool operator<(const Vector& rhs) const{
            return coordinates[0] < rhs.coordinates[0] || 
            (coordinates[0] == rhs.coordinates[0] && coordinates[1] < rhs.coordinates[1]) || 
            (coordinates[0] == rhs.coordinates[0] && coordinates[1] == rhs.coordinates[1] && coordinates[2] < rhs.coordinates[2]);
        }
        bool operator==(const Vector& rhs) const{
            return (this->coordinates[0] == rhs.coordinates[0]) && (this->coordinates[1] == rhs.coordinates[1]) && this->coordinates[2] == rhs.coordinates[2];
        }
        //auto operator<=>(const Vector& rhs) const = default;
        int size(){
            int sum = 0;
            for(int i=0; i<3; i++){
            if(coordinates[i] < 0){
                sum -= coordinates[i];
            }else{
                sum += coordinates[i];
            }
            }
            return sum;
        }

        void next_orientation(int orientation_number){
            assert(orientation_number < 24 && orientation_number>=0);
            orientation_number++;
            if(orientation_number%4!=0){
                switch(orientation_number/8){
                    case 0:
                        rot12();
                        break;
                    case 1:
                        rot02();
                        break;
                    case 2:
                        rot01();
                        break;
                    default:
                        assert(false);
                }
            }else{
                switch(orientation_number){
                    case 4:
                        rot01();
                        rot01();
                        break;
                    case 12:
                        rot12();
                        rot12();
                        break;
                    case 20:
                        rot02();
                        rot02();
                        break;
                    case 8:
                        rot01();
                        break;
                    case 16:
                        rot12();
                        break;
                    case 24:
                        rot02();
                        rot12();
                        break;
                    default:
                        assert(false);
                }
            }
        }
        std::array<int, 3> coordinates;
    private:
        void rot01(){
            int temp = coordinates[0];
            coordinates[0] = -coordinates[1];
            coordinates[1] = temp;
        }
        void rot12(){
            int temp = coordinates[1];
            coordinates[1] = -coordinates[2];
            coordinates[2] = temp;
        }
        void rot02(){
            int temp = coordinates[0];
            coordinates[0] = -coordinates[2];
            coordinates[2] = temp;
        }
};

class Scanner{
    public:
        Scanner(int n) : aligned_to(n), beacons(), scanner_name(n), orientation(0), displacement(0,0,0){
            }
        void add_beacon(Vector point){
            beacons.emplace_back(point);
        }
        int name() const{
            return scanner_name;
        }
        int alignment() const{
            return aligned_to;
        }
        bool realigned() const{
            return scanner_name != aligned_to;
        }
        Vector position(){
            return displacement;
        }
        static void align(std::vector<Scanner>& scanner_list, int first, int second){
            int i = scanner_list[first].aligned_to;
            int j = scanner_list[second].aligned_to;
            if(i == j){
                return;
            }
            if(i > j){
                int temp = first;
                first = second;
                second = temp;
            }
            auto original_second_alignment = scanner_list[second].aligned_to;
            auto original_second_orientation = scanner_list[second].orientation;
            auto original_second_displacement = scanner_list[second].displacement;
            bool successful = attempt_align(scanner_list[first], scanner_list[second]);
            if(successful){
                //std::cout<<"Aligning "<<second<<" to "<<first<<" from "<<scanner_list[second].aligned_to<<" and "<<original_second_alignment<<std::endl;
                //std::cout<<"Success with orientation "<<scanner_list[second].orientation<<" and displacement ";
                //scanner_list[second].displacement.print();
                for(Scanner& s : scanner_list){
                    if(s.aligned_to == original_second_alignment){
                        s.displacement -= original_second_displacement; 
                        //std::cout<<"Old displacement from "<<second<<" to "<<s.name()<<" is ";
                        //s.displacement.print();
                        //Now the displacement value is the displacement from
                        //second -> s, in the original alignment
                        //rather than the displacement from original_second_alignment -> s

                        //This nonsense is necessary to make sure that s transforms its orientation
                        //The same way that second did
                        s.orientation = original_second_orientation;
                        for(int i=original_second_orientation; (i%24) != scanner_list[second].orientation; i++){
                            s.next_orientation();
                        }
                        //std::cout<<"New displacement from "<<second<<" to "<<s.name()<<" is ";
                        //s.displacement.print();
                        s.aligned_to = scanner_list[second].aligned_to;
                        s.displacement += scanner_list[second].displacement;
                        //std::cout<<"New displacement of "<<s.name()<<" relative to "<<s.aligned_to<<" is ";
                        //s.displacement.print();
                    }
                }
            }
        }
        void print() const{
            std::cout<<"Scanner "<<scanner_name<<" aligned to "<<aligned_to<<" with relative position ";
            displacement.print();
            for(Vector b : beacons){
                auto temp = b + displacement;
                temp.print();
            }
        }
        int size() const{
            return beacons.size();
        }
        std::vector<Vector> beacons;
    private:
        int scanner_name;
        int aligned_to;
        int orientation;//Orientation value
        //This is rather broken, but the way it behaves is that it determines
        //What the "next_orientation" function will do
        Vector displacement;//Relative position of this scanner, if aligned_to is at (0,0,0)
        void next_orientation(){
            for(Vector& v : beacons){
                v.next_orientation(orientation);
            }
            displacement.next_orientation(orientation);
            orientation = (orientation+1)%24;
        }
        bool align_by(const Vector base, Scanner& second){
            //If too slow, can change this to an unordered set 
            //We avoid doing so for now since it would require implementing a hash for Vector
            std::set<Vector> displacements;
            for(const Vector& beacon : this->beacons){
                displacements.insert(beacon - base);
            }
            for(int i=0; i<24; i++){
                for(int j=11; j<second.size(); j++){
                    int matches = 0;
                    for(Vector beacon : second.beacons){
                        matches += displacements.count(beacon - second.beacons[j]);
                    }
                    if(matches >= 12){
                        /*std::cout<<"Overlap of "<<matches<<std::endl;
                        std::cout<<"Aligned at ";
                        base.print();
                        std::cout<<"Matched with ";
                        second.beacons[j].print();*/
                        second.displacement += (base - second.beacons[j])+this->displacement;
                        second.aligned_to = this->aligned_to;
                        return true;
                    }
                }
                second.next_orientation();
            }
            return false;
        }
        static bool attempt_align(Scanner& first, Scanner& second){
            assert(first.aligned_to < second.aligned_to);
            //Try to align second to first
            for(const Vector& base : first.beacons){
                if(first.align_by(base,second)){
                    return true;
                }
            }
            return false;
        }
};

int count_match(Scanner a, Scanner b){
    int matches = 0;
    std::set<Vector> distinct = std::set<Vector>();
    for(auto beacon : a.beacons){
        auto temp = beacon + a.position();
        distinct.insert(temp);
    }
    for(auto beacon : b.beacons){
        auto temp = beacon + b.position();
        if(std::find(distinct.begin(),distinct.end(),temp)!=distinct.end()){
            matches++;
        }
    }
    return matches;
}

int main(){
    std::string nextline;
    std::vector<Scanner> scanners = std::vector<Scanner>();
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){nextline.pop_back();}
        if(nextline.size() == 0){continue;}
        if(nextline.back() == '-'){
            scanners.emplace_back(std::stoi(nextline.substr(nextline.find("scanner")+8)));
        }else{
            int a = std::stoi(nextline);
            std::string back = nextline.substr(nextline.find(',')+1);
            int b = std::stoi(back);
            int c = std::stoi(back.substr(back.find(',')+1));
            auto temp = Vector(a,b,c);
            scanners.back().add_beacon(Vector(a,b,c));
        }
    }
    Scanner::align(scanners,0,13);
    Scanner::align(scanners,13,26);
    Scanner::align(scanners,13,11);
    Scanner::align(scanners,11,20);
    Scanner::align(scanners,20,7);
    Scanner::align(scanners,7,25);
    Scanner::align(scanners,7,5);
    Scanner::align(scanners,5,34);
    Scanner::align(scanners,11,2);
    Scanner::align(scanners,2,28);
    Scanner::align(scanners,11,15);
    Scanner::align(scanners,15,21);
    Scanner::align(scanners,21,9);
    Scanner::align(scanners,21,32);
    Scanner::align(scanners,21,12);
    Scanner::align(scanners,17,12);
    Scanner::align(scanners,17,35);
    Scanner::align(scanners,17,4);
    Scanner::align(scanners,4,23);
    Scanner::align(scanners,23,33);
    Scanner::align(scanners,4,10);
    Scanner::align(scanners,10,24);
    Scanner::align(scanners,24,1);
    Scanner::align(scanners,1,38);
    Scanner::align(scanners,1,22);
    Scanner::align(scanners,1,18);
    Scanner::align(scanners,18,27);
    Scanner::align(scanners,27,8);
    Scanner::align(scanners,18,31);
    Scanner::align(scanners,31,36);
    Scanner::align(scanners,1,16);
    Scanner::align(scanners,16,37);
    Scanner::align(scanners,37,14);
    Scanner::align(scanners,14,6);
    Scanner::align(scanners,6,19);
    Scanner::align(scanners,6,29);
    Scanner::align(scanners,29,30);
    Scanner::align(scanners,30,3);


    /*for(int i=0; i<scanners.size(); i++){
        for(int j=i+1; j<scanners.size(); j++){
            Scanner::align(scanners,i,j);
        }
    }*/
    std::set<Vector> distinct = std::set<Vector>();
    for(auto scanner : scanners){
        std::cout<<scanner.name()<<": ";
        scanner.position().print();
        for(auto beacon : scanner.beacons){
            auto temp = beacon + scanner.position();
            if(std::find(distinct.begin(),distinct.end(),temp)==distinct.end()){
                distinct.insert(temp);
            }
        }
    }
    std::cout<<"Size "<<distinct.size()<<std::endl;
    int max_distance = 0;
    for(auto scanner : scanners){
        for(auto scanner2 : scanners){
            auto temp = scanner.position() - scanner2.position();
            if(temp.size() > max_distance){
                //std::cout<<"New max between "<<scanner.name()<<" and "<<scanner2.name()<<std::endl;
                max_distance = temp.size();
            }
        }
    }
    std::cout<<"Max distance "<<max_distance<<std::endl;
}
