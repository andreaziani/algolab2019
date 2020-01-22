#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

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
                                boost::property<boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    edge_desc add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost

        return e;
    }
};


typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


struct booking {
    int start;
    int target;
    int t1;
    int t2;
    int price;

    booking(int s, int t, int t1, int t2, int p) : start(s), target(t), t1(t1), t2(t2), price(p) {}
};

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);

    int num_test;
    cin >> num_test;

    for (int t = 0; t < num_test; t++) {
        int num_bookings, num_stations;
        cin >> num_bookings >> num_stations;

        vector<int> available_cars(num_stations);

        for (int x = 0; x < num_stations; x++) {
            cin >> available_cars[x];
        }

        vector<booking> bookings;
        map<int, int> timings;

        for (int x = 0; x < num_bookings; x++) {
            int s, target, d, a, p;
            cin >> s >> target >> d >> a >> p;

            bookings.emplace_back(s - 1, target - 1, d, a, p);

            if (timings.find(d) == timings.end())
                timings[d] = 0;

            if (timings.find(a) == timings.end())
                timings[a] = 0;
        }

        int index = 0;
        for (auto & timing : timings) {
            timing.second = index;
            index++;
        }


        vector<vector<int>> optimals(num_stations, vector<int>(timings.size(), 0));
        for (int x = 0; x < num_bookings; x++) {
            optimals[bookings[x].start][timings[bookings[x].t1]]++;
            optimals[bookings[x].target][timings[bookings[x].t2]]++;
        }
        for (auto & optimal : optimals) {
            optimal[optimal.size() - 1]++;
        }


        graph G(timings.size() * num_stations + num_stations);
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        vector<int> station_points;
        for (int x = 0; x < num_stations; x++) {
            station_points.push_back(timings.size() * num_stations + x);
        }

        const vertex_desc v_source = boost::add_vertex(G);
        const vertex_desc v_target = boost::add_vertex(G);

        for (int x = 0; x < num_stations; x++) {
            adder.add_edge(v_source, station_points[x], available_cars[x], 100); //link source to station
            adder.add_edge(station_points[x], x * timings.size() + timings.begin()->second, INT32_MAX,
                           100); //link station to first time
            adder.add_edge(x * timings.size() + std::prev(timings.end())->second, v_target, INT32_MAX,
                           100); //link last time to dest

            for (int y = 0; y < optimals[x].size(); y++) {
                if (optimals[x][y] != 0 || y == 0) {
                    for (int z = y + 1; z < optimals[x].size(); z++) {
                        if (optimals[x][z] != 0) {
                            adder.add_edge(x * timings.size() + y, x * timings.size() + z, INT32_MAX,
                                           (z - y) * 100); //link timing to next timing
                            y = z - 1;
                            break;
                        }

                    }
                }
            }
        }

        vector<pair<edge_desc, int>> travels;
        for (int x = 0; x < num_bookings; x++) {
            int start = bookings[x].start * timings.size() + timings[bookings[x].t1];
            int finish = bookings[x].target * timings.size() + timings[bookings[x].t2];

            travels.emplace_back(adder.add_edge(start, finish, 1, -bookings[x].price + 100 * (abs(start %
                                                                                                  int(timings.size()) -
                                                                                                  finish %
                                                                                                  int(timings.size())))),
                                 bookings[x].price);
        }

        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);

        long sum = 0;
        for (auto &travel : travels) {
            if (rc_map[travel.first] == 0)
                sum += travel.second;
        }
        cout << sum << "\n";
    }
}