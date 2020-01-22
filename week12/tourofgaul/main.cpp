#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                                                                              boost::property<boost::edge_weight_t, long>>>>>
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator
typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
// Custom edge adder class
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    edge_desc add_edge(int from, int to, long capacity, long cost)
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
        return e;
    }
};

struct Edge {
    int source, dest, cap, rew;
    Edge(int source, int dest, int cap, int rew){
        this->source = source;
        this->dest = dest;
        this->cap = cap;
        this->rew = rew;
    }

    Edge(){}
};

using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    
    while(t--){
        int stops, food;
        cin >> stops >> food;
        graph G(stops);
        auto source = boost::add_vertex(G);
        auto dest = boost::add_vertex(G);

        auto c_map = boost::get(boost::edge_capacity, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        
        edge_adder adder(G);
        vector<Edge> direct_edges(stops - 1);
        for(int i = 0; i < stops - 1; i++){
            int c; 
            cin >> c;
            direct_edges[i] = Edge(i, i + 1, c, 128);
            adder.add_edge(i, i + 1, c, 128);
            adder.add_edge(source, i, c, 0);
            adder.add_edge(i + 1, dest, c, 0);
        }

        map<pair<int,int>, vector<Edge>> same_path;
        for(int i = 0; i < food; i++){
            int a, b, d;
            cin >> a >> b >> d;
            same_path[{a, b}].push_back(Edge(a, b, 1, d));
        }

        vector<pair<edge_desc, int>> interesting_edges; 
        for(auto it = same_path.begin(); it != same_path.end(); it++){
            int start = it->first.first;
            int end = it->first.second;
            //cout << start << " " << end << endl;
            int bottleneck = direct_edges[start].cap;
            vector<Edge> tmp = it->second;
            while(start < end){
                if(bottleneck > direct_edges[start].cap)
                    bottleneck = direct_edges[start].cap;
                start++;
            }

            if(tmp.size() > bottleneck && tmp.size() > 1){
                sort(tmp.begin(), tmp.end(), [](Edge &e1, Edge &e2) -> bool {return e1.rew > e2.rew;});
                tmp.erase(tmp.begin() + bottleneck, tmp.end());
            }
            
            for(int i = 0; i < tmp.size(); i++){
                auto ed = adder.add_edge(tmp[i].source, tmp[i].dest, tmp[i].cap, (tmp[i].dest - tmp[i].source) * 128 - tmp[i].rew);
                int cost = tmp[i].rew;
                interesting_edges.emplace_back(ed, cost);
            }
            //cout << endl;
        }

        boost::successive_shortest_path_nonnegative_weights(G, source, dest);
        long cost = 0;
        for(auto &e : interesting_edges) {
            if(rc_map[e.first] == 0){
                cost += e.second;
            }
        }
        cout << cost << "\n";

    }

    return 0;
}