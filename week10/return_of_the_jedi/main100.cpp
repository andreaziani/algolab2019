#include <iostream>
#include <deque>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

struct Edge {
    int from;
    int to;
    int cost;

    Edge(int from, int to, int cost) : from(from), to(to), cost(cost) {};
    Edge() {};

    static bool sort(const Edge& a, const Edge& b) {
        return tie(a.cost, a.to, a.from) < tie(b.cost, b.to, b.from);
    };
};

tuple<deque<int>,int> kruskal(int n, const deque<Edge>& edges) {
    int mst_cost = 0;
    deque<int> mst;
    boost::disjoint_sets_with_storage<> uf(n);
    int n_components = n;
    for (int i = 0; i < (int) edges.size(); i++) {
        auto& e = edges[i];
        int c1 = uf.find_set(e.from);
        int c2 = uf.find_set(e.to);
        if (c1 != c2) {
            uf.link(c1, c2);
            mst.push_back(i);
            mst_cost += e.cost;
            if (--n_components == 1) {
                break;
            }
        }
    }
    return make_tuple(mst, mst_cost);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;

    while(t--){
        int n, i_tatooine;
        cin >> n >> i_tatooine;
        i_tatooine--;

        deque<Edge> edges;
        for (int j = 0; j < n-1; j++) {
            for (int k = 0; k < n-j-1; k++) {
                int c;
                cin >> c;
                Edge new_edge = Edge(j, j+k+1, c);
                edges.push_back(new_edge);
            }
        }

        sort(edges.begin(), edges.end(), Edge::sort);

        deque<int> og_mst;
        tie(og_mst, ignore) = kruskal(n, edges);

        int next_cheapest = numeric_limits<int>::max();
        for (auto i : og_mst) {
            Edge e = edges[i];
            edges.erase(edges.begin() + i);

            int cost;
            tie(ignore, cost) = kruskal(n, edges);
            next_cheapest = min(cost, next_cheapest);

            edges.insert(edges.begin() + i, e);
        }

        cout << next_cheapest << endl;
    }

    return 0;
}

