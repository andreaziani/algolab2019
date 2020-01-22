#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> 
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Segment_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K; 
typedef CGAL::Delaunay_triangulation_2<K> Triangulation; 
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_handle         Vertex_handle;
using namespace std;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    while(true){
        int n; cin >> n;
        if(n == 0) break;
        vector<K::Point_2> rest(n);
        
        for(int i = 0; i < n; i++){
            int x, y;
            cin >> x >> y;
            rest[i] = K::Point_2(x, y);
        }

        Triangulation t;
        t.insert(rest.begin(), rest.end());

        int m; cin >> m;
        for(int i = 0; i < m; i++){
            int x, y;
            cin >> x >> y;
            auto point = K::Point_2(x, y);
            Vertex_handle vertex = t.nearest_vertex(point);
            cout << CGAL::to_double(CGAL::squared_distance(vertex->point(), point)) << "\n";
        }

    }
    return 0;
}