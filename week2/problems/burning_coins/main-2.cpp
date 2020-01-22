#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int dp(vector<int> &coins, vector<vector<int>> &memo, int start, int end, int turn){
    if(start == end && turn % 2 == 0)
        return coins[start];
    else if(start == end)
        return 0;
    
    if(memo[start][end] != -1)
        return memo[start][end];

    int take_first, take_last;
    if(turn % 2 == 0){
        take_first = coins[start] + dp(coins, memo, start + 1, end, turn + 1);
        take_last = coins[end] + dp(coins, memo, start, end - 1, turn + 1);
        memo[start][end] = max(take_first, take_last);
    } else {
        take_first = dp(coins, memo, start + 1, end, turn + 1);
        take_last = dp(coins, memo, start, end - 1, turn + 1);
        memo[start][end] = min(take_first, take_last);
    }
    
    return memo[start][end];
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n; cin >> n;
        vector<int> coins(n);
        for(int i = 0; i < n; i++)
            cin >> coins[i];
        
        vector<vector<int>> memo(n, vector<int>(n, -1));

        int res = dp(coins, memo, 0, n - 1, 0);
        cout << res << "\n";
    }
    return 0;
}