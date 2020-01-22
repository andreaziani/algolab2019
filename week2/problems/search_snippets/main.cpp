#include <stdlib.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <climits>
#include <set>
using namespace std;

int search_snippets(vector<vector<int> > pos, vector<int> occ){
    vector<pair<int,int> > p;
    vector<int> visited(occ.size(), 0);
    set<int> s;
    for(int i = 0; i < occ.size(); i++)
        for(int j = 0; j < pos[i].size(); j++)
           p.push_back(pair<int,int>(pos[i][j], i));
    sort(p.begin(), p.end());
    int res = INT_MAX;
    int start = 0;
    int end = occ.size() - 1;
    //cout << end << "\n";
    for(int i = 0; i < occ.size(); i++){
        visited[p[i].second]++;
        s.insert(p[i].second);
    }
    
    while(start <= end && end < p.size()){
        //cout << start << " " << end << " " << p[start].first << " " << p[end].first << " res: " << res <<  "  " << p.size() << endl ;
        if(s.size() == occ.size()){
            if(res > (p[end].first - p[start].first + 1)) res = p[end].first - p[start].first + 1;
            visited[p[start].second]--;
            if(visited[p[start].second] == 0) s.erase(p[start].second);
            start++;
        } else{
            end++;
            if(end < p.size()){ 
                visited[p[end].second]++;
                s.insert(p[end].second);
            }
        }
    }
    return res;
}

int main(){ 
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    for(int i = 0; i < t; i++){
        int n; 
        cin >> n;
        vector<int> occ(n, 0);
        for(int j = 0; j < n; j++){
            cin >> occ[j];
        }
        vector<vector<int> > pos;
        for(int j = 0; j < n; j++){
            vector<int> tmp(occ[j]);
            for(int k = 0; k < occ[j]; k++)
                cin >> tmp[k];
            pos.push_back(tmp);
        }
        cout << search_snippets(pos, occ) << '\n';
    }
    return 0; 
}