#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> def(n);
        for(int i = 0; i < n; i++)
            cin >> def[i];

        vector<int> skip(n, 0);

        int sum = 0;
        int start = 0;
        int end = 0;
        vector<pair<int,int>> segments;
        while(start <= end && end < n + 1){
            if(sum >= k){
                if(sum == k){
                    segments.push_back({start, end - 1});
                    skip[start] = end - start;
                }
                sum -= def[start];
                start++;
            } else {
                sum += def[end];
                end++;

            }
            
        }

        int d_seg = 1;
        int prev_end = segments[0].second;
        for(int i = 1; i < segments.size(); i++){
            if(prev_end < segments[i].first){
                d_seg++;
                prev_end = segments[i].second;
            }
        }
        
        if(d_seg < m){
            cout << "fail\n";
            continue;
        }

        vector<vector<int>> memo(m, vector<int>(n, 0));
        int max = skip[n - 1];
        for(int i = n - 1; i >= 0; i--){
            if(skip[i] > max)
                max = skip[i];
            memo[0][i] = max;
            
        }
        
        for(int i = 1; i < m; i++){
            for(int j = n - 1; j >= 0; j--){
                if(skip[j] != 0){
                    int v1 = 0;
                    int v2 = 0;
                    if(j + skip[j] < n)
                        v1 = skip[j] + memo[i - 1][j + skip[j]];
                    if(j + 1 < n)
                        v2 = memo[i][j + 1];

                    memo[i][j] = std::max(v1, v2);
                } else{
                    if(j + 1 < n)
                        memo[i][j] = memo[i][j + 1];
                }
            }
        }


        cout << memo[m - 1][0] << "\n";
    }
    
    return 0;
}