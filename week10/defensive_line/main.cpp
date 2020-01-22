#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
/*
//RECURSIVE FUNC IS LIKE THAT BUT GOES IN ASS FAILURE
int best_attack(int i, int j, vector<vector<int>> &memo, vector<int> len, int n){
    if(j >= n)
        return 0;
    if(memo[i][j] != -1)
        return memo[i][j];
    if(len[j] > 0)
        return memo[i][j] = max(best_attack(i, j + 1, memo, len, n), len[j] + best_attack(i - 1, j + len[j], memo, len, n));
    else 
        return memo[i][j] = best_attack(i, j + 1, memo, len, n);
}
*/
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> defenders(n);
        for(int i = 0; i < n; i++)
            cin >> defenders[i];
        
        vector<int> len(n);
        vector<pair<int,int>> segments;
        map<pair<int,int>, set<pair<int,int>>> intersections;

        int start = 0;
        int end = 0;
        int curr_sum = 0;
        while(end < n + 1 && start < n){
            if(start > end){
                end = start;
                curr_sum = 0;
            }

            if(curr_sum == k){
                len[start] = end - start;
                segments.push_back({start, end - 1});
                //cout << start << " " << end - 1 << endl;
                //cout << end - start << endl;
            }
            
            if(curr_sum < k){
                curr_sum += defenders[end++];
            } else if(curr_sum >= k){
                curr_sum -= defenders[start++];
            }
        }
        
        int disjoint_seg = 1;
        int last_end = segments[0].second;
        for(int i = 1; i < segments.size(); i++){
            if(last_end < segments[i].first){
                disjoint_seg++;
                last_end = segments[i].second;
            }

        }

        
        if(disjoint_seg < m){
            cout << "fail\n";
        } else {
            
            vector<vector<int>> memo(m, vector<int>(n, -1));
            memo[0][n - 1] = len[n - 1];
            for(int i = n - 2; i >= 0; i--)
                memo[0][i] = max(len[i], memo[0][i + 1]);

            for(int i = 1; i < m; i++){
                for(int j = n - 1; j >= 0; j--){
                    if(len[j] > 0){
                        int v1 = 0;
                        int v2 = 0;
                        if(j + 1 < n){
                            v1 = memo[i][j + 1];
                        }
                        if(j + len[j] < n){
                            v2 = memo[i - 1][j + len[j]];
                        }
                        v2 += len[j];
                        memo[i][j] = max(v1, v2);
                    } else {
                        int v1 = 0;
                        if(j + 1 < n){
                            v1 = memo[i][j + 1];
                        }
                        memo[i][j] = v1;
                    }
                }
            }
            int res = memo[m - 1][0];

            cout << res << endl;
        }
        
    }
    return 0;
}