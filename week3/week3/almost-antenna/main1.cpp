#include <iostream>
#include <cmath>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 Point;

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
   	int n;
    while (std::cin >> n && n != 0) {
        vector<Point> coords(n);
        for(int i = 0; i < n; i++){
            long x, y; 
            cin >> x >> y;
            coords[i] = Point(x, y);
        }

        Min_circle min_circle (coords.begin(), coords.end(), true);
        // remove one support point each turn and take the circle with min radius
	    K::FT min_rad = min_circle.circle().squared_radius();
	    for(auto iter = min_circle.support_points_begin(); iter != min_circle.support_points_end(); iter++) {
	    	for(int i = 0; i < n; i++) {
	    		if(coords[i] == *iter) {
	    			int add = i > 0 ? -1 : 1; // put coords[i] equal to next or prev element 
	    			coords[i] = coords[i + add];

	    			Min_circle tmp(coords.begin(), coords.end(), true);
	    			K::FT rad = tmp.circle().squared_radius();
	    			if(rad < min_rad) {
	    				min_rad = rad;
	    			}
	    			coords[i] = *iter;
	    			break;
	    		}
	    	}
	    }
		min_rad = sqrt(min_rad);
	    cout << ceil_to_double(min_rad) << '\n';
    }
    return 0;
}