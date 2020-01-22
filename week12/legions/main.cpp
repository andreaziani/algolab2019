#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
using namespace std;

int ceil_to_double(const CGAL::Quotient<ET> &x)
{
    double a = ceil(CGAL::to_double(x));
    while (a < x)
        a += 1;
    while (a - 1 >= x)
        a -= 1;
    return a;
}

struct Line{
    double a, b, c, v;
    Line(double a, double b, double c, double v){
        this->a = a;
        this->b = b;
        this->c = c;
        this->v = v;
    }
    Line(){};
};

int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int t;
    cin >> t;
    while(t--){
        double xs, ys, n;
        cin >> xs >> ys >> n;

        vector<Line> lines;
        for(int i = 0; i < n; i++){
            double a, b, c, v;
            cin >> a >> b >> c >> v;
            lines.emplace_back(a, b, c, v);
        }

        Program lp (CGAL::SMALLER, false, 0, false, 0);
        const int z = 0;
        const int px = 1;
        const int py = 2;

        lp.set_c(z, -1); // MAX z
        
        for(int i = 0; i < n; i++){
            K::Line_2 line = K::Line_2(lines[i].a, lines[i].b, lines[i].c);
            bool side = line.has_on_positive_side(K::Point_2(xs, ys));
            if(side){
                lp.set_a(z, i, lines[i].v * (sqrt((lines[i].a * lines[i].a) + (lines[i].b * lines[i].b))));
                lp.set_a(px, i, -lines[i].a);
                lp.set_a(py, i, -lines[i].b);
                lp.set_b(i, lines[i].c);
            } else{
                lp.set_a(z, i, lines[i].v * (sqrt((lines[i].a * lines[i].a) + (lines[i].b * lines[i].b))));
                lp.set_a(px, i, lines[i].a);
                lp.set_a(py, i, lines[i].b);
                lp.set_b(i, -lines[i].c);
            }
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        cout << -ceil(CGAL::to_double(s.objective_value())) << "\n";
    }
    return 0;

}