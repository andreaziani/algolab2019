#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <utility>
#include <set>
#include <limits>
#include <climits>
using namespace std;

#define MAX_BAR_RANGE 200

void beachBars(vector<int> v){
    sort(v.begin(), v.end());
    int max_parasols = 0; // keep track of the maximum amount of parasols
    int cur_parasol_count = 0;
    // (indexes that represent) ranges in which we can build a bar
    vector<pair<int, int> > bar_ranges;
    // keeps track of current start/stop index
    int start_index = 0;
    int stop_index = 0;
    bar_ranges.push_back(make_pair(start_index, stop_index));

    // iterate through parasols and collect bar ranges
    for(int parasol_index = 0; parasol_index < v.size(); parasol_index++) {
        // if the current parasol is too far away from the previous one, remove 
        // parasols as long as there are more between the original `start_index` and current one until we're
        // back in the range of a bar
        while(v[parasol_index] - v[start_index] > MAX_BAR_RANGE) {
            cur_parasol_count--;
            start_index++;
        }
        stop_index = parasol_index; // new "last" parasol in range of the bar
        cur_parasol_count++; // as we added the current parasol, increase the amount

        if(max_parasols < cur_parasol_count) {
            //found a range with more parasols as before!
            max_parasols = cur_parasol_count;
            bar_ranges.clear(); // remove previous
            bar_ranges.push_back(make_pair(start_index, stop_index)); // add new one
        } else if(max_parasols == cur_parasol_count) {
            // another location for the bar with equal amount of reached parasols
            bar_ranges.push_back(make_pair(start_index, stop_index));
        }
    }
    // We have a list of parasol's ranges where we can build a bar in the middle and reach maximum
    // amount of customers. We have to find the one bar with the minimum distance.
    set<int> bar_locs;
    int min_distance_found = INT_MAX;
    for(int range_index = 0; range_index < bar_ranges.size(); range_index++) {
        // `added_range / 2` is the location of the bar
        int added_range = v[bar_ranges[range_index].first] +
            v[bar_ranges[range_index].second];

        int diff = (v[bar_ranges[range_index].second] -
            v[bar_ranges[range_index].first] + 1) / 2; // +1 because of rounding

        if(diff > min_distance_found) { //ignore
            continue;
        } else if(diff < min_distance_found) { //replace everything
            min_distance_found = diff;
            bar_locs.clear();
        }
        // now add the current bar to our bar location collection, as we know it is the best one
        // or as good as the best we not till now
        if(added_range % 2 == 0) {
            bar_locs.insert(added_range / 2);
        } else {
            bar_locs.insert((added_range - 1) / 2);
            bar_locs.insert((added_range - 1) / 2 + 1);
        }
    }
    // output basic information
    cout << max_parasols << " " << min_distance_found << '\n';
    // set is already ordered
    set<int>::iterator loc_iter = bar_locs.begin();
    cout << *loc_iter++;
    for(; loc_iter != bar_locs.end(); ++loc_iter) {
        cout << " " << *loc_iter;
    }
    cout << '\n';
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int n; 
        cin >> n;
        vector<int> parasols(n);
        for(int j = 0; j < n; j++)
            cin >> parasols[j];
        beachBars(parasols);
    }
    return 0;
}