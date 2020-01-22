/*
    The key idea of this problem is to model the graph in a smart way: 
    Minimize the sum of rejected jobs plus the sum of purchased tickets so that you do not have an edge between an accepted job and nonpurchased ticket. 
    Reminds the property that the maximum s-t flow equals the minimum s-t cut. 
    So the goal is designing a network in which a minimum s-t cut = the minimum sum of rejected jobs plus the sum of purchased tickets.

    Still, if you are not conviced try to think that with this graph model:
    -   the max flow is equal to the amount of money you will (for sure) spend in tickets 
        and not considering (not gain) because it is not worth to take this ticket for the optimal solution. 
    -   so the optimal = max_possible_gain - secure_expenses_and_discards
*/
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
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

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n_zones, n_jobs;
        cin >> n_zones >> n_jobs;

        graph G(n_zones + n_jobs + 2);
        edge_adder adder(G);
        int source = n_zones + n_jobs;
        int dest = n_zones + n_jobs + 1;
        int max_possible_reward = 0;
        for(int i = 0 ; i < n_zones; i++){
            int cost;
            cin >> cost;
            adder.add_edge(source, i, cost);
        }

        for(int i = 0; i < n_jobs; i++){
            int reward;
            cin >> reward;
            max_possible_reward += reward;
            adder.add_edge(i + n_zones, dest, reward);
        }
        
        for(int i = 0; i < n_jobs; i++){
            int n;
            cin >> n;
            for(int j = 0; j < n; j++){
                int z;
                cin >> z;
                adder.add_edge(z, i + n_zones, INT_MAX);
            }
        }

        long flow = boost::push_relabel_max_flow(G, source, dest);
        cout << max_possible_reward - flow << "\n";

    }
    return 0;
}