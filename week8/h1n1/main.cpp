#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef pair<K::FT, Triangulation::Face_handle> face_handle; // {dist, Face_handle}

int main(){
    std::ios_base::sync_with_stdio(false);
    while(true){
        int n;
        cin >> n;
        if(n == 0) break;
        vector<K::Point_2> p;
        for(int i = 0; i < n; i++){
            double x, y;
            cin >> x >> y;
            p.emplace_back(x, y);
        }

        Triangulation t;
        t.insert(p.begin(), p.end());
        
        priority_queue<face_handle, vector<face_handle>, less<face_handle>> min_heap;
        for(auto it = t.all_faces_begin(); it != t.all_faces_end(); it++){
            if(t.is_infinite(it)){
                it->info() = -1; // because one escapee can be already on an infinite face
            } else{
                it->info() = 0; 
                continue;
            }

            for(int i = 0; i < 3; i++){
                auto f = it->neighbor(i);
                if(!t.is_infinite(f))
                    min_heap.push({t.segment(it, i).squared_length(), f});
            }
        }

        while(!min_heap.empty()){
            auto v = min_heap.top();
            min_heap.pop();
            auto curr_f = v.second;
            auto dist = v.first;
            if(curr_f->info() != 0) // already visited
                continue;
            
            curr_f->info() = v.first; // set distance 
            for(int i = 0; i < 3; i++){
                auto neighbor_face = curr_f->neighbor(i);
                if(!t.is_infinite(neighbor_face) && neighbor_face->info() == 0){ //not infinite and not visited
                    K::FT minimum_dist = min(t.segment(curr_f, i).squared_length(), dist);
                    min_heap.push({minimum_dist, neighbor_face});
                }
            }
        }

        int m;
        cin >> m;
        for (int i = 0; i < m; ++i)
        {
            double x, y;
            long d;
            cin >> x >> y >> d;
            K::Point_2 p(x, y);
            auto v = t.nearest_vertex(p);
            if (CGAL::squared_distance(p, v->point()) < d)
            {
                cout << "n";
                continue;
            }

            auto f = t.locate(p);
            if(t.is_infinite(f)){
                cout << "y";
            } else if (f->info() < d * 4.0)
                cout << "n";
            else
                cout << "y";
        }
        cout << "\n";

    }
    return 0;
}