#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <random> 
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
using namespace std;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    while(true){
        int n;
        cin >> n;
    
        if(n == 0) break;

        double a, b, x, y;
        cin >> x >> y >> a >> b;
        K::Ray_2 line = K::Ray_2(K::Point_2(x, y), K::Point_2(a, b));
        vector<K::Segment_2> segments;
        for(int i = 0; i < n; i++){
            double r, s, t, u;
            cin >> r >> s >> t >> u;
            segments.push_back(K::Segment_2(K::Point_2(r, s), K::Point_2(t, u)));
        }
        
        random_shuffle(segments.begin(), segments.end());

        K::Segment_2 s_line = K::Segment_2(K::Point_2(x, y), K::Point_2(a, b));
        bool inside = false;
        int i = 0;
        for(; i < n; i++){
            if(CGAL::do_intersect(line, segments[i])){
                auto result = CGAL::intersection(line, segments[i]);
                if (result) {
                    if (const K::Segment_2* s = boost::get<K::Segment_2>(&*result)) {
                        if(CGAL::collinear_are_ordered_along_line(s_line.source(), (*s).source(), (*s).target())) {
                            // order is: laser source -> start of segment -> end of segment
                            s_line = K::Segment_2(s_line.source(), (*s).source());
                        } else {
                            s_line = K::Segment_2(s_line.source(), (*s).target()); // degenerate segment
                        }
                    } else {
                        const K::Point_2* p = boost::get<K::Point_2 >(&*result);
                        s_line = K::Segment_2(K::Point_2(x,y), *p);
                    }
                    inside = true;
                    break;
                }
            }
        }

        if(inside){
            for(; i < n; i++){
                if(CGAL::do_intersect(s_line, segments[i])){
                    auto result = CGAL::intersection(line, segments[i]);
                    if (result) {
                        if (const K::Segment_2* s = boost::get<K::Segment_2>(&*result)) {
                            if(CGAL::collinear_are_ordered_along_line(s_line.source(), (*s).source(), (*s).target())) {
                                // order is: laser source -> start of segment -> end of segment
                                s_line = K::Segment_2(s_line.source(), (*s).source());
                            } else {
                                s_line = K::Segment_2(s_line.source(), (*s).target()); // degenerate segment
                            }
                        } else {
                            const K::Point_2* p = boost::get<K::Point_2 >(&*result);
                            s_line = K::Segment_2(K::Point_2(x,y), *p);
                        }
                    }
                }
            }
            cout << floor_to_double(s_line.target().x()) << " " << floor_to_double(s_line.target().y()) << "\n";
        } else {
            cout << "no\n";
        }
    
    }
    return 0;
}