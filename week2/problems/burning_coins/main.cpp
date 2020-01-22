#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int burninigCoins(vector<int> &coins, vector<vector<int>> &memo, int l, int r){
    if(r == l) return coins[l];
    if(memo[l][r] != 0) return memo[l][r];
    if(l + 1 == r) return memo[l][r] = max(coins[l], coins[r]); // only two coins, take max
    if(r - l == 2){ // 3 coins
        int res = 0;
        if(coins[l] < coins[r]){ // take the max boundary
            res += coins[r];
            if(coins[l + 1] < coins[l]) res+= coins[l + 1]; // then take the min (adv. will takes max)
            else res += coins[l];
        }
        else {
            res += coins[l];
            if(coins[r - 1] < coins[r]) res += coins[r - 1];
            else res += coins[r];
        }
        return memo[l][r] = res;
    }
    //more than 3 coins
    int left_left = burninigCoins(coins, memo, l + 2, r); //me left, him left
    int left_right = burninigCoins(coins, memo, l + 1, r - 1);// me left, him right
    int right_right = burninigCoins(coins, memo, l, r - 2); // me right, him right
    int right_left = left_right; // me right, him left
    // take the max from (take left first + min possible then, take right first + min possible then)
    return memo[l][r] = max(coins[l] + min(left_right, left_left), coins[r] + min(right_right, right_left));
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int n;
        cin >> n;
        vector<int> coins(n);
        for(int j = 0; j < n; j++)
            cin >> coins[j];
        vector<vector<int>> memo(coins.size(), vector<int>(coins.size(), 0));
        cout << burninigCoins(coins, memo, 0, coins.size() - 1) << '\n';
    }
    return 0;
}