#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> 
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Circle_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K; 
typedef CGAL::Delaunay_triangulation_2<K> Triangulation; 
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_handle         Vertex_handle;
using namespace std;

double hours(double distance) {
    double x = sqrt(distance) - 0.5; // first sqrt to get the distance
    double result;
    if (x <= 0.0) {
        result = 0;
    } else {
        result = ceil(sqrt(x)); // second sqrt because t^2 + 0.5 = x => t = sqrt(x - 0.5);
    }
    return result;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    while(true){
        int n; cin >> n;
        if(n == 0) break;
        int l, b, r, t;
        cin >> l >> b >> r >> t;
        vector<K::Point_2> obstacles(n);
        map<K::Point_2, K::FT> time_to_die;

        for(int i = 0; i < n; i++){
            K::FT x, y;
            cin >> x >> y;
            obstacles[i] = K::Point_2(x, y);
            K::Point_2 up = K::Point_2(x, t);
            K::Point_2 down = K::Point_2(x, b);
            K::Point_2 left = K::Point_2(l, y);
            K::Point_2 right = K::Point_2(r, y);
            auto d1 = CGAL::squared_distance(obstacles[i], up);
            auto d2 = CGAL::squared_distance(obstacles[i], down);
            auto d3 = CGAL::squared_distance(obstacles[i], right);
            auto d4 = CGAL::squared_distance(obstacles[i], left);
            time_to_die[obstacles[i]] = min(d1, min(d2, min(d3, d4)));
        }
        
        Triangulation triang;
        triang.insert(obstacles.begin(), obstacles.end());
        for(auto edge_iter = triang.finite_edges_begin(); edge_iter != triang.finite_edges_end(); ++edge_iter) {
            auto vertex1 = edge_iter->first->vertex(triang.cw(edge_iter->second));
            auto vertex2 = edge_iter->first->vertex(triang.ccw(edge_iter->second));

            Triangulation::Point vertex1_point = vertex1->point();
            Triangulation::Point vertex2_point = vertex2->point();

            // calculate distance of the points of both vertex and half them (divide by 4 as distance is squared and 4 = 2^2)
            double vertex_distance = CGAL::to_double(CGAL::squared_distance(vertex1_point, vertex2_point)) / 4;

            time_to_die[vertex1_point] = min(time_to_die[vertex1_point], vertex_distance);
            time_to_die[vertex2_point] = min(time_to_die[vertex2_point], vertex_distance);
        }
        
        vector<double> times;
        for(auto it = time_to_die.begin(); it != time_to_die.end(); it++){
            if(it->second != -1)
                times.push_back(CGAL::to_double(it->second));
        }

        sort(times.begin(), times.end());

        cout << hours(times[0]) << " " 
                << hours(times[n / 2]) << " " 
                << hours(times[n - 1]) << "\n";

    }
    return 0;
}