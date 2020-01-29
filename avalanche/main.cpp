#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;


// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc_ed;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

bool feasibleTime(vector<int> &agents, vector<int> &shelters, weighted_graph &G, int time, int escape_time, int cap){
    int shelter_total = (shelters.size() * cap);
    graph G_ed(agents.size() + shelter_total);

    for(int i = 0; i < agents.size(); i++){
        int n = boost::num_vertices(G);
        std::vector<int> dist_map(n);

        boost::dijkstra_shortest_paths(G, agents[i],
            boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));
        
        for(int j = 0; j < shelters.size(); j++){
            for(int k = 0; k < cap; k++){
                if(dist_map[shelters[j]] <= time - (k + 1) * escape_time)
                    boost::add_edge(i, j + agents.size() + (shelters.size() * k), G_ed);
            }
        }
        
    }

    int n1 = boost::num_vertices(G_ed);
    std::vector<vertex_desc> mate_map(n1);  // exterior property map
    
    boost::edmonds_maximum_cardinality_matching(G_ed,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G_ed)));
    int matching_size = boost::matching_size(G_ed,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G_ed)));

    return matching_size == agents.size();
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m, a, s, c, d;
        cin >> n >> m >> a >> s >> c >> d;
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);
        
        int max_time = 0;
        for(int i = 0; i < m; i++){
            char w;
            int x, y, z;
            cin >> w >> x >> y >> z;

            edge_desc e;
            e = boost::add_edge(x, y, G).first; weights[e] = z;
            max_time += z;
            if(w == 'L'){
                e = boost::add_edge(y, x, G).first; weights[e] = z;
            }
        }

        vector<int> agents(a);
        for(int i = 0; i < a; i++)
            cin >> agents[i];
        
        vector<int> shelters(s);
        for(int i = 0; i < s; i++)
            cin >> shelters[i];

        int i = 0;
        for(; i < (max_time + 2 * d); i++)
            if(feasibleTime(agents, shelters, G, i, d, c))
                break;
            
        cout << i << "\n";

    }
    return 0;
}