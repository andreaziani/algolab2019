#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <tuple>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
using namespace std;

class ContourLine{
    public: long x, y, r;

    ContourLine(long x, long y, long r){
        this->x = x;
        this->y = y;
        this->r = r;
    }

    ContourLine(){}
};


class Stadium {
    public: long x, y, d, u;
    vector<ContourLine> contained;
    Stadium(long x, long y, long d, long u){
        this->x = x;
        this->y = y;
        this->d = d;
        this->u = u;
    }

    Stadium(){}

    void addContourLine(ContourLine c){
        contained.push_back(c);
    }
};

class Warehouse {
    public: 
    long x, y, s, a;
    vector<int> revenues;
    vector<ContourLine> contained;
    Warehouse(long x, long y, long s, long a){
        this->x = x;
        this->y = y;
        this->a = a; //% alcol
        this->s = s; //supply
    }

    Warehouse(){}

    void addReveneu(int r){
        this->revenues.push_back(r);
    }

    void addContourLine(ContourLine c){
        contained.push_back(c);
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;

    while(t--){
        int n, m, c;
        cin >> n >> m >> c;
        cout << fixed << setprecision(0);
        vector<Warehouse> warehouses;
        vector<Stadium> stadiums;
        vector<ContourLine> lines;
        for(int i = 0; i < n; i++){
            long x, y, s, a;
            cin >> x >> y >> s >> a;
            warehouses.push_back(Warehouse(x, y, s, a));
        }

        for(int i = 0; i < m; i++){
            long x, y, d, u;
            cin >> x >> y >> d >> u;
            stadiums.push_back(Stadium(x, y, d, u));
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                int r; cin >> r;
                warehouses[i].addReveneu(r);
            }
        }

        for(int i = 0 ; i < c; i++){
            long x, y, r;
            cin >> x >> y >> r;
            lines.push_back(ContourLine(x, y, r));
        }

        bool found = false;
        set<tuple<double,double,double>> used_c; 
        for(int i = 0; i < n && !found; i++){
            K::Point_2 w = K::Point_2(warehouses[i].x, warehouses[i].y);
            for(int j = 0; j < c; j++){
                if(CGAL::squared_distance(w, K::Point_2(lines[j].x, lines[j].y)) < (double) lines[j].r * lines[j].r){
                    used_c.insert({lines[j].x, lines[j].y, lines[j].r});
                    warehouses[i].addContourLine(lines[j]);
                }
                if(used_c.size() == 101) {
                    found = true;
                    break;
                }
            }
            
        }

        for(int i = 0; i < m && !found; i++){
            K::Point_2 w = K::Point_2(stadiums[i].x, stadiums[i].y);
            for(int j = 0; j < c; j++){
                if(CGAL::squared_distance(w, K::Point_2(lines[j].x, lines[j].y)) < (double) lines[j].r * lines[j].r){
                    used_c.insert({lines[j].x, lines[j].y, lines[j].r});
                    stadiums[i].addContourLine(lines[j]);
                }
                if(used_c.size() == 101) {
                    found = true;
                    break;
                }
            }
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0); 
        
        int var_counter = 0;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                set<tuple<double,double,double>> intersections; 
                for(int c1 = 0; c1 < warehouses[i].contained.size(); c1++){
                    intersections.insert({warehouses[i].contained[c1].x, warehouses[i].contained[c1].y, warehouses[i].contained[c1].r});
                }
                
                for(int c2 = 0; c2 < stadiums[j].contained.size(); c2++){
                    if(intersections.find({stadiums[j].contained[c2].x, stadiums[j].contained[c2].y, stadiums[j].contained[c2].r}) != intersections.end()){
                        intersections.erase({stadiums[j].contained[c2].x, stadiums[j].contained[c2].y, stadiums[j].contained[c2].r});
                    } else {
                        intersections.insert({stadiums[j].contained[c2].x, stadiums[j].contained[c2].y, stadiums[j].contained[c2].r});
                    }

                }

                lp.set_c(var_counter, (double) -100.0 * warehouses[i].revenues[j] + intersections.size()); // MAX a(w,s) * r(w,s) - t(w,s)/100
                var_counter++;
            }
        }

        int row_counter = 0;
        var_counter = 0;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                lp.set_a(var_counter, row_counter, 1); // sum[a(i, s)] <= supply(i)
                var_counter++;
            }
            lp.set_b(i, warehouses[i].s);
            row_counter++;
        }

        for(int j = 0; j < m; j++){
            var_counter = j;
            for(int i = 0; i < n; i++){
                lp.set_a(var_counter, row_counter, 1); // sum[a(w, j)] <= demand(j)
                var_counter += m;
            }
            lp.set_b(row_counter, stadiums[j].d);
            row_counter++;
        }

        
        for(int j = 0; j < m; j++){
            var_counter = j;
            for(int i = 0; i < n; i++){
                lp.set_a(var_counter, row_counter, -1); // -sum[a(w, j)] >= -demand(j)
                var_counter += m;
            }
            lp.set_b(row_counter, -stadiums[j].d);
            row_counter++;
        }
        

        for(int j = 0; j < m; j++){
            var_counter = j;
            for(int i = 0; i < n; i++){
                lp.set_a(var_counter, row_counter, warehouses[i].a);
                var_counter += m;
            }
            lp.set_b(row_counter, 100.0 * stadiums[j].u);
            row_counter++;
        }

        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_linear_program(lp, ET());
        assert(s.solves_linear_program(lp));

        if (s.is_infeasible()) {
            cout << "RIOT!\n";
        } else{
            cout << floor(CGAL::to_double(-s.objective_value() / 100.0)) << "\n";
        }
        
    }
    return 0;
}