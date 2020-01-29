/*
    80 point solution, to improve this some edges should be removed as in the solution "main.cpp". 
    The idea is to keep only the edges between times which are used by each station.
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

struct travel{
    int source, dest, departure, arrival, profit;
    travel(int s, int d, int de, int a, int p){
        source = s;
        dest = d;
        departure = de;
        arrival = a;
        profit = p;
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, s;
        cin >> n >> s;

        vector<int> init_cars(s);
        for(int i = 0; i < s; i++)
            cin >> init_cars[i];
        
        vector<travel> travels;
        set<int> times;
        for(int i = 0; i < n; i++){
            int s, d, dep, arr, p;
            cin >> s >> d >> dep >> arr >> p;
            travels.emplace_back(s - 1, d - 1, dep, arr, p);
            times.insert(dep);
            times.insert(arr);
        }

        map<int,int> t_index;
        int idx = 0;
        for(auto &i : times){
            t_index[i] = idx;
            idx++;
        }

        graph G(s * times.size());
        auto v_source = boost::add_vertex(G);
        auto v_target = boost::add_vertex(G);
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        for(int i = 0; i < s; i++){
            adder.add_edge(v_source, i * times.size(), init_cars[i], 0);
            adder.add_edge(i * times.size() + (times.size() - 1), v_target, INT_MAX, 0);
        }

        for(int i = 0; i < s; i++){
            for(int j = 0; j < times.size() - 1; j++){
                adder.add_edge(j + (i * times.size()), (j + 1) + (i * times.size()), INT32_MAX, 100); // i *  times.size() => right row
            }
        }

        vector<edge_desc> int_edges;
        for(int i = 0; i < n; i++){
            int s_source = travels[i].source;
            int s_dest = travels[i].dest;
            int t1 = t_index[travels[i].departure];
            int t2 = t_index[travels[i].arrival];
            int_edges.push_back(adder.add_edge(t1 + (s_source * times.size()), t2 + (s_dest * times.size()), 1, 100 * (t2 - t1) - travels[i].profit));
        }

        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);

        long profit_tot = 0;
        for(int i = 0; i < n; i++){
            auto e = int_edges[i];
            if(c_map[e] - rc_map[e] > 0){
                profit_tot += travels[i].profit;
            }
        }

        cout << profit_tot << "\n";

    }
    return 0;
}