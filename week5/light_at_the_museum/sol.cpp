#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <map>
using namespace std;

void computeCombinations(vector<vector<pair<int,int>>> &s, map<vector<int>, int> &comb, vector<int> curr_brightness, int curr, int n_switches, int n){
    if(comb.find(curr_brightness) != comb.end()){
        comb[curr_brightness] = min(n_switches, comb[curr_brightness]);
    } else {
        comb[curr_brightness] = n_switches;
    }
    
    if(curr < n){
        vector<int> b1 = curr_brightness; //leave switch like this
        vector<int> b2 = curr_brightness; //switch off
        for(int i = 0; i < s[curr].size(); i++){
            b2[i] = curr_brightness[i] - s[curr][i].first + s[curr][i].second;
        }
        
        computeCombinations(s, comb, b1, curr + 1, n_switches, n);
        computeCombinations(s, comb, b2, curr + 1, n_switches + 1, n);
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;

        vector<int> target_brightness(m);
        for(int i = 0; i < m; i++)
            cin >> target_brightness[i];
        
        vector<vector<pair<int,int>>> switches1(n);
        vector<int> curr_b1(m, 0);
        vector<int> curr_b2(m, 0);
        for(int i = 0; i < n/2; i++){
            for(int j = 0; j < m; j++){
                int on, off;
                cin >> on >> off;
                switches1[i].push_back({on, off});
                curr_b1[j] += on;
            }
        }
        
        for(int i = n/2; i < n; i++){
            for(int j = 0; j < m; j++){
                int on, off;
                cin >> on >> off;
                switches1[i].push_back({on, off});
                curr_b2[j] += on;
            }
        }
        
        map<vector<int>, int> l1;
        map<vector<int>, int> l2;
        
        computeCombinations(switches1, l1, curr_b1, 0, 0, n/2);
        computeCombinations(switches1, l2, curr_b2, n/2, 0, n);

        int res = INT_MAX;
        for(auto it = l1.begin(); it != l1.end(); it++){
            vector<int> target = target_brightness;
            for(int i = 0; i < m; i++){
                target[i] -= it->first[i];
            }

            if(l2.find(target) != l2.end()){
                auto v = l2.find(target)->second;
                res = min(res, v + it->second);
            }
        }

        if(res == INT_MAX)
            cout << "impossible\n";
        else
            cout << res << "\n";

    }
    return 0;
}