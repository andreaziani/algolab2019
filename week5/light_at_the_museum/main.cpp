#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>
using namespace std;


void computeCombinations(vector<int> &lights_per_room, vector<vector<pair<int,int>>> &s,
                        map<vector<int>, int> &l,
                        vector<int> curr_lights, int curr, int swaps){
    
    auto it = l.find(curr_lights);
    if (it != l.end()) {
        it->second = min(it->second, swaps);
    } else {
        l.insert({curr_lights, swaps});
    }
    
    if (curr < s.size()) {
        computeCombinations(lights_per_room, s, l, curr_lights, curr + 1, swaps);
        for(int i = 0; i < curr_lights.size(); i++){ // SWAP
            curr_lights[i] = curr_lights[i] - s[curr][i].first + s[curr][i].second;
        }
        computeCombinations(lights_per_room, s, l, curr_lights, curr + 1, swaps + 1);
    }
    
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n_switches, n_rooms;
        cin >> n_switches >> n_rooms;

        vector<int> lights_per_room(n_rooms, 0);
        for(int i = 0; i < n_rooms; i++){
            cin >> lights_per_room[i];
        }

        vector<vector<pair<int,int>>> lights(n_switches, vector<pair<int,int>>(n_rooms));
        vector<vector<pair<int,int>>> s1;
        vector<vector<pair<int,int>>> s2;
        map<vector<int>, int> l1; // {brightness foreach room, n of switches swapped of s1}
        map<vector<int>, int> l2; // {brightness foreach room, n of switches swapped of s2}
        vector<int> curr_lights_1(n_rooms, 0);
        vector<int> curr_lights_2(n_rooms, 0);

        for(int i = 0; i < n_switches; i++){
            for(int j = 0; j < n_rooms; j++){
                int on, off;
                cin >> on >> off;
                lights[i][j] = {on , off};
                if(i < n_switches / 2)
                    curr_lights_1[j] += on;
                else 
                    curr_lights_2[j] += on;
            }
        }

        for(int i = 0; i < n_switches / 2; i++){
            s1.push_back(lights[i]);
        }

        for(int i = n_switches / 2; i < n_switches; i++){
            s2.push_back(lights[i]);
        }

        computeCombinations(lights_per_room, s1, l1, curr_lights_1, 0, 0);
        computeCombinations(lights_per_room, s2, l2, curr_lights_2, 0, 0);

        int g_min = INT_MAX;
        
        for(auto i = l1.begin(); i != l1.end(); i++){
            vector<int> target = lights_per_room;
            for(int j = 0; j < n_rooms; j++){
                target[j] -= i->first[j];
            }

            auto it = l2.find(target);
            if(it != l2.end()){
                g_min = min(g_min, i->second + it->second);
            }
        }

        if(g_min == INT_MAX)
            cout << "impossible\n";
        else
            cout << g_min << "\n";
    }
    return 0;
}