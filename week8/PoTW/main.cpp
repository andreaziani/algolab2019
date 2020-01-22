#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
							  boost::property<boost::edge_capacity_t, long,
											  boost::property<boost::edge_residual_capacity_t, long,
															  boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
	graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder
{
	graph &G;

public:
	explicit edge_adder(graph &G) : G(G) {}

	void add_edge(int from, int to, long capacity)
	{
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		const auto e = boost::add_edge(from, to, G).first;
		const auto rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t-- > 0){
        int z, j;
        cin >> z >> j;
        graph G(z + j);
		edge_adder adder(G);
        const vertex_desc v_source = boost::add_vertex(G);
        const vertex_desc v_target = boost::add_vertex(G);
        long max = 0;
        for(int i = 0; i < z; i++){
            int cost;
            cin >> cost;
            adder.add_edge(v_source, i, cost);
        }

        for(int i = 0; i < j; i++){
            int reward;
            cin >> reward;
            max += reward;
            adder.add_edge(i + z, v_target, reward);
        }

        for(int i = 0; i < j; i++){
            int zone; cin >> zone;
            for(int k = 0; k < zone; k++){
                int num; 
                cin >> num;
                adder.add_edge(num, i + z, INT_MAX);
            }
        }

        long flow = boost::push_relabel_max_flow(G, v_source, v_target);
        //cout << max << " " << flow << "\n";
        cout << max - flow << "\n";
    }
    return 0;
}