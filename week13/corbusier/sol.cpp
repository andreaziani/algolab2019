#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int dp(vector<long> &h, int target, int k, int curr_disk, int curr_sum, vector<vector<int>> &memo, int count){
    if(memo[curr_disk][curr_sum] != -1)
        return memo[curr_disk][curr_sum];

    if(curr_sum == target && count != 0)
        return memo[curr_disk][curr_sum] = 1;
    
    if(curr_disk == h.size() - 1)
        return memo[curr_disk][curr_sum] = 0;

    int take = dp(h, target, k, curr_disk + 1, (curr_sum + h[curr_disk]) % k, memo, count + 1);
    int not_take = dp(h, target, k, curr_disk + 1, curr_sum % k, memo, count);

    return memo[curr_disk][curr_sum] = max(take, not_take);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, i, k;
        cin >> n >> i >> k;
        int target = i % k;
        vector<long> h(n);
        for(int i = 0; i < n; i++)
            cin >> h[i];

        vector<vector<int>> memo(n, vector<int>(k, -1));

        if(dp(h, target, k, 0, 0, memo, 0)) // last count needed to don't have a empty subset
            cout << "yes\n";
        else 
            cout << "no\n";
        
    }
    return 0;
}