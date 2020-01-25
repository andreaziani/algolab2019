#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <tuple>
#include <algorithm>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		>					graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges

int main(){
    int t;
    cin >> t;
    while(t--){
        long n, r;
        cin >> n >> r;

        vector<IPoint> planets;
        for(size_t i = 0; i < n; i++){
            double x, y;
            cin >> x >> y;
            planets.emplace_back(K::Point_2(x, y), i);
        }

        int start = 2, end = (n + 1) / 2;
        while(start <= end){
            int m = (start + end)/ 2;
            Delaunay t;
            t.insert(planets.begin() + m, planets.end());     
            
            graph G;
            for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
                Index i1 = e->first->vertex((e->second+1)%3)->info();
                Index i2 = e->first->vertex((e->second+2)%3)->info();

                if(t.segment(e).squared_length() <= r * r)
                    boost::add_edge(i1, i2, G);
            }

            std::vector<int> component_map(boost::num_vertices(G));
	        int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G)));

            std::vector<int> component_vertices(ncc, 0);
            int curr_max = 0;
            // Iterate over all vertices
            for (int i = 0; i < boost::num_vertices(G); ++i){
                component_vertices[component_map[i]]++;
                if(component_vertices[component_map[i]] > curr_max)
                    curr_max = component_vertices[component_map[i]];
            }
            
            if(m <= curr_max){
                start = m + 1;
            } else {
                end = m - 1;
            }
        }

        cout << start - 1 << "\n";
    }
    return 0;
}