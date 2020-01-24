#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
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
        int n, m, s, d;
        cin >> n >> m >> s >> d;
        graph G(n * 2);
        auto v_source = boost::add_vertex(G);
        auto v_target = boost::add_vertex(G);
        edge_adder adder(G);

        for(int i = 0; i < n; i++) // vertex capacity for each node
            adder.add_edge(i, i + n, 1);

        for(int i = 0; i < m; i++){
            int a, b;
            cin >> a >> b;
            adder.add_edge(a + n, b, 1);
        }

        for(int i = 0; i < s; i++){
            int a;
            cin >> a;
            adder.add_edge(v_source, a, 1);
        }

        for(int i = 0; i < d; i++){
            int a;
            cin >> a;
            adder.add_edge(a + n, v_target, 1);
        }

        long flow = boost::push_relabel_max_flow(G, v_source, v_target);
        std::cout << flow << "\n";

    }
    return 0;
}