#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <set>
#include <stack>
using namespace std;

void find_path(vector<vector<int>> &g, priority_queue<pair<int, int>> max_heap, 
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> min_heap, 
    vector<pair<int, int>> &currPath, vector<long> &temps, int curr, int m, int k, int currDepth, set<int> &stamps, int start){
    
    currPath.push_back({curr, currDepth});
    max_heap.push({temps[curr], currDepth});
    min_heap.push({temps[curr], currDepth});

    if(currPath.size() - start == m){
        int min = min_heap.top().first;
        int max = max_heap.top().first;
        if(max - min <= k){
            stamps.insert(currPath[start].first);
        }

        while(max_heap.top().second <= currPath[start].second){
            max_heap.pop();
        }            
        while(min_heap.top().second <= currPath[start].second){
            min_heap.pop();
        }
        start++;
    }
    
    for(int i = 0; i < g[curr].size(); i++){
        find_path(g, max_heap, min_heap, currPath, temps, g[curr][i], m, k, currDepth + 1, stamps, start);
    }
    
    currPath.pop_back();
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        long n, m, k;
        cin >> n >> m >> k;
        vector<long> temps(n, 0);
        for(int i = 0; i < n; i++){
            cin >> temps[i];
        }

        vector<vector<int>> g(n);
        set<int> stamps;

        for(int i = 0; i < n - 1; i++){
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
        }

        priority_queue<pair<int,int>> max_heap;
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> min_heap;
        vector<pair<int, int>> curr_path;

        find_path(g, max_heap, min_heap, curr_path, temps, 0, m, k, 0, stamps, 0);

        if(stamps.size() == 0)
            cout << "Abort mission";
        else{
            for(auto it = stamps.begin(); it != stamps.end(); it++)
                cout << *it << " ";
        }
        cout << "\n";

    }
    return 0;
}