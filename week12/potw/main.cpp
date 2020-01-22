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
        int n, m, k;
        long x;
        cin >> n >> m >> x >> k;
        vector<vector<pair<int, long>>> g(n);
        for(int i = 0; i < m; i++){
            int u, v;
            long p;
            cin >> u >> v >> p;
            g[u].push_back({v, p});
        }

        for(int i = 0; i < n; i++){
            if(g[i].empty()){
                g[i].push_back({0, 0});
            }
        }

        vector<vector<long>> memo(k + 1, vector<long>(n, 0));
        
        for(int i = 1; i < k + 1; i++){
            for(int j = 0; j < n; j++){
                long max = 0;
                for(int dest = 0; dest < g[j].size(); dest++){
                    if(g[j][dest].first == 0){
                        max = std::max(max, memo[i][g[j][dest].first]);
                    } else{
                        max = std::max(max, memo[i - 1][g[j][dest].first] + g[j][dest].second);
                    }
                }
                memo[i][j] = max;
            }
        }

        if(memo[k][0] < x){
            cout << "Impossible\n";
        } else{
            for(int i = 0; i < k + 1; i++){
                if(memo[i][0] >= x){
                    cout << i << "\n";
                    break;
                }
            }
        }
        
    }
    

    return 0;

}