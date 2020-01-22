#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
using namespace std;

struct cell{
    ET x, y, z;
    cell(double x, double y, double z){
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
        int h, t;
        cin >> h >> t;
        vector<cell> healthy;
        vector<cell> tumoral;
        for(int i = 0; i < h; i++){
            double x, y, z;
            cin >> x >> y >> z;
            healthy.emplace_back(x, y, z);
        }

        for(int i = 0; i < t; i++){
            double x, y, z;
            cin >> x >> y >> z;
            tumoral.emplace_back(x, y, z);
        }
        
        bool found = false;
        int d;
        for(d = 0; d <= 30; d++){
            
            Program lp (CGAL::SMALLER, false, 0, false, 0);
            CGAL::Quadratic_program_options options; 
            options.set_pricing_strategy(CGAL::QP_BLAND);
 
            for(int i = 0; i < h; i++){
                ET pow_x = 1;
                int var_counter = 0;
                for(int x = 0; x <= d; x++){ //healthy
                    if(x != 0) pow_x *= healthy[i].x;
                    ET pow_y = 1;
                    for(int y = 0; y <= d - x; y++){
                        if(y != 0) pow_y *= healthy[i].y;
                        ET pow_z = 1;
                        for(int z = 0; z <= d - x - y; z++){
                            if(z != 0) pow_z *= healthy[i].z;  
                            lp.set_a(var_counter++, i, pow_x * pow_y * pow_z);
                        }
                    }
                }
                lp.set_b(i, -1);
            }

            for(int i = 0; i < t; i++){
                int var_counter = 0;
                ET pow_x = 1;
                for(int x = 0; x <= d; x++){ 
                    if(x != 0) pow_x *= tumoral[i].x;
                    ET pow_y = 1;
                    for(int y = 0; y <= d - x; y++){
                        if(y != 0) pow_y *= tumoral[i].y;
                        ET pow_z = 1;
                        for(int z = 0; z <= d - x - y; z++){
                            if(z != 0) pow_z *= tumoral[i].z;  
                            lp.set_a(var_counter++, i + h, - pow_x * pow_y * pow_z);
                        }
                    }
                }
                lp.set_b(i + h, -1);
            }

            Solution s = CGAL::solve_linear_program(lp, ET(), options);
            assert(s.solves_linear_program(lp));
            
            if(!s.is_infeasible()){
                found = true;
                break;
            }

        }

        if(found)
            cout << d << "\n";
        else cout << "Impossible!\n";
    }
    return 0;

}