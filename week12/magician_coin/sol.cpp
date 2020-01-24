#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <iomanip>
using namespace std;

double dp(vector<double> &prob, vector<vector<double>> &memo, int curr_day, int curr_gain, int target){
    if(curr_gain >= target)
        return 1;
    if(curr_day >= prob.size())
        return 0;
    
    if(memo[curr_day][curr_gain] != -1.0)
        return memo[curr_day][curr_gain];

    for(int i = 0; i <= curr_gain; i++){
        double win = dp(prob, memo, curr_day + 1, curr_gain + i, target);
        double lose = dp(prob, memo, curr_day + 1, curr_gain - i, target);
        double probs = win * prob[curr_day] + (1 - prob[curr_day]) * lose;
        memo[curr_day][curr_gain] = max(probs, memo[curr_day][curr_gain]);
    }

    return memo[curr_day][curr_gain];
}


int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(5);
    int t; cin >> t;
    while(t--){
        int n, k, m;
        cin >> n >> k >> m;
        vector<double> prob(n);
        for(int i = 0; i < n; i++)
            cin >> prob[i];
        
        vector<vector<double>> memo(n, vector<double> (m, -1.0));

        cout << dp(prob, memo, 0, k, m) << "\n";
        
    }
    return 0;
}