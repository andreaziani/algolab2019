#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor            edge_desc_g;
using namespace std;


#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
  }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;
        Graph G(n);
        weighted_graph wg(n);
        weight_map weights = boost::get(boost::edge_weight, wg);

        map<edge_desc, edge_desc_g> edge_mapping;
        for(int i = 0; i < m; i++){
            int e1, e2;
            cin >> e1 >> e2;
            if(e1 > e2)
                swap(e1, e2);
            
            auto e = boost::add_edge(e1, e2, G).first;

            auto eg = boost::add_edge(e1, e2, wg).first;
            weights[eg] = 1;
            edge_mapping[eg] = e;
        }
        
        std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
        boost::kruskal_minimum_spanning_tree(wg, std::back_inserter(mst));

        vector<edge_desc_g> critical_b;
        for(auto &e: mst){
            boost::remove_edge(edge_mapping[e], G);
            std::vector<int> c(num_vertices(G));
            int num = boost::connected_components(G, boost::make_iterator_property_map(c.begin(), get(boost::vertex_index, G), c[0]));

            if(num != 1){
                critical_b.push_back(e);
            }
            
            boost::add_edge(edge_mapping[e].m_source, edge_mapping[e].m_target, G);
        }

        sort(critical_b.begin(), critical_b.end(), [](edge_desc_g &e1, edge_desc_g &e2) -> bool {
            if(e1.m_source == e2.m_source)
                return e1.m_target < e2.m_target;
            return e1.m_source < e2.m_source;
        });

        cout << critical_b.size() << "\n";
        for(auto &e : critical_b)
            cout << e.m_source << " " << e.m_target << "\n";

        
    }
    return 0;
}