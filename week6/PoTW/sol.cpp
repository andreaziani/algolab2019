#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator                         edge_it;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

struct Edge{
    int u, v, c;
    Edge(int u, int v, int c){
        this->u = u;
        this->v = v;
        this->c = c;
    }
    Edge(){}
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, m, k, T;
        cin >> n >> m >> k >> T;
        vector<int> t_network(T);
        for(int i = 0; i < T; i++)
            cin >> t_network[i];

        vector<Edge> edges;
        graph G(n);
        
        for(int i = 0; i < m; i++){
            int u, v, c; cin >> u >> v >> c;
            edges.emplace_back(u, v, c);
            boost::add_edge(v, u, G);
        }

        std::vector<int> scc_map(n);
        int nscc = boost::strong_components(G,
            boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
        std::vector<int> c(nscc, 0); // n of linked planets per component

        for(int i = 0; i < T; i++)
            c[scc_map[t_network[i]]]++;

        weighted_graph G_d(n + 1 + nscc);
        weight_map weights = boost::get(boost::edge_weight, G_d);
        
        for(int i = 0; i < m; i++){ // reverse edge between planets
            auto curr = edges[i];
            edge_desc e;
            e = boost::add_edge(curr.v, curr.u, G_d).first; weights[e] = curr.c;
        }

        for(int i = 0; i < k; i++){ // edge to global dest
            edge_desc e;
            e = boost::add_edge(i, n, G_d).first; weights[e] = 0;
        }
        
        for(int i = 0; i < T; i++){ // vertex of teleport starts at n + 1
            edge_desc e;
            e = boost::add_edge(t_network[i], n + 1 + scc_map[t_network[i]], G_d).first; weights[e] = c[scc_map[t_network[i]]] - 1;
            e = boost::add_edge(n + 1 + scc_map[t_network[i]], t_network[i], G_d).first; weights[e] = 0;
        }

        std::vector<long> dist_map(n + 1 + nscc);
        boost::dijkstra_shortest_paths(G_d, n - 1,
            boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

        long res = dist_map[n];

        if(res <= 1e6)
            cout << res << "\n";
        else 
            cout << "no\n";
    }
    

    return 0;
}