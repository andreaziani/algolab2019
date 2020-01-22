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
    int t; cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;

        graph G(n + m + 2);
        edge_adder adder(G);
        for(int i = 0; i < m; i++){
            int a, b, c;
            cin >> a >> b >> c;
            if(c == 0){
                adder.add_edge(i, a + m, 1);
                adder.add_edge(i, b + m, 1);
            } else if(c == 1){
                adder.add_edge(i, a + m, 1);
                adder.add_edge(i, b + m, 0);
            } else{
                adder.add_edge(i, a + m, 0);
                adder.add_edge(i, b + m, 1);
            }
            adder.add_edge(n + m, i, 1); // n+m = source
        }
        vector<int> scores(n);
        for(int i = 0; i < n; i++){
            int r; cin >> r;
            scores[i] = r;
            adder.add_edge(i + m, n + m + 1, r); //n + m + 1 = dest
        }

        int flow = boost::push_relabel_max_flow(G, n + m, n + m + 1);
        const auto c_map = boost::get(boost::edge_capacity, G);
        const auto rc_map = boost::get(boost::edge_residual_capacity, G);

        if(flow < m){
            cout << "no\n"; 
        } else {
            bool possible = true;
            for (int i = 0; i < n; i++)
            {
                edge_desc e = boost::edge(i + m, n + m + 1, G).first;
                long flow_through_edge = c_map[e] - rc_map[e];
                if (flow_through_edge != scores[i])
                {
                    possible = false;
                    break;
                }
            }

            if(possible)
                cout << "yes\n";
            else 
                cout << "no\n";
        }
    }
    return 0;
}