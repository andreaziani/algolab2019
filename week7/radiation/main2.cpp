#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
using namespace std;

class Coord {
    public: ET x, y, z;
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    while(t-- > 0){
        int tumor, healthy, d;
        bool found = false;
        cin >> healthy >> tumor;

        vector<Coord> h_coords(healthy, Coord());
        vector<Coord> t_coords(tumor);
        for(int i = 0; i < healthy; i++){
            cin >> h_coords[i].x  >> h_coords[i].y >> h_coords[i].z;
        }

        for(int i = 0; i < tumor; i++){
            cin >> t_coords[i].x >> t_coords[i].y >> t_coords[i].z;
        }

        for(d = 0; d <= 30; d++){
            Program lp (CGAL::SMALLER, false, 0, false, 0);
            CGAL::Quadratic_program_options options; 
            options.set_pricing_strategy(CGAL::QP_BLAND);
 
            for(int i = 0; i < healthy; i++){ // constraints healthy 
                int var_index = 0;
                ET pow_x = 1;
                for(int x = 0; x <= d; x++){ //degree of x
                    if(x != 0) pow_x *= h_coords[i].x;
                    ET pow_y = 1;
                    for(int y = 0; y <= d - x; y++){ // degree of y
                        if(y != 0) pow_y *= h_coords[i].y;
                        ET pow_z = 1;
                        for(int z = 0; z <= d - x - y; z++){ // degree of z
                            if(z != 0) pow_z *= h_coords[i].z;
                            lp.set_a(var_index, i, pow_x * pow_y * pow_z);
                            var_index++;   
                        }
                    }
                }
                lp.set_b(i, -1);
            }

            for(int i = 0; i < tumor; i++){ // constraints tumoral 
                int var_index = 0;
                ET pow_x = 1;
                for(int x = 0; x <= d; x++){ //degree of x
                    if(x != 0) pow_x *= t_coords[i].x;
                    ET pow_y = 1;
                    for(int y = 0; y <= d - x; y++){ // degree of y
                        if(y != 0) pow_y *= t_coords[i].y;
                        ET pow_z = 1;
                        for(int z = 0; z <= d - x - y; z++){ // degree of z
                            if(z != 0) pow_z *= t_coords[i].z;
                            lp.set_a(var_index, i + healthy, - pow_x * pow_y * pow_z);
                            var_index++;   
                        }
                    }
                }
                lp.set_b(i + healthy, -1);
            }

            // solve the program, using ET as the exact type
            Solution s = CGAL::solve_linear_program(lp, ET(), options);
            assert(s.solves_linear_program(lp));
            
            if(!s.is_infeasible()){
                found = true;
                break;
            }
        }

        if(!found) cout << "Impossible!\n";
        else cout << d << "\n";
        
    }
    return 0;
}