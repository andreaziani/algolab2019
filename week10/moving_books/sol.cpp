#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <algorithm>
using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;
        vector<int> strength(n);
        multiset<int, greater<int>> boxes;
        int w_max = 0;
        for(int i = 0; i < n; i++)
            cin >> strength[i];
        
        for(int i = 0; i < m; i++){
            int b;
            cin >> b;
            w_max = max(b, w_max);
            boxes.insert(b);

        }
        
        sort(strength.begin(), strength.end(), greater<int>());

        if(w_max > strength[0]){
            cout << "impossible\n";
            continue;
        }
        
        int rounds = 0;
        while(!boxes.empty()){
            for(int i = 0; i < n && !boxes.empty(); i++){
                auto curr_w = boxes.lower_bound(strength[i]);
                if(curr_w != boxes.end())
                    boxes.erase(curr_w);
                else 
                    break;
            }
            rounds++;
        }

        cout << rounds * 3 - 1 << "\n";
    }   
    return 0;
}