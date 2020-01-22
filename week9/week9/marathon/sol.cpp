#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc_w;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc_w;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > flow_graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<flow_graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<flow_graph>::out_edge_iterator           out_edge_it; // Iterator
typedef traits::vertex_descriptor vertex_desc;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder
{
    flow_graph &G;

public:
    explicit edge_adder(flow_graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost)
    {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;      // new assign cost
        w_map[rev_e] = -cost; // new negative cost
    }
};

using namespace std;

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m, s, f;
        cin >> n >> m >> s >> f;

        weighted_graph G_dijkstra(n);
        weight_map weights = boost::get(boost::edge_weight, G_dijkstra);

        map<edge_desc_w, int> edges;
        for(int i = 0; i < m; i++){
            int a, b, c, d;
            cin >> a >> b >> c >> d;
            edge_desc e;
            e = boost::add_edge(a, b, G_dijkstra).first; weights[e] = d;
            edges[e] = c;
            e = boost::add_edge(b, a, G_dijkstra).first; weights[e] = d;
        }

        vector<int> dist_map(n);
        boost::dijkstra_shortest_paths(G_dijkstra, s,
            boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G_dijkstra))));

        edge_it e_beg, e_end;
        flow_graph G(n);
        edge_adder adder(G);
        for (boost::tie(e_beg, e_end) = boost::edges(G_dijkstra); e_beg != e_end; ++e_beg) {
            int source = boost::source(*e_beg, G_dijkstra);
            int dest = boost::target(*e_beg, G_dijkstra);
            if(weights[*e_beg] + dist_map[source] <= dist_map[dest]){
                adder.add_edge(source, dest, edges[*e_beg], weights[*e_beg]);
            }
        }

        long flow = boost::push_relabel_max_flow(G, s, f);
        std::cout << flow << "\n";
    }
    return 0;
}