#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> 
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K; 
typedef CGAL::Delaunay_triangulation_2<K> Triangulation; 
typedef Triangulation::Finite_faces_iterator Face_iterator;

using namespace std;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    while(true){
        int n; cin >> n;
        if(n == 0) break;
        vector<K::Point_2> coords(n);
        for(int i = 0; i < n; i++){
            int x, y;
            cin >> x >> y;
            coords[i] = K::Point_2(x, y);
        }

        Triangulation t;
        t.insert(coords.begin(), coords.end());
        K::FT bestTime = INT_MAX;
        for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++){
            K::FT time = t.segment(e).squared_length(); // length^2 between two points (edge source and dest)
            if(time < bestTime)
                bestTime = time;
        }

        cout << ceil(sqrt(ceil_to_double(bestTime)) / 2.0 * 100.0)  << "\n";
    }
    return 0;
}