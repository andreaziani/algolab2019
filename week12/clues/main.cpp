#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <tuple>


// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::pair<int, int>                                    info_t;
typedef CGAL::Triangulation_vertex_base_with_info_2<info_t,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle vertex_handle;
typedef Delaunay::Vertex_circulator vcirculator_t;

using namespace std;

bool try_coloring(int range, Delaunay &t){
    Delaunay b, w;

    for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++){
        v->info() = {0, false}; // component, color
    }
    
    int comp = 0;
    for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++){
        if(v->info().first == 0){
            v->info().second = ++comp;
            stack<vertex_handle> st;
            st.push(v);
            while(!st.empty()){
                auto curr_v = st.top(); st.pop();
                vcirculator_t c = t.incident_vertices(curr_v);
                do{
                    if(!t.is_infinite(c)){
                        auto d = CGAL::squared_distance(c->point(), curr_v->point());
                        if(d <= range * range){
                            if(c->info() == curr_v->info()) // coloring not possible
                                return false;
                            if(c->info().first == 0){
                                c->info() = {comp, !curr_v->info().second};
                                st.push(c);
                            }
                        }
                    }
                }while(++c != t.incident_vertices(curr_v));
            }
        }
        if(v->info().second) w.insert(v->point());
        else b.insert(v->point());
    }
    
    for(auto e = b.finite_edges_begin(); e != b.finite_edges_end(); e++)
        if(b.segment(e).squared_length() <= range * range)
            return false;
    
    for(auto e = w.finite_edges_begin(); e != w.finite_edges_end(); e++)
        if(w.segment(e).squared_length() <= range * range)
            return false;

    return true;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n_stations, n_clues, range;
        cin >> n_stations >> n_clues >> range;

        vector<K::Point_2> stations;
        for(int i = 0; i < n_stations; i++){
            double x, y;
            cin >> x >> y;
            stations.push_back(K::Point_2(x, y));
        }

        vector<pair<K::Point_2,K::Point_2>> clues;
        for(int i = 0; i < n_clues; i++){
            int x, y, x1, y1;
            cin >> x >> y >> x1 >> y1;
            clues.push_back({K::Point_2(x, y), K::Point_2(x1, y1)});
        }
        
        Delaunay t;
        t.insert(stations.begin(), stations.end());
        bool colorable = try_coloring(range, t);

        for(int i = 0; i < n_clues; i++){
            if(!colorable){
                cout << "n";
                continue;
            }
            auto v = t.nearest_vertex(clues[i].first);
            auto v2 = t.nearest_vertex(clues[i].second);


            if (CGAL::squared_distance(clues[i].first, clues[i].second) <= range * range){
                cout << "y";
            } if (v->info().first == v2->info().first &&
                CGAL::squared_distance(clues[i].first, v->point()) <= range *range &&
                CGAL::squared_distance(clues[i].second, v2->point()) <= range*range){
                cout << "y";
            } else {
                cout << "n";
            }
        }

        cout << "\n";
    }

    return 0;
}