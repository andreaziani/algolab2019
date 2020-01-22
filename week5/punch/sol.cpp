#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

struct cost_diff_beverage{
    int cost, diff_bev;
    vector<int> beverages; //i-th = 0 if not taken, 1 if taken
    cost_diff_beverage(int cost, int diff_bev, vector<int> bev){
        this->cost = cost;
        this->diff_bev = diff_bev;
        this->beverages = bev;
    }
    cost_diff_beverage(){}
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, k; 
        cin >> n >> k;
        vector<pair<int,int>> bev(n); // cost, volume
        for(int i = 0; i < n; i++){
            int c, l;
            cin >> c >> l;
            bev[i] = {c, l};
        }

        vector<cost_diff_beverage> memo(k + 1, cost_diff_beverage(-1, -1, vector<int>(n, 0)));
        
        // calculate for each volume what is the best {cost, diff beverages} which we can take
        for(int curr_volume = 1; curr_volume <= k; curr_volume++){ // i = volume which we are considering
            cost_diff_beverage tmp;
            cost_diff_beverage best = cost_diff_beverage(INT_MAX, -1, vector<int>(n, 0));
            for(int i = 0; i < n; i++){
                if(curr_volume - bev[i].second > 0){
                    tmp = memo[curr_volume - bev[i].second];
                } else {
                    tmp = cost_diff_beverage(0, 0, vector<int>(n, 0));
                }

                tmp.cost += bev[i].first;
                tmp.diff_bev += tmp.beverages[i] == 1? 0 : 1; // if already taken do not add 
                tmp.beverages[i] = 1; // set as taken

                if(tmp.cost < best.cost || (tmp.cost == best.cost && tmp.diff_bev > best.diff_bev)){
                    best = tmp;
                }
            }
            memo[curr_volume] = best;
        }

        cout << memo[k].cost << " " << memo[k].diff_bev << "\n";
    }
    return 0;
}