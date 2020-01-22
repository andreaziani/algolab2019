#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>
using namespace std;
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

vector<edge_desc> kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;
  std::vector<int> predecessor_map;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  return mst;
}

vector<pair<int,int>> get_predecessors(int source, vector<vector<int>> mst, vector<int> &depth){
    vector<bool> visited(mst.size(), false);
    vector<pair<int,int>> predecessors(mst.size(), {-1, -1});
    queue<int> q;
    q.push(source);
    visited[source] = true;
    while(!q.empty()){
        int c = q.front(); q.pop();
        for(int i = 0; i < mst[c].size(); i++){
            if(mst[c][i] != -1 && visited[i] == false){
                q.push(i);
                predecessors[i] = {c, mst[c][i]};
                depth[i] = depth[c] + 1;
                visited[i] = true;
            }
        }
    }
    return predecessors;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    while(t--){
        int n, i;
        cin >> n >> i;
        i--; // 0-based index 
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);
        for(int j = 1; j <= n - 1; j++){
            for(int k = 1; k <= n - j; k++){
                int val; cin >> val;
                edge_desc e;
                e = boost::add_edge(j - 1, j + k - 1, G).first; weights[e] = val; // 0-based index
            }
        }

        vector<edge_desc> mst_edges = kruskal(G);
        std::set<pair<int,int>> set_mst;
        vector<vector<int>> mst_adj(n, vector<int>(n, -1));
        long mst_cost = 0;
        
        for (std::vector<edge_desc>::iterator it = mst_edges.begin(); it != mst_edges.end(); ++it) {
            set_mst.insert({it->m_source, it->m_target});
            mst_adj[it->m_source][it->m_target] = weights[*it];
            mst_adj[it->m_target][it->m_source] = weights[*it];
            mst_cost += weights[*it];
        }
        vector<int> depth(n, 0);
        vector<pair<int,int>> predecessors = get_predecessors(0, mst_adj, depth); //{index of prev, weight of edge}
        
        long min_cost = LONG_MAX;
        boost::graph_traits<weighted_graph>::edge_iterator ei, ei_end;
        
        // add one-by-one each edge not already in mst
        for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei){
            if(set_mst.count({ei->m_source, ei->m_target}) == 0 && set_mst.count({ei->m_target, ei->m_source}) == 0){
                long edge_weight = weights[*ei];
                long max_weight_in_cycle = 0;
                
                long depth_source = depth[ei->m_source];
                long depth_dest = depth[ei->m_target];
                
                int start1;
                int start2;
                if(depth_dest > depth_source){
                    int diff = depth_dest - depth_source;
                    start1 = ei->m_target;
                    start2 = ei->m_source;
                    while(diff--){
                        if(max_weight_in_cycle < predecessors[start1].second)
                            max_weight_in_cycle = predecessors[start1].second;
                        start1 = predecessors[start1].first;
                    }
                } else{
                    int diff = depth_source - depth_dest;
                    start1 = ei->m_source;
                    start2 = ei->m_target;
                    while(diff--){
                        if(max_weight_in_cycle < predecessors[start1].second)
                            max_weight_in_cycle = predecessors[start1].second;
                        start1 = predecessors[start1].first;
                    }
                }

                // repeat until reach LCA or root
                while(start1 != start2){ 
                    if(max_weight_in_cycle < predecessors[start1].second)
                            max_weight_in_cycle = predecessors[start1].second;
                    if(max_weight_in_cycle < predecessors[start2].second)
                            max_weight_in_cycle = predecessors[start2].second;
                    start1 = predecessors[start1].first;
                    start2 = predecessors[start2].first;
                }

                long curr_cost = mst_cost - max_weight_in_cycle + edge_weight;
                if(min_cost > curr_cost)
                    min_cost = curr_cost;
            }
        }
        cout << min_cost << "\n";
    }
    return 0;
}