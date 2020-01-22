#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int k; 
        cin >> k;
        vector<int> balls;
        int n = 0;
        for(int i = 0; i < k; i++){
            for(int j = 0; j <= i; j++){
                int val;
                cin >> val;
                n++;
                balls.push_back(val);
            }
        }

        weighted_graph G(2 * n);
        weight_map weights = boost::get(boost::edge_weight, G);
        int node_index = 0;
        for(int i = 0; i < k; i++){
            for(int j = 0; j <= i; j++){
                int duplicate = node_index + n;
                edge_desc e;
                e = boost::add_edge(node_index, duplicate, G).first; weights[e] = balls[node_index];
                if(j + 1 <= i){
                    e = boost::add_edge(duplicate, node_index + 1, G).first; weights[e] = 0;
                    e = boost::add_edge(duplicate + 1, node_index, G).first; weights[e] = 0;
                }
                if(i + 1 < k){
                    e = boost::add_edge(duplicate, node_index + i + 1, G).first; weights[e] = 0;
                    e = boost::add_edge(duplicate, node_index + i + 2, G).first; weights[e] = 0;
                    e = boost::add_edge(duplicate + i + 1, node_index, G).first; weights[e] = 0;
                    e = boost::add_edge(duplicate + i + 2, node_index, G).first; weights[e] = 0;
                }
                node_index++;
            }
        }
        
        vector<int> dist_top = dijkstra_dist(G, 0);
        vector<int> dist_bot_right = dijkstra_dist(G, n - 1);
        vector<int> dist_bot_left = dijkstra_dist(G, n - (k - 1) - 1);
        
        int min = INT_MAX;
        int min_index = n;
        for(int i = n; i < 2*n; i++){
            int sum_dist = dist_bot_right[i] + dist_bot_left[i] + dist_top[i] - (2 * balls[i - n]); 
            if(sum_dist < min){
                min = sum_dist;
                min_index = i;
            }
        }
    
        cout << dist_top[min_index] + dist_bot_left[min_index] + dist_bot_right[min_index] - (2 * balls[min_index - n]) << "\n";

    }
    return 0;
}