#include <stdlib.h>
#include <iostream>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int t;cin >> t;
    while(t--){
        int n, m;
        double p;
        cin >> n >> m >> p;

        vector<pair<K::Point_2, K::Point_2>> missions;
        typedef std::pair<K::Point_2,Index> IPoint;
        std::vector<IPoint> jammers;
        jammers.reserve(n);
        for (Index i = 0; i < n; ++i) {
            double x, y;
            std::cin >> x >> y;
            jammers.emplace_back(K::Point_2(x, y), i);
        }

        for(int i = 0; i < m; i++){
            double x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            missions.push_back({K::Point_2(x1, y1), K::Point_2(x2, y2)});
        }

        Delaunay t;
        t.insert(jammers.begin(), jammers.end());

        EdgeV edges;
        edges.reserve(3*n);
        for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            Index i1 = e->first->vertex((e->second+1)%3)->info();
            Index i2 = e->first->vertex((e->second+2)%3)->info();
            // ensure smaller index comes first
            if (i1 > i2) std::swap(i1, i2);
            edges.emplace_back(i1, i2, t.segment(e).squared_length());
        }
        std::sort(edges.begin(), edges.end(),
            [](const Edge& e1, const Edge& e2) -> bool {
                return std::get<2>(e1) < std::get<2>(e2);
                });

        boost::disjoint_sets_with_storage<> uf_safe(n), uf_all(n), uf_same(n);
        
        for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && get<2>(*e) <= p; ++e) {
            Index c1 = uf_safe.find_set(std::get<0>(*e));
            Index c2 = uf_safe.find_set(std::get<1>(*e));
            if (c1 != c2) {
                uf_safe.link(c1, c2);
            }
        }
        
        K::FT b = 0;
        K::FT c = 0;
        EdgeV::const_iterator e = edges.begin(), e1 = edges.begin();
        for(int i = 0; i < m; i++){
            auto v1 = t.nearest_vertex(missions[i].first);
            auto v2 = t.nearest_vertex(missions[i].second);
            K::FT d1 = CGAL::squared_distance(missions[i].first, v1->point());
            K::FT d2 = CGAL::squared_distance(missions[i].second, v2->point());
            K::FT d = 4.0 * max(d1, d2);
            Index c1 = uf_safe.find_set(v1->info());
            Index c2 = uf_safe.find_set(v2->info());

            if(d <= p && c1 == c2){
                c = max(c, d);
                for (; e1 != edges.end() && uf_same.find_set(v1->info()) != uf_same.find_set(v2->info()); ++e1) {
                    Index k1 = uf_same.find_set(std::get<0>(*e1));
                    Index k2 = uf_same.find_set(std::get<1>(*e1));
                    if (k1 != k2) {
                        uf_same.link(k1, k2);
                    }
                }
                cout << "y";
            } else {
                b = max(b, d);
                for (; e != edges.end() && uf_all.find_set(v1->info()) != uf_all.find_set(v2->info()); ++e) {
                    Index k1 = uf_all.find_set(std::get<0>(*e));
                    Index k2 = uf_all.find_set(std::get<1>(*e));
                    if (k1 != k2) {
                        uf_all.link(k1, k2);
                    }
                }
                cout << "n";
            }
        }

        if(e != edges.begin()) b = max(b, get<2>(*(e - 1))); // -1 because is already the next edge 
        if(e1 != edges.begin()) c = max(c, get<2>(*(e1 - 1)));

        cout << "\n";
        cout << b << "\n";
        cout << c << "\n";


    }
    return 0;
}