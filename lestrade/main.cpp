#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;


// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef std::pair<K::Point_2, Index> IPoint;
using namespace std;
struct gang{
    K::FT x, y, where, who, how;
    gang(double x, double y, double where, double who, double how){
        this->x = x;
        this->y = y;
        this->where = where;
        this->who = who;
        this->how = how;
    }
};

struct agent{
    K::FT x, y, z;
    agent(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        double sherlock_fee, where, who, how;
        cin >> sherlock_fee >> where >> who >> how;

        int n_agents, n_gang;
        cin >> n_agents >> n_gang;

        vector<gang> g;
        
        vector<IPoint> points;
        for(int i = 0; i < n_gang; i++){
            double a, b, c, d, e;
            cin >> a >> b >> c >> d >> e;
            g.emplace_back(a, b, c, d, e);
            points.emplace_back(K::Point_2(a, b), i);
        }

        vector<agent> a;
        for(int i = 0; i < n_agents; i++){
            double x, y, c;
            cin >> x >> y >> c;
            a.emplace_back(x, y, c);
        }

        Delaunay t;
        t.insert(points.begin(), points.end());

        map<long, long> less_expensive;
        
        for(int i = 0; i < n_agents; i++){
            auto nearest = t.nearest_vertex(K::Point_2(a[i].x, a[i].y))->info();
            
            if(less_expensive.find(nearest) != less_expensive.end()){
                auto ag = less_expensive[nearest];
                if(ag > a[i].z)
                    less_expensive[nearest] = a[i].z;
            } else {
                less_expensive[nearest] = a[i].z;
            }
        }


        Program lp (CGAL::SMALLER, true, 0, true, 24);

        int i = 0;
        for(auto &e : less_expensive)
            lp.set_c(i++, e.second);

        i = -1;
        for(auto &e : less_expensive){
            i++;
            lp.set_a(i, 0, -g[e.first].where);
            lp.set_a(i, 1, -g[e.first].who);
            lp.set_a(i, 2, -g[e.first].how);
        }

        lp.set_b(0, -where);
        lp.set_b(1, -who);
        lp.set_b(2, -how);

        Solution s = CGAL::solve_linear_program(lp, ET());
        //cout << cost << endl;
        if(s.is_optimal() && CGAL::to_double(s.objective_value()) <= sherlock_fee){
            cout << "L\n";
        } else{
            cout << "H\n";
        }
    }
    return 0;
}