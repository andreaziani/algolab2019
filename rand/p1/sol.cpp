#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
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
  void add_edge(int from, int to, long capacity, long cost) {
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
  }
};

using namespace std;

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n;
        cin >> n;
        vector<vector<int>> matrix(n, vector<int>(n, 0));

        graph G(2 * n * n);
        edge_adder adder(G);
        int dest = 2*n*n - 1;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> matrix[i][j];
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                adder.add_edge((i * n) + j, ((i * n) + j) + (n*n), 1, 0); // in to outer
                int i1 = i + 1;
                int j1 = j + 1;
                if(j1 < n)
                    adder.add_edge(((i * n) + j) + (n*n), (i * n) + j1, 1, 100 - matrix[i][j1]);
                if(i1 < n)
                    adder.add_edge(((i * n) + j) + (n*n), (i * n) + j + n, 1, 100 - matrix[i1][j]);
            }
        }

        
        adder.add_edge(0, (n*n), 2, 0);
        adder.add_edge((n * n) - 1, dest, 2, 0);

        // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
        boost::successive_shortest_path_nonnegative_weights(G, 0, dest);
        int cost = boost::find_flow_cost(G);


        cost = (100 * 4 * (n - 1)) - cost;
        cost -= matrix[n - 1][n - 1] ;
        cost += matrix[0][0];

        cout << cost << "\n";
    }
    return 0;
}