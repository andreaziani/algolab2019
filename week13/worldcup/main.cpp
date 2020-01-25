#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
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
        int n_warehouse, n_stadium, n_cline;
        cin >> n_warehouse >> n_stadium >> n_cline;
        vector<Warehouse> w;
        for(int i = 0; i < n_warehouse; i++){
            long x, y, s, a;
            cin >> x >> y >> s >> a;
            w.emplace_back(x, y, s, a);
        }

        vector<Stadium> s;
        for(int i = 0; i < n_stadium; i++){
            long x, y, u, d;
            cin >> x >> y >> u >> d;
            s.emplace_back(x, y, u, d);
        }

        for(int i = 0; i < n_warehouse; i++){
            for(int j = 0; j < n_stadium; j++){
                long r;
                cin >> r;
                w[i].addReveneu(r);
            }
        }

        vector<ContourLine> l;
        for(int i = 0; i < n_cline; i++){
            long x, y, r;
            cin >> x >> y >> r;
            l.emplace_back(x, y, r);
        }

        bool found = false;
        set<tuple<double,double,double>> used_cline;
        
        for(int i = 0; i < n_warehouse && !found; i++){
            K::Point_2 p(w[i].x, w[i].y);
            for(int j = 0; j < n_cline; j++){
                K::Point_2 c(l[j].x, l[j].y);
                if(CGAL::squared_distance(c, p) < (double) l[j].r * l[j].r){
                    w[i].addContourLine(l[j]);
                    used_cline.insert({l[j].x, l[j].y, l[j].r});
                }
                if(used_cline.size() == 101){
                    found = true;
                    break;
                }
            }
        }

        for(int i = 0; i < n_stadium && !found; i++){
            K::Point_2 p(s[i].x, s[i].y);
            for(int j = 0; j < n_cline; j++){
                K::Point_2 c(l[j].x, l[j].y);
                if(CGAL::squared_distance(c, p) < (double) l[j].r * l[j].r){
                    s[i].addContourLine(l[j]);
                    used_cline.insert({l[j].x, l[j].y, l[j].r});
                }
                if(used_cline.size() == 101){
                    found = true;
                    break;
                }
            }
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0); 

        int variable = 0;
        for(int i = 0; i < n_warehouse; i++){
            for(int j = 0; j < n_stadium; j++){
                set<tuple<double,double,double>> inters;
                for(auto &c : w[i].contained)
                    inters.insert({c.x, c.y, c.r});
                for(auto &c : s[j].contained){
                    if(inters.find({c.x, c.y, c.r}) != inters.end()){
                        inters.erase({c.x, c.y, c.r});
                    } else{
                        inters.insert({c.x, c.y, c.r});
                    }
                }
                lp.set_c(variable, (double) -100.0 * w[i].revenues[j] + inters.size());
                variable++;
            }
        }

        variable = 0;
        int row = 0;
        for(int i = 0; i < n_warehouse; i++){
            for(int j = 0; j < n_stadium; j++){ 
                lp.set_a(variable, row, 1); // sum[a(w,j)] <= liter per warehouse
                variable++;
            }
            lp.set_b(row, w[i].s);
            row++;
        }

        for(int j = 0; j < n_stadium; j++){
            variable = j;
            for(int i = 0; i < n_warehouse; i++){
                lp.set_a(variable, row, 1); // sum[a(w, j)] <= demand(j)
                variable += n_stadium;
            }
            lp.set_b(row, s[j].d);
            row++;
        }

        for(int j = 0; j < n_stadium; j++){
            variable = j;
            for(int i = 0; i < n_warehouse; i++){
                lp.set_a(variable, row, -1); // sum[a(w, j)] >= demand(j)
                variable += n_stadium;
            }
            lp.set_b(row, -s[j].d);
            row++;
        }

        for(int j = 0; j < n_stadium; j++){
            variable = j;
            for(int i = 0; i < n_warehouse; i++){
                lp.set_a(variable, row, w[i].a); // sum[a(w, j) * alcool percentage] <= upper limit stadium
                variable += n_stadium;
            }
            lp.set_b(row, 100.0 * s[j].u);
            row++;
        }
        
        // solve the program, using ET as the exact type
        Solution sol = CGAL::solve_linear_program(lp, ET());
        assert(sol.solves_linear_program(lp));

        if (sol.is_infeasible()) {
            cout << "RIOT!\n";
        } else{
            cout << floor(CGAL::to_double(-sol.objective_value() / 100.0)) << "\n";
        }
    }
    return 0;
}