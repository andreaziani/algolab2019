#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int holes, canals, score, max_moves;
        cin >> holes >> canals >> score >> max_moves;
        
        vector<vector<pair<int, long>>> g(holes);
        for(int i = 0; i < canals; i++){
            int u, v;
            long p;
            cin >> u >> v >> p;
            g[u].push_back({v, p});
        }

        for(int i = 0; i < holes; i++){ // jump from a leaf to the start
            if(g[i].empty()){
                g[i].push_back({0, 0});
            }
        }

        vector<vector<long>> memo(max_moves + 1, vector<long>(holes, 0));


        for(int k = 1; k <= max_moves; k++){
            for(int i = 0; i < holes; i++){
                long max_score = 0;
                for(int j = 0; j < g[i].size(); j++){
                    if(g[i][j].first == 0){
                        max_score = max(max_score, memo[k][g[i][j].first]);
                    } else{
                        max_score = max(max_score, memo[k - 1][g[i][j].first] + g[i][j].second);
                    }
                }
                memo[k][i] = max_score;
            }
        }

        if(memo[max_moves][0] < score){
            cout << "Impossible\n";
        } else {
            for(int i = 0; i < max_moves + 1; i++){
                if(memo[i][0] >= score){
                    cout << i << "\n";
                    break;
                }
            }
        }

    }
    return 0;
}