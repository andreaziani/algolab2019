#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool isBetween(int a, int b, int target, int m){
    if(a > b){
        b += m;
        if(target < a)
            target += m;
    }
    return a <= target && target <= b;
}

bool intersect(int a, int b, int a1, int b1, int m){
    if(isBetween(a, b, a1, m) || isBetween(a, b, b1, m) || isBetween(a1, b1, a, m) || isBetween(a1, b1, b, m))
        return true;
    return false;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;
        vector<pair<int, int>> jedis(n);
        vector<pair<int,bool>> sums;

        for(int i = 0; i < n; i++){
            int a, b;
            cin >> a >> b;
            jedis[i] = {a, b};
            sums.push_back({a, true});
            sums.push_back({b + 1, false});
        }

        sort(sums.begin(), sums.end());
        sort(jedis.begin(), jedis.end(), [](pair<int,int> &p1, pair<int,int> &p2) -> bool{ return p1.second < p2.second;});

        int partial_sum = 0;
        int min = 0;
        int sector = 1;
        // find sector with min number of overlapped jedis
        for(int i = 0; i < sums.size(); i++){ 
            if(sums[i].second){
                partial_sum++; // a jedi starts his segment, increment p_sum
            } else{
                partial_sum--; // a jedi ended his segment, decrement p_sum
                if(partial_sum < min){ // update min and sector if less intersections
                    min = partial_sum;
                    sector = sums[i].first;
                }
            }
        }


        vector<int> jedis_in_min_intersection; // index of jedi in min intersection sector
        for(int i = 0; i < n; i++){
            if(isBetween(jedis[i].first, jedis[i].second, sector, m)){
                jedis_in_min_intersection.push_back(i);
            }
        }

        int max = 0;
        
        //Try to take one jedi and discard the other of the intersection point
        for(int i = 0; i < jedis_in_min_intersection.size(); i++){
            int seg_start = jedis[jedis_in_min_intersection[i]].first;
            int seg_end = jedis[jedis_in_min_intersection[i]].second;
            int curr_max = 1;

            for(int j = jedis_in_min_intersection[i] + 1; j < n; j++){ // +1 because we already consider the first jedi
                if(!intersect(jedis[j].first, jedis[j].second, seg_start, seg_end, m)){
                    curr_max++;
                    seg_end = jedis[j].second;
                }
            }

            for(int j = 0; j < jedis_in_min_intersection[i]; j++){
                if(!intersect(jedis[j].first, jedis[j].second, seg_start, seg_end, m)){
                    curr_max++;
                    seg_end = jedis[j].second;
                }
            }

            if(max < curr_max)
                max = curr_max;
        }

        // Do not take overlapped jedis = take the first jedi which ends after the min intersection point and does not overlap
        int found_idx = 0;
        while(found_idx < n && jedis[found_idx].second <= sector) // first jedi which end after sector
            found_idx++;

        found_idx = found_idx % n; // if idx == n, idx = 0

        while(found_idx < n && isBetween(jedis[found_idx].first, jedis[found_idx].second, sector, m)) // first jedi not overlapped to the sector
            found_idx++;

        found_idx = found_idx % n;

        while(found_idx < n && isBetween(jedis[found_idx].first, jedis[found_idx].second, sector, m)) // additional check to be sure to have considered all possible jedis
            found_idx++;

        found_idx = found_idx % n;


        int seg_start = jedis[found_idx].first;
        int seg_end = jedis[found_idx].second;
        int curr_max = 1;

        for(int j = found_idx + 1; j < n; j++){ // +1 because we already consider the first jedi
            if(!intersect(jedis[j].first, jedis[j].second, seg_start, seg_end, m)){
                curr_max++;
                seg_end = jedis[j].second;
            }
        }

        for(int j = 0; j < found_idx; j++){
            if(!intersect(jedis[j].first, jedis[j].second, seg_start, seg_end, m)){
                curr_max++;
                seg_end = jedis[j].second;
            }
        }

        if(max < curr_max)
            max = curr_max;

        cout << max << "\n";
    }
}