#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

// dynamic programming
int dominoFalls(vector<long long> heights, int n){
    vector<int> memo(n, 1);
    for(int i = n - 2; i >= 0; i--){
        int j = i + 1;
        while(j < n && j - i < heights[i]){
            memo[i] += memo[j];
            j += memo[j];
        }
    }
    return memo[0];
}

int main(){ 
    int t;
    cin >> t;
    vector<vector<long long> > heights;
    
    // read input
    for(int i = 0; i < t; i++){
        int length;
        cin >> length;
        vector<long long> height(length);
        for(int i = 0; i < length; i++)
            cin >> height[i];
        heights.push_back(height);
    }

    for(int i = 0; i < heights.size(); i++){
        cout << dominoFalls(heights[i], heights[i].size()) << endl;
    }

    return 0; 
}