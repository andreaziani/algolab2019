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
        long n; cin >> n;
        vector<long> bombs(n);
        for(int i = 0; i < n; i++){
            cin >> bombs[i];
        }

        // every bomb can be deactivated at most with min(her_timer, minimum anchestor)
        for(int i = 0; i < ((n - 1) / 2); i++){
            if(bombs[i] < bombs[(2*i+1)])
                bombs[(2*i+1)] = bombs[i];
            if(bombs[i] < bombs[(2*i+2)])
                bombs[(2*i+2)] = bombs[i];
        }

        vector<pair<long,long>> timer_index(n, {0,0});
        for(int i = 0; i < n; i++){
            timer_index[i] = {bombs[i], i};
        }

        sort(timer_index.begin(), timer_index.end(), [](pair<long,long> p1, pair<long,long> p2) -> bool{
            if(p1.first == p2.first)
                return p1.second > p2.second;
            else 
                return p1.first < p2.first;
        });

        bool explode = false;
        for(int i = 0; i < n; i++){
            if(timer_index[i].first <= i){
                explode = true;
                break;
            }
        }

        if(explode)
            cout << "no\n";
        else 
            cout << "yes\n";
    }
    return 0;
}