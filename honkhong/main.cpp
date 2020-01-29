#include <stdlib.h>
#include <iostream>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT                                      Info;
typedef CGAL::Triangulation_vertex_base_2<K>                    Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info, K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        long long n_trees, n_balloons, tree_radius;
        cin >> n_trees >> n_balloons >> tree_radius;

        vector<K::Point_2> trees(n_trees);
        for(int i = 0; i < n_trees; i++){
            cin >> trees[i];
        }

        vector<pair<K::Point_2, K::FT>> balloons(n_balloons);
        for(int i = 0; i < n_balloons; i++){
            double x, y, r;
            cin >> x >> y >> r;
            balloons[i] = {K::Point_2(x, y), r};
        }
        
        Delaunay t;
        t.insert(trees.begin(), trees.end());

        priority_queue<pair<K::FT, Delaunay::Face_handle>> max_heap; 


        for(auto it = t.all_faces_begin(); it != t.all_faces_end(); it++){
            if(t.is_infinite(it)){
                for(int i = 0; i < 3; i++){
                    auto neig_f = it->neighbor(i);
                    if(!t.is_infinite(neig_f)){
                        auto dist = t.segment(it, i).squared_length();
                        max_heap.push({dist, neig_f});
                    }
                }
                it->info() = -1;
            } else{
                it->info() = 0;
                auto d = CGAL::squared_radius(it->vertex(0)->point(), it->vertex(1)->point(), it->vertex(2)->point());
                max_heap.push({d, it});
            }  
        }

        while(!max_heap.empty()){
            auto curr_fh = max_heap.top(); max_heap.pop();
            auto curr_face = curr_fh.second;
            if(curr_face->info() != 0) continue;
            
            curr_face->info() = curr_fh.first;
            for(int i = 0; i < 3; i++){
                auto neig_f = curr_face->neighbor(i);
                if(!t.is_infinite(neig_f)){
                    auto dist = t.segment(curr_face, i).squared_length();
                    dist = min(dist, curr_face->info());
                    max_heap.push({dist, neig_f});
                }
            }
        }

        for(int i = 0; i < n_balloons; i++){
            auto p = balloons[i].first;
            auto r = balloons[i].second;
            auto f = t.locate(p);

            auto face = t.locate(p);
            auto nearest = t.nearest_vertex(p,face)->point();
            // if cannot be inflated
            if(CGAL::square((tree_radius+r)) > CGAL::squared_distance(p,nearest)){
                std::cout << 'n';
            } else {
                if(t.is_infinite(face) || CGAL::square(2*r + 2*tree_radius) <= face->info()){
                    std::cout << 'y';  
                } else {
                    std::cout << 'n';  
                }
            }
        }

        cout << "\n";
    }
    return 0;
}