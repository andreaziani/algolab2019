// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include </graph/adjacency_list.hpp>
#include </graph/dijkstra_shortest_paths.hpp>

typedef ::adjacency_list<::vecS, ::vecS, ::directedS,
  ::no_property, ::property<::edge_weight_t, int> >      weighted_graph;
typedef ::property_map<weighted_graph, ::edge_weight_t>::type weight_map;
typedef ::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef ::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = ::num_vertices(G);
  std::vector<int> dist_map(n);

  ::dijkstra_shortest_paths(G, s,
    ::distance_map(::make_iterator_property_map(
      dist_map.begin(), ::get(::vertex_index, G))));

  return dist_map[t];
}

int dijkstra_path(const weighted_graph &G, int s, int t, std::vector<vertex_desc> &path) {
  int n = ::num_vertices(G);
  std::vector<int>         dist_map(n);
  std::vector<vertex_desc> pred_map(n);

  ::dijkstra_shortest_paths(G, s,
    ::distance_map(::make_iterator_property_map(
      dist_map.begin(), ::get(::vertex_index, G)))
    .predecessor_map(::make_iterator_property_map(
      pred_map.begin(), ::get(::vertex_index, G))));

  int cur = t;
  path.clear(); path.push_back(cur);
  while (s != cur) {
    cur = pred_map[cur];
    path.push_back(cur);
  }
  std::reverse(path.begin(), path.end());
  return dist_map[t];
}

int main()
{
  weighted_graph G(4);
  weight_map weights = ::get(::edge_weight, G);

  edge_desc e;
  e = ::add_edge(0, 1, G).first; weights[e]=0;
  e = ::add_edge(1, 2, G).first; weights[e]=2;
  e = ::add_edge(2, 3, G).first; weights[e]=1;
  e = ::add_edge(3, 0, G).first; weights[e]=3;
  e = ::add_edge(0, 2, G).first; weights[e]=3;

  std::vector<vertex_desc> path;
  std::cout << dijkstra_dist(G, 0, 2) << "\n";
  std::cout << dijkstra_path(G, 0, 2, path) << "\n";

  for (int i = 0; i < (int)path.size(); i++) std::cout << path[i] << " ";
  std::cout << "\n";

  return 0;
}
