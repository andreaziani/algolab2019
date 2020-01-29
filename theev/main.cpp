#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
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
    int t; cin >> t;
    while(t--){
        int n; 
        cin >> n;
        std::vector<K::Point_2> cities(n);

        for(int i = 0; i < n; i++)
            cin >> cities[i];
        
        vector<pair<K::FT, K::Point_2>> distances;
        for(int i = 0; i < n; i++){
            distances.push_back({CGAL::squared_distance(cities[0], cities[i]), cities[i]});
        }

        sort(distances.begin(), distances.end(), 
        [](pair<K::FT, K::Point_2> &p1, pair<K::FT, K::Point_2> &p2) -> bool { return p1.first < p2.first; });

        K::FT min_radius = INT64_MAX;
        
        vector<K::Point_2> tmp;
        for(int i = 0; i < n; i++)
            tmp.push_back(distances[i].second);

        for(int i = 1; i < n; i++){
            Min_circle mc(tmp.begin() + i, tmp.end(), true);
            Traits::Circle c = mc.circle();
            K::FT dist = std::max(c.squared_radius(), distances[i - 1].first);
            min_radius = min(dist, min_radius);
        }

        cout << fixed << setprecision(0);
        cout << ceil_to_double(min_radius) << "\n";
        
    }
    return 0;
}