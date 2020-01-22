#include <stdlib.h>
#include <iostream>
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

void solve1(int a, int b){
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    // set the coefficients of A and b
    const int X = 0; 
    const int Y = 1;
    // Variable, n of the entry in the matrix, value
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  //  x + y  <= 4
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  // 4x + 2y <= ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); //-x + y <= 1

    //obj func
    lp.set_c(X, a);                                       // ax
    lp.set_c(Y, -b);                                       //-by
    lp.set_c0(0);                                          // +0

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if(s.is_unbounded())
        cout << "unbounded\n";
    else if(s.is_optimal())
        cout << -ceil(CGAL::to_double(s.objective_value())) << "\n";
    else
        cout << "no\n";
}

void solve2(int a, int b){
    Program lp (CGAL::SMALLER, false, 0, true, 0); 
    // set the coefficients of A and b
    const int X = 0; 
    const int Y = 1;
    const int Z = 2;
    // Variable, n of the entry in the matrix, value
    lp.set_a(X, 0,  -1); lp.set_a(Y, 0, -1); lp.set_a(Z, 0, 0); lp.set_b(0, 4);  //  x + y  >= -4
    lp.set_a(X, 1,  -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);  //  4x +2y + z >= -ab
    lp.set_a(X, 2,  1); lp.set_a(Y, 2, -1); lp.set_a(Z, 2, 0); lp.set_b(2, 1);  //  -x + y >= -1
    
    //obj func
    lp.set_c(X, a);                                       // ax
    lp.set_c(Y, b);                                       // by
    lp.set_c(Z, 1);                                       // z
    lp.set_c0(0);                                          // +0

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if(s.is_unbounded())
        cout << "unbounded\n";
    else if(s.is_optimal())
        cout << ceil(CGAL::to_double(s.objective_value())) << "\n";
    else 
        cout << "no\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int p, a, b;
    cin >> p;
    while(p != 0){
        cin >> a >> b;
        if(p == 1)
            solve1(a,b);
        else
            solve2(a,b);
        cin >> p;
    }
    return 0;
}