#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <iomanip>
using namespace std;

double dp(vector<vector<double>> &memo, vector<double> &probs, int day, int curr_gain, int target){
    if(curr_gain >= target) // target reached
        return 1;
    
    if(day >= memo.size()) // impossible to reach the target
        return 0;
    
    if(memo[day][curr_gain] != -1)
        return memo[day][curr_gain];
    
    for(int i = 0; i <= curr_gain; i++){
        double win = dp(memo, probs, day + 1, curr_gain + i, target);
        double lose = dp(memo, probs, day + 1, curr_gain - i, target);
        double probability = win * probs[day] + (1 - probs[day]) * lose; 
        memo[day][curr_gain] = max(probability, memo[day][curr_gain]);
    }

    return memo[day][curr_gain];
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        double n, k, m;
        cin >> n >> k >> m;
        vector<double> probs(n);
        for(int i = 0; i < n; i++)
            cin >> probs[i];
        
        vector<vector<double>> memo(n, vector<double>(m, -1.0));

        cout << fixed << setprecision(5);
        cout << dp(memo, probs, 0, k, m) << "\n";
    }
    return 0;
}