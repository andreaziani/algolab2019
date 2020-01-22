#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
#define MUST_COVER 0
#define MUST_TAKE 1
#define ALREADY_COVERED 2

int chariot_race(vector<vector<int>> &g, vector<int> &cost, vector<vector<int>> &memo, int root, int state) {
    if(memo[state][root] != -1) return memo[state][root];
    // take = all cases
    int prendo = cost[root]; 
    for(int i = 0; i < g[root].size(); i++)
        prendo += chariot_race(g, cost, memo, g[root][i], ALREADY_COVERED);
    if(state == MUST_TAKE) return memo[state][root] = prendo;
    
    // don't take = MUST_COVER or ALREADY_COVERED
    int take_childs = 0;
    for(int i = 0; i < g[root].size(); i++)
        take_childs += chariot_race(g, cost, memo, g[root][i], MUST_COVER);

    int non_prendo = INT_MAX;
    for(int i = 0; i < g[root].size(); i++){
        int res = take_childs - chariot_race(g, cost, memo, g[root][i], MUST_COVER) + chariot_race(g, cost, memo, g[root][i], MUST_TAKE);
        if(res < non_prendo)
            non_prendo = res;
    }
    
    if(state == ALREADY_COVERED)
        if(take_childs < non_prendo) non_prendo = take_childs;
    
    return memo[state][root] = min(prendo, non_prendo);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t-- > 0){
        int n; 
        cin >> n;
        vector<vector<int>> adj(n, vector<int>());
        vector<int> costs(n);
        vector<vector<int>> memo(3, vector<int>(n, -1)); 
        for(int i = 0; i < n - 1; i++){
            int s, d;
            cin >> s >> d;
            adj[s].push_back(d);
        }
        for(int i = 0; i < n; i++)
            cin >> costs[i];
        
        cout << chariot_race(adj, costs, memo, 0, MUST_COVER) << '\n';
    }
    return 0;
}