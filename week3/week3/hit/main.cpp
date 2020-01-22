#include <stdlib.h>
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

using namespace std;
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
        for(int i = 0; i < n; i++)
            if(CGAL::do_intersect(ray, seg[i])){
                cout << "yes" << '\n';
                intersect = true;
                break;
            }
        if(!intersect) cout << "no" << '\n';
    }
    
    return 0;
}