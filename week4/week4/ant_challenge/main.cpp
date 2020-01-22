#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>
#include <climits>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_index_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int main(void)
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t-- > 0) {
        int n, e, s, a, b;
        cin >> n >> e >> s >> a >> b;

        Graph g(n);
        property_map<Graph, edge_index_t>::type indices = get(edge_index, g);

        vector<vector<int > > weights(s, vector<int>(e, -1));

        for(int i = 0; i < e; i++) {
            int t1, t2;
            cin >> t1;
            cin >> t2;
            bool success;
            Edge edge;
            tie(edge, success) = add_edge(t1, t2, g);
            indices[edge] = i;
            for(int j = 0; j < s; j++) cin >> weights[j][i];
        }
        // hives are useless
        int skip;
        for(int k = 0; k < s; k++) cin >> skip;
        // minimum spanning for each species
        vector<int> spanning_tree_global(e, numeric_limits<int>::max());
        for(int k = 0; k < s; k++) {
            vector<Edge> spanning_tree;
            kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree), weight_map(make_iterator_property_map(weights[k].begin(), indices)));

            for(vector<Edge>::iterator spanning_tree_edge = spanning_tree.begin(); spanning_tree_edge != spanning_tree.end(); ++spanning_tree_edge) {
                if(weights[k][indices[*spanning_tree_edge]] < spanning_tree_global[indices[*spanning_tree_edge]]) {
                    spanning_tree_global[indices[*spanning_tree_edge]] = weights[k][indices[*spanning_tree_edge]];
                }
            }
        }
        // dijkstra on global minimum spanning tree
        vector<Vertex> pred(n);
        vector<int> dist(n);
        dijkstra_shortest_paths(g, a, predecessor_map(&pred[0]).distance_map(&dist[0]).weight_map(make_iterator_property_map(&spanning_tree_global[0], indices)));
        cout << dist[b] << '\n';
    }
    return 0;
}