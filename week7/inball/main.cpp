#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

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

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n, d;
    
    while (true)
    {
        cin >> n;
        if(n == 0) break;
        cin >> d;
        vector<vector<int>> ineq(n, vector<int>(d + 1));
        for (int i = 0; i < n; i++)
            for (int j = 0; j <= d; j++)
                cin >> ineq[i][j];

        Program lp(CGAL::SMALLER, false, 0, false, 0);

        int var_radius = d;       // variable radius
        lp.set_c(var_radius, -1); // Max radius
        
        for (int i = 0; i < ineq.size(); i++)
        {
            int distance = 0;
            // constraints
            // sum 1 -> d of (aj*xj) + radius*distance <= bi
            // sum 1 -> d of (aj*xj) <= bi
            for (int j = 0; j < d; j++)
            {
                lp.set_a(j, i, ineq[i][j]); // var. dimension, line, value
                lp.set_a(j, i + ineq.size(), ineq[i][j]);
                distance += ineq[i][j] * ineq[i][j];
            }
            lp.set_a(var_radius, i, sqrt(distance));
            lp.set_b(i, ineq[i][d]);
            lp.set_b(i + ineq.size(), ineq[i][d]);
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        assert(s.solves_linear_program(lp));
        if (s.is_optimal())
            cout << -ceil_to_double(s.objective_value()) << "\n";
        else if (s.is_unbounded())
            cout << "inf\n";
        else
            cout << "none\n";
    }
    return 0;
}