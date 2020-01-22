// ATTENTION! LP programs have col, row when declare
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

using namespace std;

typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution <ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

using namespace std;
class Asteroid{
    public: 
        K::Point_2 coord;
        double d;
    Asteroid(double x, double y, int d){
        coord = K::Point_2(x, y);
        this->d = d;
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int a, s, b, e;
        cin >> a >> s >> b >> e;

        vector<Asteroid> asteroids;
        for(int i = 0; i < a; i++){
            double x, y;
            double d;
            cin >> x >> y >> d;
            asteroids.push_back(Asteroid(x,y,d));
        }

        vector<K::Point_2> shots;
        vector<double> radius(s, 0.0);
        vector<K::Point_2> hunters;

        for(int i = 0; i < s; i++){
            double x, y;
            cin >> x >> y;
            shots.push_back(K::Point_2(x,y));
        }

        for(int i = 0; i < b; i++){
            double x, y;
            cin >> x >> y;
            hunters.push_back(K::Point_2(x,y));
        }

        if(b > 0){
            Triangulation t;
            t.insert(hunters.begin(), hunters.end());
        
            for(int i = 0; i < s; i++){
                auto p = t.nearest_vertex(shots[i])->point();
                radius[i] = CGAL::to_double(CGAL::squared_distance(p, shots[i]));
            }
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0);

        for(int i = 0; i < a; i++){
            for(int j = 0; j < s; j++){
                double dist = CGAL::squared_distance(shots[j], asteroids[i].coord);
                if(radius[j] != 0){
                    if(radius[j] > dist){
                        if(dist <= 1.0){
                            dist = 1.0;
                        }
                        lp.set_a(j, i,  -(1 / dist));
                    }
                } else {
                    if(dist <= 1.0){
                        dist = 1.0;
                    }
                    lp.set_a(j, i,  -(1 / dist));
                }
            }
            lp.set_b(i, -asteroids[i].d);
        }


        for(int i = 0; i < s; i++)
            lp.set_c(i, 1);
        
        Solution sol = CGAL::solve_linear_program(lp, ET());
        assert(sol.solves_linear_program(lp));
        
        //cout << CGAL::to_double(sol.objective_value()) << endl;
        if (!sol.is_infeasible() && CGAL::to_double(sol.objective_value()) <= e) {
		    cout << "y" << endl;
	    } else {
		    cout << "n" << endl;
	    }

        
    }
    return 0;
}