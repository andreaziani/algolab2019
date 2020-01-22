#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
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

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int num_of_tests;
    cin >> num_of_tests;

    while(num_of_tests > 0) {
      int n, m;
      cin >> n >> m;

      graph G(n+m);
      edge_adder adder(G);
      const vertex_desc v_source = boost::add_vertex(G);
      const vertex_desc v_target = boost::add_vertex(G);

      vector<int> final_scores(n, 0);
      int flow_to_reach = 0;
      for(int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        int round_node = n + i;
        switch(c) {
          case 0:
            adder.add_edge(round_node, a, 1);
            adder.add_edge(round_node, b, 1);
            adder.add_edge(v_source, round_node, 1);
            flow_to_reach++;
            break;
          case 1: 
            final_scores[a]--;
            break;
          case 2:
            final_scores[b]--;
            break;
        }
      }

        bool buonafede = true;

      for(int i = 0; i < n; i++) {
        int final_score;
        cin >> final_score;
        final_scores[i] += final_score;

            if(final_scores[i] < 0) {
                i++;
                while(i < n) {
                    cin >> final_score;
                    i++;
                }
                buonafede = false;
            }
            else adder.add_edge(i, v_target, final_scores[i]);
      }

        if(buonafede) {
            long max_flow = boost::push_relabel_max_flow(G, v_source, v_target);
            if(max_flow < flow_to_reach)
              buonafede = false;
        }

        if(buonafede) {
             const auto rc_map = boost::get(boost::edge_residual_capacity, G);

             for(int i = 0; i < n; i++) {
                 edge_desc edge = boost::edge(i, v_target, G).first;
                 if(rc_map[edge] != 0) {
                  buonafede = false;
                     i = n;
                 }
            }
        }

    if(buonafede) cout << "yes\n";
    else cout << "no\n";

      num_of_tests--;
    }
}