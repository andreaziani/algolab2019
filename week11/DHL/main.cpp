#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int min_cost(int startA, int startB, vector<int> &volumes, vector<int> &weights, vector<int> &p_sum_a, vector<int> &p_sum_b){
    if (startA == 0 && startB == 0)  return 0; 
    if (startA == 0 || startB == 0)  return -1; 
    int min = INT_MAX;

    for(int endA = 1; endA <= startA; endA++){
        for(int endB = 1; endB <= startB; endB++){
            
            int next_cost = min_cost(startA - endA, startB - endB, volumes, weights, p_sum_a, p_sum_b);
            if(next_cost == -1) continue;

            int costA = -endA;
            int costB = -endB;
            costA += p_sum_a[startA] - p_sum_a[endA - 1];
            costB += p_sum_b[startB] - p_sum_b[endB - 1];
            //cout << "p A " << p_sum_a[startA] - p_sum_a[endA - 1] << " s: " << startA << " " << endA << endl;
            //for(int i = 0; i < endA; i++) costA += volumes[startA - 1 - i];
            //for(int i = 0; i < endB; i++) costB += weights[startB - 1 - i];
            //cout << "p A " << costA + endA << " s: " << startA << " " << endA << endl;
            int tot_cost = costA * costB;
            min = std::min(min, tot_cost + next_cost);
        }
    }
    
    return min;
}


/*
int min_cost(vector<int> &volumes, vector<int> &weights){
    int n = volumes.size();
    vector<vector<int>> cost(n + 1, vector<int> (n + 1, -1));
    vector<int> pref_sum_a(n + 1, 0);
    vector<int> pref_sum_b(n + 1, 0);
    
    for(int i = 0; i < n; i++){
        pref_sum_a[i + 1] = pref_sum_a[i] + volumes[i];
        pref_sum_b[i + 1] = pref_sum_b[i] + weights[i];
        cout << pref_sum_a[i + 1] << " " << pref_sum_b[i + 1] << endl;
    }
    
    
    cost[0][0] = 0;

    for (int i = 1; i <= n; ++i) { // start
        for (int j = 1; j <= n; ++j) { // start
            if (i == 0 && j == 0) { continue; }
            
            int minimum = INT_MAX;;
            for (int a = 1; a <= i; ++a) { // end
                for (int b = 1; b <= j; ++b) { // end
                    int next_cost = cost[i - a][j - b];
                    if (next_cost == -1) { continue; }

                    int costA = pref_sum_a[i] - pref_sum_a[a - 1] - a;
                    int costB = pref_sum_b[j] - pref_sum_b[b - 1] - b;
                    
                    //cout << i << " " << j <<  " " << costA << " " << costB << endl;
                    int cost_tot = costA * costB;

                    int total = next_cost + cost_tot;
            
                    minimum = min(minimum, total);   
                }
            }
            
            cost[i][j] = minimum;
        }
    }

    return cost[n][n];
}
*/
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n; 
        cin >> n;

        vector<int> volumesA(n);
        vector<int> weightsB(n);

        for(int i = 0; i < n; i++)
            cin >> volumesA[i];
        
        for(int i = 0; i < n; i++)
            cin >> weightsB[i];
        
        vector<int> pref_sum_a(n + 1, 0);
        vector<int> pref_sum_b(n + 1, 0);
    
        for(int i = 0; i < n; i++){
            pref_sum_a[i + 1] = pref_sum_a[i] + volumesA[i];
            pref_sum_b[i + 1] = pref_sum_b[i] + weightsB[i];
            //cout << pref_sum_a[i + 1] << " " << pref_sum_b[i + 1] << endl;
        }

        int res = min_cost(n, n, volumesA, weightsB, pref_sum_a, pref_sum_b);

        cout << res << "\n";
    }
    return 0;
}