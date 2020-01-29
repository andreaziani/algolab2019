/* 50 points, only the first subtask */
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

#define ALREADY_COVERED 0
#define CAN_COVER 1

int dp(vector<vector<int>> &memo, int state, vector<vector<int>> &tree, int curr_node, int source){
    if(tree[curr_node].size() == 0)
        return state == ALREADY_COVERED? 0 : 1;

    if(memo[state][curr_node] != - 1)
        return memo[state][curr_node];
    
    // already covered
    int not_take = 0;
    for(auto node : tree[curr_node]){
        not_take += dp(memo, CAN_COVER, tree, node, source);
    }

    if(state == ALREADY_COVERED){
        return memo[state][curr_node] = not_take;
    }
    
    int take = curr_node == source? 0 : 1;
    for(auto node: tree[curr_node]){
        take += dp(memo, ALREADY_COVERED, tree, node, source);
    }

    return memo[state][curr_node] = max(take, not_take);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int c_centers, troops_per_comm, supervisions;
        cin >> c_centers >> troops_per_comm >> supervisions;

        vector<vector<int>> tree((c_centers * troops_per_comm) + 1);
        int source = (c_centers * troops_per_comm);
        for(int i = 0; i < supervisions; i++){
            int u, v, h;
            cin >> u >> v >> h;
            for(int j = 0; j < h; j++){
                int x, y;
                cin >> x >> y;
                tree[(u * troops_per_comm) + x].push_back(v * troops_per_comm + y);
            }
        }

        
        for(int i = 0; i < troops_per_comm; i++){
            tree[source].push_back(i);
        }

        vector<vector<int>> memo(2, vector<int>((c_centers * troops_per_comm) + 1, -1));
        
        int res = dp(memo, CAN_COVER, tree, source, source);

        cout << res << "\n";

    }
    return 0;
}