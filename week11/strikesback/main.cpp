#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;

typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

using namespace std;
struct asteroid{
    K::Point_2 p;
    double d;
    asteroid(double x, double y, double d){
        p = K::Point_2(x,y);
        this->d = d;
    }  
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int a, s, b;
        int e;
        cin >> a >> s >> b;
        cin >> e;

        vector<asteroid> as;
        for(int i = 0; i < a; i++){
            double x, y, d;
            cin >> x >> y >> d;
            as.emplace_back(x,y,d);
        }

        vector<K::Point_2> sp;
        vector<K::Point_2> bh;

        for(int i = 0; i < s; i++){
            double x, y;
            cin >> x >> y;
            sp.emplace_back(x,y);
        }

        for(int i = 0; i < b; i++){
            double x, y;
            cin >> x >> y;
            bh.emplace_back(x,y);
        }

        vector<double> radius(s, 0.0);
        if(b > 0){
            Triangulation t;
            t.insert(bh.begin(), bh.end());
            for(int i = 0; i < s; i++){
                auto p = t.nearest_vertex(sp[i])->point();
                radius[i] = CGAL::to_double(CGAL::squared_distance(p, sp[i]));
            }
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0);

        for(int i = 0; i < a; i++){
            for(int j = 0; j < s; j++){
                auto d = CGAL::to_double(CGAL::squared_distance(sp[j], as[i].p));
                if(radius[j] != 0){
                    if(radius[j] > d){
                        d = min(d, radius[j]);
                        lp.set_a(j, i, -1 / max(1.0, d));
                    }
                } else {
                    lp.set_a(j, i, -1 / max(1.0, d));
                }
                
            }

            lp.set_b(i, -as[i].d);
        }

        for(int j = 0; j < s; j++)
            lp.set_c(j, 1);
        
        Solution sol = CGAL::solve_linear_program(lp, ET());
        assert(sol.solves_linear_program(lp));
        
        if (!sol.is_infeasible() && CGAL::to_double(sol.objective_value()) <= e) {
		    cout << "y\n";
	    } else {
		    cout << "n\n";
	    }


    }
    return 0;
}