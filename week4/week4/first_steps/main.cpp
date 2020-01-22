#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;

void kruskal(const weighted_graph &G, const weight_map & weights, int n) {
    std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    
    int sum = 0;
    int maxD = 0;
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        sum += boost::get(weights, *it);
        //std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
    }
    vector<int>         dist_map(n);
    vector<vertex_desc> pred_map(n);

    boost::dijkstra_shortest_paths(G, 0,
        boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G)))
        .predecessor_map(boost::make_iterator_property_map(
        pred_map.begin(), boost::get(boost::vertex_index, G))));
    for(int i = 1; i < n; i++){
        maxD = maxD < dist_map[i]? dist_map[i] : maxD;
    }
    cout << sum << " " << maxD << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t-- > 0){
        int n, m;
        cin >> n >> m;
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);
        for(int i = 0; i < m; i++){
            int s, d, w; 
            cin >> s >> d >> w;
            edge_desc e;
            e = boost::add_edge(s, d, G).first; weights[e] = w;
        }
        kruskal(G, weights, n);
    }
    return 0;
}