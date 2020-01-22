#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
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
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

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
		w_map[e] = cost;	  // new assign cost
		w_map[rev_e] = -cost; // new negative cost
	}
};

int X[5001];
int Y[5001];
int D[5001];
int E[5001];
int c, g, b, k, a;

long find_flow(long cap)
{
	graph G(c + 1);
	edge_adder adder(G);
	auto c_map = boost::get(boost::edge_capacity, G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);
	for (int i = 0; i < g; i++)
	{
		adder.add_edge(X[i], Y[i], E[i], D[i]);
	}
	int v_source = c;
	adder.add_edge(v_source, k, cap, 0);

	boost::successive_shortest_path_nonnegative_weights(G, v_source, a);
	long cost1 = boost::find_flow_cost(G);
	if (cost1 <= b)
	{
		int s_flow = 0;
		out_edge_it e, eend;
		for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source, G), G); e != eend; ++e)
		{
			s_flow += c_map[*e] - rc_map[*e];
		}
		return s_flow;
	}
	else
	{
		return -1;
	}
}

long binary(long l, long u)
{
	if (l == u)
	{
		return find_flow(l);
	}
	else if (u - l == 1) 
	{
		long upp = find_flow(u); // try right
		if (upp == -1)
		{
			return find_flow(l); // try left
		}
		else
		{
			return upp;
		}
	}
	long mid = (l + u) / 2;
	long flow = find_flow(mid);
	if (flow == -1) // cost > budget
	{
		return binary(l, mid - 1);
	}
	else
	{
		return binary(mid, u);
	}
}

void solve()
{
	cin >> c >> g >> b >> k >> a;
	for (int i = 0; i < g; i++)
	{
		long x, y, d, e;
		cin >> x >> y >> d >> e;
		X[i] = x;
		Y[i] = y;
		D[i] = d;
		E[i] = e;
	}

	graph G1(c + 1);
	edge_adder adder(G1);

	for (int i = 0; i < g; i++)
	{
		adder.add_edge(X[i], Y[i], E[i], D[i]);
	}

	long flow1 = boost::push_relabel_max_flow(G1, k, a);
	cout << binary(0, flow1) << "\n";
}

int main()
{
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t--)
	{
		solve();
	}
}