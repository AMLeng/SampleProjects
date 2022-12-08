#include <iostream>
#include <string>
#include<vector>
#include <utility>
#include <cassert>

bool check_visible(std::vector<std::vector<int>> heights, int a, int b){
    int i = 1;
    while(i<=a){
        if(heights[a-i][b] >= heights[a][b]){
            break;
        }
        i++;
    }
    if(i > a){
        return true;
    }
    i = 1;
    while(i<=b){
        if(heights[a][b-i] >= heights[a][b]){
            break;
        }
        i++;
    }
    if(i > b){
        return true;
    }
    i = a+1;
    while(i<heights.size()){
        if(heights[i][b] >= heights[a][b]){
            break;
        }
        i++;
    }
    if(i >= heights.size()){
        return true;
    }
    i = b+1;
    while(i<heights.front().size()){
        if(heights[a][i] >= heights[a][b]){
            break;
        }
        i++;
    }
    if(i >= heights.front().size()){
        return true;
    }
    return false;
}

int score(std::vector<std::vector<int>> heights, int a, int b){
    auto scores = std::vector<int>{1,1,1,1};
    int i = 1;
    while(i<=a){
        if(heights[a-i][b] >= heights[a][b]){
            break;
        }
        scores[0]++;
        i++;
    }
    if(i > a){
        scores[0]--;
    }
    i = 1;
    while(i<=b){
        if(heights[a][b-i] >= heights[a][b]){
            break;
        }
        scores[1]++;
        i++;
    }
    if(i > b){
        scores[1]--;
    }
    i = a+1;
    while(i<heights.size()){
        if(heights[i][b] >= heights[a][b]){
            break;
        }
        scores[2]++;
        i++;
    }
    if(i >=heights.size()){
        scores[2]--;
    }
    i = b+1;
    while(i<heights.front().size()){
        if(heights[a][i] >= heights[a][b]){
            break;
        }
        scores[3]++;
        i++;
    }
    if(i >=heights.front().size()){
        scores[3]--;
    }
    return scores[0]*scores[1]*scores[2]*scores[3];
}
int main(){
    std::string nextline;
    auto trees = std::vector<std::vector<int>>();
    while(std::getline(std::cin,nextline)){
        if(nextline.back() == '\r'){
            nextline.pop_back();
        }
        trees.emplace_back(std::vector<int>());
        for(char c : nextline){
            trees.back().push_back(c-'0');
        }
    }
    int n = trees.size();
    int m =trees.front().size();
    int sum = 0;
    int max = 0;
    for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
        if(check_visible(trees,i,j)){
            sum++;
        }
        int temp = score(trees,i,j);
        if(temp > max){
            max = temp;
        }
    }
    }
    std::cout<<sum<<std::endl;
    std::cout<<max<<std::endl;
}
