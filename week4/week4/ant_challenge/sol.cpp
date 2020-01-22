#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

vector<edge_desc> kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    return mst;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n_trees, n_edges, n_species, start, finish;
        cin >> n_trees >> n_edges >> n_species >> start >> finish;

        weighted_graph G(n_trees);
        weight_map weights = boost::get(boost::edge_weight, G);
        
        weighted_graph G_dijkstra(n_trees);
        weight_map weights_d = boost::get(boost::edge_weight, G_dijkstra);

        vector<vector<int>> w_species(n_edges, vector<int>(n_species, -1));
        vector<edge_desc> edge_descs;
        map<pair<int,int>, edge_desc> dijkstra_ed;

        for(int i = 0; i < n_edges; i++){
            int u, v;
            cin >> u >> v;

            for(int j = 0; j < n_species; j++)
                cin >> w_species[i][j];
            
            edge_desc e;
            e = boost::add_edge(u, v, G).first;
            edge_descs.push_back(e);
            
            edge_desc e1 = boost::add_edge(u, v, G_dijkstra).first;
            dijkstra_ed[{u, v}] = e1;
            weights_d[e1] = 10e6;
        }
        
        int skip;
        for(int k = 0; k < n_species; k++) cin >> skip;
        
        for(int i = 0; i < n_species; i++){
            for(int j = 0; j < n_edges; j++){
                weights[edge_descs[j]] = w_species[j][i];
            }
      
            vector<edge_desc> mst = kruskal(G);

            for(int j = 0; j < mst.size(); j++){
                edge_desc e = dijkstra_ed[{mst[j].m_source, mst[j].m_target}];
                if(weights_d[e] > weights[mst[j]]){
                    weights_d[e] = weights[mst[j]];
                    
                }
            }
        }

        std::vector<int> dist_map(n_trees);
        boost::dijkstra_shortest_paths(G_dijkstra, start,
            boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G_dijkstra))));

        cout << dist_map[finish] << "\n";

    }
    return 0;
}