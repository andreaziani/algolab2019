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

using namespace std;
double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	while(true) {
	    int n;
	    cin >> n;
	    if(n == 0) break;

	    vector<Point> peoples(n);
	    for(int i = 0; i < n; i++) {
	    	double x, y;
	    	cin >> x >> y;
	    	peoples[i] = Point(x, y);
	    }

	    Min_circle min_circle(peoples.begin(), peoples.end(), true);
	    cout << ceil_to_double(sqrt(min_circle.circle().squared_radius())) << '\n';
	}
}
