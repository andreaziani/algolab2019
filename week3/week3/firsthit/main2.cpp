#include <stdlib.h>
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

using namespace std;

double floor_to_double(const EK::FT& x){
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    while(true){
        bool intersect = false;
        int n; 
        cin >> n;
        if(n == 0) break;
        long r_x, r_y, r_a, r_b;
        cin >> r_x >> r_y >> r_a >> r_b; 
        EK::Point_2 p(r_x, r_y), q(r_a, r_b);
        EK::Ray_2 ray(p, q);
        vector<EK::Segment_2> seg(n);
        for(int i = 0; i < n; i++){
            long a, b, c, d;
            cin >> a >> b >> c >> d;
            EK::Point_2 p1(a,b), q1(c, d);
            EK::Segment_2 s(p1, q1);
            seg[i] = s;
        }

        EK::Point_2 best_pt;
        for(int i = 0; i < n; i++){    
            if(CGAL::do_intersect(ray, seg[i])){
                auto o = CGAL::intersection(ray, seg[i]);
                if (const EK::Point_2* op = boost::get<EK::Point_2>(&*o)){
                    if(!intersect || CGAL::has_smaller_distance_to_point (p, *op, best_pt)){
                        best_pt = *op;
                        intersect = true;
                    }
                }
                else if (const EK::Segment_2* os = boost::get<EK::Segment_2>(&*o)){
                    EK::Point_2 min = CGAL::has_smaller_distance_to_point (p, os->source(), os->target())? os->source() : os->target();
                    if(!intersect || CGAL::has_smaller_distance_to_point (p, min, best_pt)){
                        best_pt = min;
                        intersect = true;
                    }
                } else throw runtime_error("unexpected intersection");
            }
        }
        if(intersect) cout << (long) floor_to_double(best_pt.hx()) << " " << (long) floor_to_double(best_pt.hy()) << '\n';
        else cout << "no" << '\n';
    }
    
    return 0;
}