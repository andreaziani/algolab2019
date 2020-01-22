#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

using namespace std;

struct line{
    int index;
    double y_start;
    K::Point_2 p1;
    line(int i, double x1, double y1, double y){
        index = i;
        p1 = K::Point_2(x1, y1);
        y_start = y;
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;

        vector<line> lines;
        for(int i = 0; i < n; i++){
            long long y, y1, x1;
            cin >> y >> x1 >> y1;
            lines.emplace_back(i, x1, y1, y);
        }

        sort(lines.begin(), lines.end(), [](line &l1, line &l2) -> bool {
            return l1.y_start > l2.y_start;
        });
        
        vector<double> res;
        vector<line> down_lines;
        line last_up_line(-1, 0, 0, 0);
        vector<line>::iterator it;
        for(int i = 0; i < n; i++){
            if(lines[i].p1.y() - lines[i].y_start >= 0){ // ray to the top
                if(!down_lines.empty()){
                    for(it = down_lines.end() - 1; it != down_lines.begin() - 1; it--){
                        /*
                        if(CGAL::right_turn(K::Point_2(0, 0), K::Point_2(lines[i].p1.x(), lines[i].p1.y() - lines[i].y_start), K::Point_2(it->p1.x(), it->p1.y() - it->y_start))){
                            break;
                        }*/
                        auto r = CGAL::compare_slope(K::Line_2(K::Point_2(0, lines[i].y_start), lines[i].p1), 
                                K::Line_2(K::Point_2(0, it->y_start), K::Point_2((it->p1.x())*(-1), it->p1.y())));
                        if (r == CGAL::LARGER) {
                            break;
                        }
                    }
                    down_lines.erase(it + 1, down_lines.end());
                } 
                if(down_lines.empty()){
                    if (last_up_line.index == -1) {
					    res.push_back(lines[i].index);
					    last_up_line = lines[i];
				    } else {
                        if(!CGAL::left_turn(K::Point_2(0, 0), 
                                            K::Point_2(last_up_line.p1.x(), last_up_line.p1.y() - last_up_line.y_start), 
                                            K::Point_2(lines[i].p1.x(), lines[i].p1.y() - lines[i].y_start))){
                            last_up_line = lines[i];
                            res.push_back(lines[i].index);
                        }
                    }
                }
            } else{ // ray to bot
                if (!down_lines.empty()) {
                    for(it = down_lines.end() - 1; it != down_lines.begin() - 1; it--){
                        /*
                        if(CGAL::right_turn(K::Point_2(0, 0), K::Point_2(lines[i].p1.x(), lines[i].p1.y() - lines[i].y_start), K::Point_2(it->p1.x(), it->p1.y() - it->y_start))){
                            break;
                        }*/
                        auto r = CGAL::compare_slope(K::Line_2(K::Point_2(0, lines[i].y_start), K::Point_2(lines[i].p1.x()*(-1), lines[i].p1.y())), 
                                    K::Line_2(K::Point_2(0, it->y_start), K::Point_2((it->p1.x())*(-1), it->p1.y())));
                        if (r != CGAL::SMALLER) {
                            break;
                        }
                    }
                    down_lines.erase(it + 1, down_lines.end());
			    }
                down_lines.push_back(lines[i]);
            }
        }

        for(line d : down_lines)
            res.push_back(d.index);
        
        sort(res.begin(), res.end());

        for(int i : res)
            cout << i << ' ';
        
        cout << "\n";
    }
    return 0;
}