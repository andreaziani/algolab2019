#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    graph;
// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
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
int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int g, s, l;
        cin >> g >> s >> l;
        graph G(g + s);
        edge_adder adder(G);
        vertex_desc source = boost::add_vertex(G);
        vertex_desc dest = boost::add_vertex(G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        for (int i = 0; i < g; i++)
        {
            adder.add_edge(source, i, 1);
        }

        for (int i = g; i < g + s; i++)
        {
            adder.add_edge(i, dest, 1);
        }
        for (int i = 0; i < l; i++)
        {
            int u, v;
            cin >> u >> v;
            adder.add_edge(u, v + g, 1);
        }

        // Find a min cut via maxflow
        int flow = boost::push_relabel_max_flow(G, source, dest);

        std::vector<int> vis(g + s + 2, false); // visited flags
        std::queue<int> Q;                      // BFS queue (from std:: not boost::)
        vis[source] = true;                     // Mark the source as visited
        Q.push(source);
        while (!Q.empty())
        {
            const int u = Q.front();
            Q.pop();
            out_edge_it ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg)
            {
                const int v = boost::target(*ebeg, G);
                // Only follow edges with spare capacity
                if (rc_map[*ebeg] == 0 || vis[v])
                    continue;
                vis[v] = true;
                Q.push(v);
            }
        }

        vector<int> res1, res2;
        for (int i = 0; i < g; i++)
        {
            if (!vis[i])
                res1.push_back(i);
        }

        for (int i = g; i < g + s; i++)
        {
            if (vis[i])
                res2.push_back(i - g);
        }

        cout << res1.size() << " " << res2.size() << "\n";
        for (auto e : res1)
        {
            cout << e << " ";
        }

        for (auto e : res2)
        {
            cout << e << " ";
        }

        cout << "\n";
    }
    return 0;
}