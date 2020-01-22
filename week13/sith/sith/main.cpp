#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		>					graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges


using namespace std;

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, r;
        cin >> n >> r;
        vector<pair<K::Point_2,int>> planets(n);
        
        for(int i = 0; i < n; i++){
            double x, y;
            cin >> x >> y;
            planets[i].first = K::Point_2(x, y);
            planets[i].second = i;
        }

        int start = 2, end = (n + 1) / 2;
        
        while(start <= end){
            int turn_of_end = (start + end) / 2;
            
            Triangulation t;
            t.insert(planets.begin() + turn_of_end, planets.end());

            graph G;
            for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
                int i = e->second;
                Triangulation::Face& f = *e->first; //face handle
                Vertex u = f.vertex(f.cw(i)); // u
                Vertex v = f.vertex(f.ccw(i)); // v

                if(CGAL::squared_distance(u->point(), v->point()) <= (long) r * (long) r){
                    boost::add_edge(u->info(), v->info(), G);
                }
            }

            std::vector<int> component_map(G.vertex_set().size());	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	        int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

            vector<int> diff_components(ncc);
            int max_planets_in_component = 0;
            for(int i = 0; i < G.vertex_set().size(); i++){
                diff_components[component_map[i]]++;
                if(diff_components[component_map[i]] > max_planets_in_component)
                    max_planets_in_component = diff_components[component_map[i]];
            }

            if(max_planets_in_component >= turn_of_end){
                start = turn_of_end + 1;
            } else{
                end = turn_of_end - 1;
            }
        }

        cout << start - 1 << "\n";

    }
    return 0;
}