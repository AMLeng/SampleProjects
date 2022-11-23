#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

//sum of integers in (start, end], for start >= 0
constexpr int integer_sum(int start, int end){
    return ((end-start)*(start+end+1))/2;
}
    //We treat this as a math problem and solve it in a highly ad-hoc fashion
    //Since we know the region is to the right, the initial x velocity is always positive
    //So that if v0 is the initial x velocity, we know the possible x position values are:
    //v0, 2v0-1, 3v0-3, ..., (v0+1)v0/2, or alternatively 
    //at time t (t)(2v0-t+1)/2 for t from 0 to v0,
    //And then constant afterwards
int max_pos(int velocity){
    return integer_sum(0,velocity);
}
int x_pos(int time, int x_velocity){
    if(time < x_velocity){
        return integer_sum(x_velocity-time,x_velocity);
    }else{
        return max_pos(x_velocity);
    }
}
//y velocities can drop below 0, so we must use slightly different formulas depending on which case we're in
int y_pos(int time, int y_vel){
    if(time <= y_vel){
        return integer_sum(y_vel-time,y_vel);
    }else{
        //diff measures how many timesteps we are in the regime time > y_vel
        int diff = time - y_vel - 1;
        if(diff <= y_vel){
            return integer_sum(diff, y_vel);
        }else{
            return -integer_sum(y_vel, diff);
        }
    }
}

class Region{
    public:
        Region(int x1, int x2, int y1, int y2) : xmin(x1), xmax(x2), ymin(y1), ymax(y2){}
        bool valid_x(int x){
            return x>= xmin && x<= xmax;
        }
        bool valid_y(int y){
            return y>= ymin && y<= ymax;
        }
        //For a given velocity, return the list of times < velocity for which
        //A probe would have the appropriate x coordinate
        std::vector<int> candidate_small_times(int x_velocity){
            auto candidates = std::vector<int>();
            int x_pos = 0;
            int time = 0;
            while(x_pos < xmin && time < x_velocity){
                x_pos += x_velocity - time;
                time++;
            }
            while(x_pos <= xmax && time < x_velocity){
                candidates.emplace_back(time);
                x_pos += x_velocity - time;
                time++;
            }
            return candidates;
        }
        std::vector<int> candidate_y_velocity(int time){
            auto candidates = std::vector<int>();
            //Recalling how we compute y coordinates (see Probe)
            //Observe that since the y region is always negative, we will always
            //Be in the regime t>2*y_velocity, where the y-position is
            //-integer_sum(y_velocity, time-y_velocity-1);
            //That is, y_pos(time) = -(time-2*y_velocity-1)*time/2;
            //Solving in terms of y_velocity, we then get that for y_pos
            //To be between ymin and ymax, we must have (real valued) yvelocity in the range
            //[((2*ymin/t)-1+t)/2,((2*ymax/t)-1+t)/2]
            //This is impossible as long as |2*ymin/t| <1
            //In particular, note that if ymax/t < 1/2, this is impossible
            int i = ((2*ymin)/time-1+time)/2;
            int position = y_pos(time, i);
            while(position < ymin){
                i++;
                position = y_pos(time, i);
            }while(position <= ymax){
                candidates.emplace_back(i);
                i++;
                position = y_pos(time, i);
            }
            return candidates;
        }
        int xmin;
        int xmax;
        int ymin;
        int ymax;
};

int main(){
    //Since the input just consists of four integers, we don't even bother reading in an input file
    /*int x1 = 20;
    int x2 = 30;
    int y1 = -10;
    int y2 = -5;*/
    int x1 = 119;
    int x2 = 176;
    int y1 = -141;
    int y2 = -84;

    auto target = Region(x1,x2,y1,y2);
    int max_height = 0;
    auto valid_probes = std::vector<std::pair<int,int>>();
    for(int init_x = 1; init_x <= x2; init_x++){
        //This takes care of times where the probe lands in the region for
        // t< init_x
        auto candidate_times = target.candidate_small_times(init_x);
        for(int time : candidate_times){
            auto candidate_y = target.candidate_y_velocity(time);
            for(int y_vel : candidate_y){
                //Add probe and update max height
                std::pair<int,int> p = std::make_pair(init_x,y_vel);
                if(std::find(valid_probes.begin(),valid_probes.end(),p)==valid_probes.end()){
                    valid_probes.push_back(p);
                }
                int pos = max_pos(y_vel);
                if(pos > max_height){
                    max_height = pos;
                }
            }
        }
        if(target.valid_x(max_pos(init_x))){
            for(int time = init_x; time <= -2*target.ymin; time++){
                auto candidate_y = target.candidate_y_velocity(time);
                for(int y_vel : candidate_y){
                    //Add probe and update max height
                    std::pair<int,int> p = std::make_pair(init_x,y_vel);
                    if(std::find(valid_probes.begin(),valid_probes.end(),p)==valid_probes.end()){
                        valid_probes.push_back(p);
                    }
                    int pos = max_pos(y_vel);
                    if(pos > max_height){
                        max_height = pos;
                    }
                }
            }
        }
    }
    std::cout<<"The maximum possible height is "<<max_height<<std::endl;
    std::cout<<"There number of valid probes is "<<valid_probes.size()<<std::endl;
}
