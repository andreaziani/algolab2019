#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int dp(vector<int> &coins, vector<vector<int>> &memo, int turn, int k, int m, int start, int end){
    if(start == end){
        return turn == k? coins[start] : 0;
    }

    if(memo[start][end] != -1)
        return memo[start][end];
    
    int take_first = dp(coins, memo, (turn + 1) % m, k, m, start + 1, end);
    int take_last = dp(coins, memo, (turn + 1) % m, k, m, start, end - 1);
    if(turn == k){
        memo[start][end] = max(take_first + coins[start], take_last + coins[end]);
    } else {
        memo[start][end] = min(take_last, take_first);
    }

    return memo[start][end];
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> coins(n);
        for(int i = 0; i < n; i++) 
            cin >> coins[i];

        vector<vector<int>> memo(n, vector<int>(n, -1));
        cout << dp(coins, memo, 0, k, m, 0, n - 1) << "\n";
    }
    return 0;
}