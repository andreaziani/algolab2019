#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS,
                       no_property, property<edge_weight_t, long>>
    weighted_graph;
typedef property_map<weighted_graph, edge_weight_t>::type w_map;
typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int n, m, k, T;
        cin >> n >> m >> k >> T;
        std::unordered_set<int> tel_net;
        for (int i = 0; i < T; i++){
            int index;
            cin >> index;
            tel_net.insert(index);
        }

        weighted_graph G(2 * n);
        w_map weights = get(edge_weight, G);

        for (int i = 0; i < m; i++) // READ GRAPH AND REVERSE EDGES
        {
            int u, v, c;
            cin >> u >> v >> c;
            edge_desc e;
            e = add_edge(v, u, G).first;
            weights[e] = c;
        }

        // scc_map[i]: index of SCC containing i-th vertex
        std::vector<int> scc_map(2*n);  // 2*N vertex only if the tel_net has size = n
        // nscc: total number of SCCs
        int nscc = boost::strong_components(G,
            boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
        
        vector<int> scc_sizes(nscc, 0);
        for(int i = 0; i < n; i++) // calculate the size of each connected components
            if(tel_net.count(i))
                scc_sizes[scc_map[i]]++;

        for(int i = 0; i < n; i++){ // for each real vertex 
            int tel_node = scc_map[i] + n; // fake node = connected comp index + n
            if(tel_net.count(i)){ // if it's part of the tel network
                edge_desc e;
                e = add_edge(tel_node, i, G).first; // add a "fake vertex"
                weights[e] = 0; // 0 for the access 
                e = add_edge(i, tel_node, G).first;
                weights[e] = scc_sizes[scc_map[i]] - 1; // size of his ssc - 1 for exit
            }
        }

        vector<long> dist_map(2 * n);
        vector<vertex_desc> pred_map(2 * n);

        dijkstra_shortest_paths(G, n - 1,
                                distance_map(make_iterator_property_map(
                                                dist_map.begin(), get(vertex_index, G)))
                                    .predecessor_map(make_iterator_property_map(
                                        pred_map.begin(), get(vertex_index, G))));
        
        long min = 1000001;
        for(int i = 0; i < k; i++){
            if(dist_map[i] < min) 
                min = dist_map[i];
        }

        if(min == 1000001) cout << "no\n";
        else cout << min << "\n";
        
    }
    return 0;
}