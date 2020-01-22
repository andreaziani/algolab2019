#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

using namespace std;

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    
    while(t-- > 0){
        int n_vertices, n_edges, n_stores;
        cin >> n_vertices >> n_edges >> n_stores;

        vector<int> stores(n_stores);
        graph G(n_vertices);
        edge_adder adder(G);

        const vertex_desc v_target = boost::add_vertex(G);

        for(int i = 0; i < n_stores; i++){
            cin >> stores[i];
            //cout << stores[i] << " " << v_target << endl;
            adder.add_edge(stores[i], v_target, 1);
            adder.add_edge(v_target, stores[i], 1);
        }

        for(int i = 0; i < n_edges; i++){
            int s, d;
            cin >> s >> d;
            //cout << s << " " << d << endl;
            adder.add_edge(s, d, 1);
            adder.add_edge(d, s, 1);
        }

        int flow = boost::push_relabel_max_flow(G, 0, v_target);
        //cout << flow << endl;
        flow != n_stores ? cout << "no\n" : cout << "yes\n";
    }
    return 0;
}