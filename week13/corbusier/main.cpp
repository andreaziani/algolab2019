#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int dp(long target, int k, vector<long> &heights, vector<vector<vector<int>>> &memo, int curr, long amount, int state, int count){   
    if(memo[state][curr][amount] != -1)
        return memo[state][curr][amount];
    
    if(amount == target && count != 0)
        return memo[state][curr][amount] = 1;
    else if(curr == heights.size() - 1)
        return memo[state][curr][amount] = 0;
    
    memo[1][curr][amount] = dp(target, k, heights, memo, curr + 1, (amount + heights[curr]) % k, 1, count + 1); // take
    memo[0][curr][amount] = dp(target, k, heights, memo, curr + 1, amount % k, 0, count); // not take

    return memo[state][curr][amount] || memo[!state][curr][amount];
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    
    while(t--){
        int n, i, k;
        cin >> n >> i >> k;
        
        vector<long> heights(n);
        for(int i = 0; i < n; i++){
            int h;
            cin >> h;
            heights[i] = h;
        }
        
        // [taken, not taken][position][amount for position]
        vector<vector<vector<int>>> memo(2, vector<vector<int>>(n, vector<int> (k, -1)));
        
        if(dp(i % k, k, heights, memo, 0, 0, 0, 0)){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }

    return 0;
}