#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <deque>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

using namespace std;

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int n;
    while(std::cin >> n && n != 0){
        deque<K::Point_2> citizens;

        
        for(int i = 0; i < n; i++){
            double x, y;
            cin >> x >> y;
            citizens.emplace_back(x, y);
        }

        K::FT min_r = Min_circle(citizens.begin(), citizens.end(), true).circle().squared_radius();
        for(int i = 0; i < n; i++){
            auto p = citizens.front();
            citizens.pop_front();
            
            Min_circle mc(citizens.begin(), citizens.end(), true);
            Traits::Circle c = mc.circle();
            
            citizens.push_back(p);
            K::FT r = c.squared_radius();
            if(r < min_r){
                min_r = r;
            }
        }
        min_r = sqrt(min_r);
        cout << ceil_to_double(min_r) << "\n";
    }
    return 0;
}