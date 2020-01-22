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
        int lines, letters_per_line;
        string note;
        bool possible = true;
        cin >> lines >> letters_per_line;
        cin >> note;

        vector<pair<char,char>> combined(lines * letters_per_line);
        for(int i = 0; i < lines * letters_per_line; i++){
            char val;
            cin >> val;
            combined[i].first = val;
        }
        for(int i = 0; i < lines; i++){
            for(int j = letters_per_line - 1; j >= 0; j--){
                char val;
                cin >> val;
                combined[i * letters_per_line + j].second = val;
            }
        }

        if(lines*letters_per_line < note.length()){
            cout << "No\n";
        } else {
            map<pair<char,char>, int> unique_pairs; // [char couple, times]
            for(int i = 0; i < lines * letters_per_line; i++){
                if(unique_pairs.count({combined[i].second, combined[i].first}) != 0){
                    unique_pairs[{combined[i].second, combined[i].first}]++;
                } else{
                    unique_pairs[combined[i]]++;
                }
            }

            graph G(26);
            edge_adder adder(G);
            const vertex_desc v_source = boost::add_vertex(G);
            const vertex_desc v_target = boost::add_vertex(G);

            unordered_map<char, int> note_nodes; //weights of note's edges
            for(int i = 0; i < note.length(); i++){
                note_nodes[note[i]]++;
            }

            for(auto it = note_nodes.begin(); it != note_nodes.end(); it++){
                adder.add_edge(it->first - 65, v_target, it->second);
            }

            for(auto it = unique_pairs.begin(); it != unique_pairs.end(); it++){
                if(note_nodes[it->first.second] != 0 || note_nodes[it->first.first] != 0){
                    vertex_desc common = boost::add_vertex(G);
                    adder.add_edge(v_source, common, it->second);
                    if(note_nodes[it->first.first] != 0)
                        adder.add_edge(common, it->first.first - 65, it->second);
                    if(note_nodes[it->first.second] != 0)
                        adder.add_edge(common, it->first.second - 65, it->second);
                }
                
            }
            int flow = boost::push_relabel_max_flow(G, v_source, v_target);
            if(flow != note.length()) cout << "No\n";
            else cout << "Yes\n";
        }
    }
    return 0;
}