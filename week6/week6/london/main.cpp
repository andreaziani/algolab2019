#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <algorithm>
using namespace std;

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
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int lines, letters_per_line;
        string note;
        cin >> lines >> letters_per_line >> note;
        //read input and set the pair correctly
        vector<pair<char, char>> paper(lines * letters_per_line);
        // front page
        for (int i = 0; i < lines * letters_per_line; i++)
        {
            char val;
            cin >> val;
            paper[i].first = val;
        }
        // back page
        for (int i = 0; i < lines; i++)
        {
            for (int j = letters_per_line - 1; j >= 0; j--)
            {
                char val;
                cin >> val;
                paper[i * letters_per_line + j].second = val;
            }
        }

        if (letters_per_line * lines < note.length())
        {
            cout << "No\n";
        }
        else
        {
            map<pair<char, char>, int> possible_couples; // [char couple, occurrency]
            for (int i = 0; i < lines * letters_per_line; i++)
            {
                if (possible_couples.count({paper[i].second, paper[i].first}) != 0)
                    possible_couples[{paper[i].second, paper[i].first}]++;
                else
                    possible_couples[paper[i]]++;
            }

            graph G(26);
            edge_adder adder(G);
            const vertex_desc v_source = boost::add_vertex(G);
            const vertex_desc v_target = boost::add_vertex(G);

            vector<int> alph(26, 0);
            //node id, # occurrency in string note
            for (int i = 0; i < note.length(); i++) // O(length(note))
                alph[note[i] - 'A']++;

            for (int i = 0; i < 26; i++)
                adder.add_edge(i, v_target, alph[i]);

            for (auto it = possible_couples.begin(); it != possible_couples.end(); it++)
            {
                if(alph[it->first.second - 'A'] != 0 || alph[it->first.first - 'A'] != 0){
                    vertex_desc new_node = boost::add_vertex(G);
                    adder.add_edge(v_source, new_node, it->second);
                    if(alph[it->first.first - 'A'] != 0)
                        adder.add_edge(new_node, it->first.first - 'A', it->second);
                    if(alph[it->first.second - 'A'] != 0)
                        adder.add_edge(new_node, it->first.second - 'A', it->second);
                } 
            }

            int flow = boost::push_relabel_max_flow(G, v_source, v_target); // O(V^2 * E)
            if (flow != note.length())
                cout << "No\n";
            else
                cout << "Yes\n";
        }
    }
    return 0;
}