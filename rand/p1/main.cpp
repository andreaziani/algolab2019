#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>
using namespace std;

// this solution is very slow but can be improved by splitting the uppath and pass the already taken by reference to this
// new function 
int dp(int n, vector<vector<int>> matrix, int curr_x, int curr_y, bool down, set<pair<int,int>> already_taken, int curr_gain){
    if((curr_x == 0 && curr_y == 0) && !down){
        return curr_gain;
    }

    if((curr_x == n - 1 && curr_y == n - 1) || !down){
        int take_left = 0;
        int take_up = 0;
        if(curr_y - 1 >= 0){
            if(already_taken.find({curr_x, curr_y - 1}) == already_taken.end()){
                take_left = dp(n, matrix, curr_x, curr_y - 1, false, already_taken, curr_gain + matrix[curr_x][curr_y - 1]);
            } else {
                take_left = dp(n, matrix, curr_x, curr_y - 1, false, already_taken, curr_gain);
            }
        }
        if(curr_x - 1 >= 0){
            if(already_taken.find({curr_x - 1, curr_y}) == already_taken.end()){
                take_up = dp(n, matrix, curr_x - 1, curr_y, false, already_taken, matrix[curr_x - 1][curr_y] + curr_gain);
            } else {
                take_up = dp(n, matrix, curr_x - 1, curr_y, false, already_taken, curr_gain);
            }
        } 
        return max(take_left, take_up);
    } else {
        int take_right = curr_gain;
        int take_bottom = curr_gain;
        if(curr_y + 1 < n){
            if(already_taken.find({curr_x, curr_y + 1}) == already_taken.end()){
                already_taken.insert({curr_x, curr_y + 1});
                take_right = dp(n, matrix, curr_x, curr_y + 1, true, already_taken, matrix[curr_x][curr_y + 1] + curr_gain);
            } else {
                take_right = dp(n, matrix, curr_x, curr_y + 1, true, already_taken, curr_gain);
            }
        }
        
        if(curr_x + 1 < n){
            if(already_taken.find({curr_x + 1, curr_y}) == already_taken.end()){
                already_taken.insert({curr_x + 1, curr_y});
                take_bottom = dp(n, matrix, curr_x + 1, curr_y, true, already_taken, matrix[curr_x + 1][curr_y] + curr_gain);
            } else {
                take_bottom = dp(n, matrix, curr_x + 1, curr_y, true, already_taken, curr_gain);
            }
        }
        return max(take_right, take_bottom);
    }
}
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n;
        cin >> n;
        vector<vector<int>> matrix(n, vector<int>(n, 0));

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> matrix[i][j];
            }
        }

        set<pair<int,int>> already_taken;
        cout << dp(n, matrix, 0, 0, true, already_taken, 0) << "\n";
    }
    return 0;
}