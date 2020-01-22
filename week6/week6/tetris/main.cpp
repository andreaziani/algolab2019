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

int main()
{
	std::ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t-- > 0){
		int width, n_bricks;
		cin >> width >> n_bricks; 
		graph G(width + 1);
		edge_adder adder(G);
		map<int, vector<int>> edges;

        for (int x = 0; x < n_bricks; x++) {
            int a, b;
            cin >> a >> b;
            if (a == 0 || b == 0) {
                if (a < b)
                    adder.add_edge(a, b, 1);
                else if (b < a)
                    adder.add_edge(b, a, 1);
            } else {
                if (a < b)
                    edges[a].push_back(b);
                else if (b < a)
                    edges[b].push_back(a);
            }
        }

        for (auto it = edges.begin(); it != edges.end(); it++) {
            vertex_desc node_copy = boost::add_vertex(G);
            adder.add_edge(it->first, node_copy, 1);
            for (int x = 0; x < it->second.size(); x++) {
                adder.add_edge(node_copy, it->second[x], 1);
            }
        }

        cout << boost::push_relabel_max_flow(G, 0, width) << "\n";

	}
	return 0;
}