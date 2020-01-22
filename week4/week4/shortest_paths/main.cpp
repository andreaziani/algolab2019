#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n, m, q;
    cin >> n >> m >> q;
    vector<pair<int,int> > coord(n);
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    for(int i = 0; i < n; i++){
        cin >> coord[i].first >> coord[i].second;
    }
    for(int i = 0; i < m; i++){
        int a, b, c;
        cin >> a >> b >> c;
        edge_desc e;
        e = boost::add_edge(a, b, G).first; weights[e] = c;
    }

    for(int i = 0; i < q; i++){
        int s, t;
        cin >> s >> t;
        int res = dijkstra_dist(G, s, t);
        if(res != INT_MAX) cout << res << '\n';
        else cout << "unreachable" << '\n';
    }

    return 0;
}