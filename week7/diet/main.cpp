#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>
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

void solve(vector<pair<int,int>> nutrients, vector<pair<int, vector<int>>> foods){
    Program lp (CGAL::SMALLER, true, 0, false, 0); 

    for(int i = 0; i < nutrients.size(); i++){
        for(int j = 0; j < foods.size(); j++){ // sum of nuts <= max
            lp.set_a(j, i,  foods[j].second[i]);
        }
        lp.set_b(i, nutrients[i].second);
    }

    for(int i = 0; i < nutrients.size(); i++){
        for(int j = 0; j < foods.size(); j++){ // sum of nuts >= min
            lp.set_a(j, i + nutrients.size(),  -foods[j].second[i]);
        }
        lp.set_b(i + nutrients.size(), -nutrients[i].first);
    }

    for(int j = 0; j < foods.size(); j++){ 
        lp.set_c(j, foods[j].first);// xj * price
    }
    lp.set_c0(0);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if(!s.is_infeasible())
        cout << floor(CGAL::to_double(s.objective_value())) << "\n";     
    else 
        cout << "No such diet.\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    while(n != 0 || m != 0){
        vector<pair<int,int>> nutrients(n); //min & max
        vector<pair<int, vector<int>>> foods;
        for(int i = 0; i < n; i++)
            cin >> nutrients[i].first >> nutrients[i].second;
        for(int i = 0; i < m; i++){
            int p;
            cin >> p;
            vector<int> nut_amount(n);
            for(int j = 0; j < n; j++)
                cin >> nut_amount[j];
            
            foods.push_back({p, nut_amount});
        }
        solve(nutrients, foods);  
        cin >> n >> m;
    }
    return 0;
}