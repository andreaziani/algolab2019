#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
using namespace boost;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, long, boost::property<boost::edge_capacity_t, long>>>     weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


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
int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int n, m, s, f; std::cin >> n >> m >> s >> f;
        flow_graph G;
        weighted_graph g;
        auto weights = boost::get(boost::edge_weight, g);
        edge_adder adder(G); 
        
        std::map<edge_desc, int> memo;
        for(int i = 0; i < m; ++i){
            int a, b, c, d; std::cin>>a>>b>>c>>d;
            edge_desc e = boost::add_edge(a, b, g).first; weights[e] = d;
            memo[e] = c;
            e = boost::add_edge(b, a, g).first; weights[e] = d;
            memo[e] = c;
        }
        
        std::vector<int> dist_map(n);
        boost::dijkstra_shortest_paths(g, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),boost::get(boost::vertex_index, g))));
        boost::graph_traits<weighted_graph>::edge_iterator ei, ei_end;
        
        for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei){
            int v_source = boost::source(*ei, g);
            int v_target = boost::target(*ei, g);
            if (weights[*ei] + dist_map[v_source] <= dist_map[v_target]){
                adder.add_edge(v_source, v_target, memo[*ei], weights[*ei]);
            }
        }
    
        long flow = boost::push_relabel_max_flow(G, s, f);
        std::cout<<flow<<"\n";
    }
    return 0;
}