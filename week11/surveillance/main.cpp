#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
							  boost::property<boost::edge_capacity_t, long,
											  boost::property<boost::edge_residual_capacity_t, long,
															  boost::property<boost::edge_reverse_t, traits::edge_descriptor,
																			  boost::property<boost::edge_weight_t, long>>>>>
	graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
class edge_adder
{
	graph &G;

public:
	explicit edge_adder(graph &G) : G(G) {}
	void add_edge(int from, int to, long capacity)
	{
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		const edge_desc e = boost::add_edge(from, to, G).first;
		const edge_desc rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int intersections, streets, police, photo;
        cin >> intersections >> streets >> police >> photo;
        
        int source = intersections * 2;
        int dest = intersections * 2 + 1;
        graph G(intersections * 2 + 2);
        edge_adder adder(G);

        vector<int> police_index(police);
        for(int i = 0; i < police; i++){
            cin >> police_index[i];
            adder.add_edge(source, police_index[i], 1);
            adder.add_edge(police_index[i] + intersections, dest, 1);
        } 

        for(int i = 0; i < photo; i++){
            int index;
            cin >> index;
            adder.add_edge(index, index + intersections, 1);
        }

        for(int i = 0; i < streets; i++){
            int s, d;
            cin >> s >> d;
            adder.add_edge(s, d, INT_MAX);
            adder.add_edge(s + intersections, d + intersections, 1);
        }

        long flow1 = boost::push_relabel_max_flow(G, source, dest);

        cout << flow1 << endl;

    }
    return 0;
}