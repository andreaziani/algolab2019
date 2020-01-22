#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef std::pair<int,bool> info_t;
typedef CGAL::Triangulation_vertex_base_with_info_2<info_t, K> vertex_t;
typedef CGAL::Triangulation_face_base_2<K> face_t;
typedef CGAL::Triangulation_data_structure_2<vertex_t,face_t> triangulation_t;
typedef CGAL::Delaunay_triangulation_2<K,triangulation_t> Delaunay;
typedef Delaunay::Vertex_handle vertex_handle;
typedef Delaunay::Vertex_circulator vcirculator_t;

bool try_coloring(Delaunay &t, long r){
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v)
        v->info() = { 0, false };

    int components = 0;
    Delaunay whites, blacks;
    for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
        if (v->info().first == 0) {
            v->info().first = ++components;
            stack<vertex_handle> stack;
            stack.push(v);
            do {
                auto h = stack.top();
                stack.pop();
                vcirculator_t c = t.incident_vertices(h);
                do {
                    if (!t.is_infinite(c) &&
                        CGAL::squared_distance(h->point(), c->point()) <= r*r) {
                        
                        if (c->info() == h->info()) 
                            return false;
                        if (c->info().first == 0) {
                            stack.push(c);
                            c->info() = { components, !h->info().second };
                        }
                    }
                }while (++c != t.incident_vertices(h));
            } while (!stack.empty());
        }
        
        if (v->info().second) whites.insert(v->point());
        else blacks.insert(v->point());
    }
    
    //VALIDATION STEP, NEEDED BECAUSE THE GRAPH CAN BE NOT COLORABLE WHILE THE SPANNING FOREST IS COLORABLE
    //= IF ALL THE EDGES BETWEEN WHITES ARE LARGER THAN R*R
    for (auto e = whites.finite_edges_begin(); e != whites.finite_edges_end(); ++e)
        if (whites.segment(*e).squared_length() <= r*r)
            return false;

    //= IF ALL THE EDGES BETWEEN BLACKS ARE LARGER THAN R*R
    for (auto e = blacks.finite_edges_begin(); e != blacks.finite_edges_end(); ++e)
        if (blacks.segment(*e).squared_length() <= r*r)
            return false;
        
    return true;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, m;
        double r; 
        bool colorable = true;
        cin >> n >> m >> r;

        vector<K::Point_2> stations;
        for(int i = 0; i < n; i++){
            long x, y;
            cin >> x >> y;
            stations.emplace_back(K::Point_2(x,y));
        }

        vector<pair<K::Point_2, K::Point_2>> clues(m);
        for(int i = 0; i < m; i++){
            long x, y, x1, y1;
            cin >> x >> y >> x1 >> y1;
            clues[i] = {K::Point_2(x, y), K::Point_2(x1, y1)};
        }

        Delaunay t;
        t.insert(stations.begin(), stations.end());

        colorable = try_coloring(t, r);
        for(int i = 0; i < m; i++){
            if(!colorable){
                cout << "n";
                continue;
            }

            if (CGAL::squared_distance(clues[i].first, clues[i].second) <= r*r){
                cout << "y";
                continue;
            }

            auto station_holmes = t.nearest_vertex(clues[i].first);
            auto station_watson = t.nearest_vertex(clues[i].second);
            if (station_holmes->info().first == station_watson->info().first &&
                CGAL::squared_distance(clues[i].first, station_holmes->point()) <= r*r &&
                CGAL::squared_distance(clues[i].second, station_watson->point()) <= r*r){
                cout << "y";
            } else {
                cout << "n";
            }
            
        }
        
        cout << "\n";
    }
    return 0;
}