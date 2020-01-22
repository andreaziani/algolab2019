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

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int n_players, rounds;
        cin >> n_players >> rounds;
        
        vector<int> scores(n_players);
        graph G(n_players + rounds);
        const vertex_desc v_source = boost::add_vertex(G);
        const vertex_desc v_target = boost::add_vertex(G);

        edge_adder adder(G);
        
        for (int i = 0; i < rounds; i++)
        {
            int a, b, c;
            cin >> a >> b >> c;
            int point1 = 1, point2 = 1;
            int round_node = i + n_players;
            if (c == 1) {
                adder.add_edge(round_node, a, point1);
            }
            else if (c == 2){
                adder.add_edge(round_node, b, point2);
            } else {
                adder.add_edge(round_node, a, point1);
                adder.add_edge(round_node, b, point2);
            }    
            adder.add_edge(v_source, round_node, 1);
        }

        for(int i = 0; i < n_players; i++)
            cin >> scores[i];
        
        for (int i = 0; i < n_players; i++)
            adder.add_edge(i, v_target, scores[i]);
        

        int flow = boost::push_relabel_max_flow(G, v_source, v_target);
        const auto c_map = boost::get(boost::edge_capacity, G);
        const auto rc_map = boost::get(boost::edge_residual_capacity, G);
        int i;

        for (i = 0; i < n_players; i++)
        {
            edge_desc e = boost::edge(i, v_target, G).first;
            long flow_through_edge = c_map[e] - rc_map[e];
            if (flow_through_edge != scores[i] || flow < rounds)
            {
                cout << "no\n";
                break;
            }
        }
        if (i == n_players)
            cout << "yes\n";
    }
    return 0;
}
