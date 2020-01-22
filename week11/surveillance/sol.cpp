#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
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
    int t; cin >> t;
    while(t--){
        int n, m, k, l;
        cin >> n >> m >> k >> l;
        graph G(n * 2 + 2); 
        edge_adder adder(G);

        int source = n * 2;
        int dest = n * 2 + 1;

        vector<int> police_stations(n);
        vector<int> photo(n);
        for(int i = 0; i < k; i++){
            int p;
            cin >> p;
            adder.add_edge(source, p, 1);
            adder.add_edge(p + n, dest, 1);
        }  

        for(int i = 0; i < l; i++){
            int p; cin >> p;
            adder.add_edge(p, p + n, 1);
        }

        for(int i = 0; i < m; i++){
            int x, y;
            cin >> x >> y;
            adder.add_edge(x, y, INT64_MAX);
            adder.add_edge(x + n, y + n, 1);               
        }

        long flow = boost::push_relabel_max_flow(G, source, dest);
        std::cout << flow << "\n";
        
    }
    return 0;
}