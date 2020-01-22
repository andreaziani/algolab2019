#include <stdlib.h>
#include <iostream>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

struct Edge{
    int cost, s, t;
    Edge(int c, int s, int t){
        this->cost = c;
        this->s = s;
        this->t = t;
    }

    static bool sort(const Edge& a, const Edge& b) {
        return tie(a.cost, a.t, a.s) < tie(b.cost, b.t, b.s);
    };
};

pair<deque<int>, long> find_mst(int n, deque<Edge> &edges){
    boost::disjoint_sets_with_storage<> uf(n);
    int n_components = n;
    deque<int> mst;
    long costo = 0;
    for (int i = 0; i < edges.size(); i++) {
        int c1 = uf.find_set(edges[i].s);
        int c2 = uf.find_set(edges[i].t);
        if (c1 != c2) {
            costo += edges[i].cost;
            uf.link(c1, c2);
            mst.push_back(i);
            if (--n_components == 1) break;
        }
    }
    return {mst, costo};
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, tat;
        cin >> n >> tat;

        deque<Edge> edges;
        for(int i = 0; i < n - 1; i++){
            for(int k = 0; k < n - i - 1; k++){
                int cost;
                cin >> cost;
                edges.push_back(Edge(cost, i, i + k + 1));
            }
        }

        std::sort(edges.begin(), edges.end(), Edge::sort);

        pair<deque<int>, long> mst = find_mst(n, edges);

        long best_second_cost = INT64_MAX;
        for(int i = 0; i < mst.first.size(); i++){
            auto e = edges[mst.first[i]];
            edges.erase(edges.begin() + mst.first[i]);
        
            long curr_cost = find_mst(n, edges).second;
            if(curr_cost < best_second_cost)
                best_second_cost = curr_cost;
            
            edges.insert(edges.begin() + mst.first[i], e);
        }

        cout << best_second_cost << "\n";
        
    }

    return 0;
}