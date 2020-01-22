#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
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
// Custom edge adder class
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost)
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
    }
};

using namespace std;
#define MAX_PRICE 20

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int n;
        cin >> n;
        int num_stud = 0;
        graph G(n);
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        //auto r_map = boost::get(boost::edge_reverse, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        vertex_desc source = boost::add_vertex(G);
        vertex_desc dest = boost::add_vertex(G);

        vector<pair<int, int>> amount_cost(n);
        for (int i = 0; i < n; i++)
        {
            int amount, cost;
            cin >> amount >> cost;
            amount_cost[i] = {amount, cost};
        }

        vector<pair<int, int>> students_revenue(n);
        for (int i = 0; i < n; i++)
        {
            int students, rev;
            cin >> students >> rev;
            students_revenue[i] = {students, rev};
            num_stud += students;
        }

        vector<pair<int, int>> freez_cost(n - 1);
        for (int i = 0; i < n - 1; i++)
        {
            int f, c;
            cin >> f >> c;
            freez_cost[i] = {f, c};
        }

        for (int i = 0; i < n; i++)
        {
            adder.add_edge(source, i, amount_cost[i].first,  amount_cost[i].second);
            adder.add_edge(i, dest, students_revenue[i].first, MAX_PRICE - students_revenue[i].second);
        }

        for(int i = 0; i < n - 1; i++){
            adder.add_edge(i, i + 1, freez_cost[i].first, freez_cost[i].second); // freezed
        }

        boost::successive_shortest_path_nonnegative_weights(G, source, dest);
        int flow = 0;
        int cost = boost::find_flow_cost(G);
        out_edge_it e, eend;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e) {
            flow += c_map[*e] - rc_map[*e];
        }

        if(flow < num_stud)
            cout << "impossible ";
        else 
            cout << "possible ";
        
        cout << flow << " " << -(cost - MAX_PRICE * flow) << "\n"; // (cost - max gain possible) = -(actual gain)
    }
    return 0;
}