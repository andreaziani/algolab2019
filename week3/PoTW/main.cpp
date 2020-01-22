#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int russia(vector<int> &coins, vector<vector<int>> &memo, int k, int l, int r, int m, int turn){
    if(memo[l][r] != -1) return memo[l][r];
    if(l == r) return memo[l][r] = turn == k? coins[l] : 0; // one coin
    //2 or more coins
    int left = russia(coins, memo, k, l + 1, r, m, (turn + 1) % m); //take left
    int right = russia(coins, memo, k, l, r - 1, m, (turn + 1) % m);//take right

    if(turn != k) return memo[l][r] = min(left, right);
    return memo[l][r] = max(coins[l] + left, coins[r] + right); // my turn = add the value of the coin 
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    while(t-- > 0){
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> coins(n);
        vector<vector<int> > memo(n, vector<int>(n, -1));
        for(int i = 0; i < n; i++){ 
            cin >> coins[i];
        }
        cout << russia(coins, memo, k, 0, n - 1, m, 0) << '\n';
    }
    return 0;
}