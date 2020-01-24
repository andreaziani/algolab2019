/*
    This solution takes 80 points, to improve it we need to remove some edges.
    e.g. find a bottleneck edge between cities and remove all the jumps that i cannot take in any
    case.
*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
    return e;
  }
};

#define MAX 128

struct Edge{
    edge_desc e;
    int r;
    Edge(edge_desc ed, int re){
        e = ed;
        r = re;
    }
};


int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;

        graph G(n);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        auto source = boost::add_vertex(G);
        auto dest = boost::add_vertex(G);

        edge_adder adder(G);
        vector<int> n_food;
        for(int i = 0; i < n - 1; i++){
            int c;
            cin >> c;
            n_food.push_back(c);
            adder.add_edge(i, i + 1, c, MAX);
            adder.add_edge(source, i, c, 0);
        }

        int i = 1;
        for(auto e : n_food){
            adder.add_edge(i, dest, e, 0);
            i++;
        }

        vector<Edge> interesting_edges; 
        for(int i = 0; i < m; i++){
            int a, b, c;
            cin >> a >> b >> c;
            interesting_edges.push_back(Edge(adder.add_edge(a, b, 1, MAX * (b - a) - c), c));
        }


        boost::successive_shortest_path_nonnegative_weights(G, source, dest);
        long res = 0;
        for(auto e : interesting_edges){
            if(c_map[e.e] - rc_map[e.e] > 0){
                res += e.r;
            }
        }

        cout << res << "\n";
    }
    return 0;
}