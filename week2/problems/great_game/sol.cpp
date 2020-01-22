#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int min_moves(vector<vector<int>> &g, vector<int> &memo_min, vector<int> &memo_max, int pos, int target, int moves);
int max_moves(vector<vector<int>> &g, vector<int> &memo_min, vector<int> &memo_max, int pos, int target, int moves);

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;
        vector<vector<int>> g(n);

        int red, black;
        cin >> red >> black;
        red--, black--;

        for(int i = 0; i < m; i++){
            int u, v;
            cin >> u >> v;
            g[--u].push_back(--v);
        }

        vector<int> memo_min(n, -1); // moves from i-th pos on
        vector<int> memo_max(n, -1);

        // it's like to play 2 different games for each figure, at the end we compare the n. of moves to establish the winner
        int moves_r = min_moves(g, memo_min, memo_max, red, n - 1, 0);
        int moves_b = min_moves(g, memo_min, memo_max, black, n - 1, 0); // both they want to minimize on their even move

        if(moves_r < moves_b) cout << "0\n";      // when there are less moves to red than sherlock wins
        else if(moves_r > moves_b) cout << "1\n"; // otherwise moriarty
        else if(moves_r % 2 != 0) cout << "0\n";  // if num. of moves is equal. Than we need to check with which figure Holmes moved in last move.
        else cout << "1\n";                  
    }
    return 0;
}

int min_moves(vector<vector<int>> &g, vector<int> &memo_min, vector<int> &memo_max, int pos, int target, int moves){
    if(pos == target)
        return moves;
    
    if(memo_min[pos] != -1)
        return memo_min[pos] + moves;


    int min = INT_MAX;
    for(int i = 0; i < g[pos].size(); i++){
        min = std::min(min, max_moves(g, memo_min, memo_max, g[pos][i], target, moves + 1));
    }
    memo_min[pos] = min - moves; // min moves from this position on 
    
    return min;
}

int max_moves(vector<vector<int>> &g, vector<int> &memo_min, vector<int> &memo_max, int pos, int target, int moves){
    if(pos == target)
        return moves;
    
    if(memo_max[pos] != -1)
        return memo_max[pos] + moves;

    int max = INT_MIN;
    for(int i = 0; i < g[pos].size(); i++){
        max = std::max(max, min_moves(g, memo_min, memo_max, g[pos][i], target, moves + 1));
    }
    memo_max[pos] = max - moves; // max moves from this position on
    
    return max;
}