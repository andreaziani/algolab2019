#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;


size_t T, n, m, p;

double round_down(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void compute_emst(std::vector<K::Point_2> vert, long p) {
    Index n = vert.size();
    typedef std::pair<K::Point_2,Index> IPoint;
    std::vector<IPoint> points;
    points.reserve(n);
    for(Index i = 0; i < n; ++i) {
        points.emplace_back(vert[i], i);
    }
    Delaunay T;
    T.insert(points.begin(), points.end());

    EdgeV edges;
    edges.reserve(3*n);
    for(auto e = T.finite_edges_begin(); e != T.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        if(i1 > i2) std::swap(i1, i2);
            edges.emplace_back(i1, i2, T.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
	    return std::get<2>(e1) < std::get<2>(e2);
    });

    boost::disjoint_sets_with_storage<> uf(n), ufa(n), ufb(n);
    Index n_components = n, n_compa = n, n_compb;
    EdgeV::const_iterator e;
    EdgeV::const_iterator ea = edges.begin(), eb = edges.begin();
    long a = 0, b = 0;

    for(e = edges.begin(); e != edges.end(); ++e) {
        Index c1, c2;
        K::FT w;
        std::tie(c1, c2, w) = *e;
        if(w > p) break;
        c1 = uf.find_set(c1);
        c2 = uf.find_set(c2);
        // b = -round_down(-w);
        if(c1 != c2) {
            uf.link(c1, c2);
            if(--n_components == 1) {
                break;
            }
        }
    }

    std::vector<std::pair<long, long>> check;
    for(int i = 0; i < m; ++i) {
        long xs, xt, ys, yt;
        std::cin >> xs >> ys >> xt >> yt;
        K::Point_2 s(xs, ys), t(xt, yt);
        auto ss = T.nearest_vertex(s);
        auto tt = T.nearest_vertex(t);
        check.push_back({ss->info(), tt->info()});
        K::FT omega = 4*std::max(CGAL::squared_distance(s, ss->point()), CGAL::squared_distance(t, tt->point()));
        if(a < omega) a = -round_down(-omega);
        if(omega <= p && uf.find_set(ss->info()) == uf.find_set(tt->info())) {
            if(b < omega) b = -round_down(-omega);
            for(; eb != edges.end() && ufb.find_set(ss->info()) != ufb.find_set(tt->info()); ++eb) {
                Index c1, c2;
                K::FT w;
                std::tie(c1, c2, w) = *eb;
                if(b < w) b = -round_down(-w);
                c1 = ufb.find_set(c1);
                c2 = ufb.find_set(c2);
                if(c1 != c2) {
                    ufb.link(c1, c2);
                    if(--n_compb == 1) {
                        break;
                    }
                }
            }
            std::cout << 'y';
        } else {
            std::cout << 'n';
        }
        for(; ea != edges.end() && ufa.find_set(ss->info()) != ufa.find_set(tt->info()); ++ea) {
            Index c1, c2;
            K::FT w;
            std::tie(c1, c2, w) = *ea;
            if(a < w) a = -round_down(-w);
            c1 = ufa.find_set(c1);
            c2 = ufa.find_set(c2);
            if(c1 != c2) {
                ufa.link(c1, c2);
                if(--n_compa == 1) {
                    break;
                }
            }
        }
    }

    std::cout << '\n' << a << '\n' << b << '\n';
}

using namespace std;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> T;
    while(T--) {
        cin >> n >> m >> p;
        vector<K::Point_2> jam(n);
        for(int i = 0; i < n; ++i) {
            long x, y;
            cin >> x >> y;
            jam[i] = K::Point_2(x, y);
        }
        compute_emst(jam, p);
    }
    return 0;
}