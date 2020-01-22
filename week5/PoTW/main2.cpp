#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;


struct line {
	int idx;
	double y0, x1, y1;
};


int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--){
        int n;
        cin >> n;
        vector<line> lines(n);
        for (int i = 0; i < n; i++) {
            cin >> lines[i].y0 >> lines[i].x1 >> lines[i].y1;
            lines[i].idx = i;
        }
        
        sort(lines.begin(), lines.end(), [](line &r1, line &r2) -> bool { return r1.y0 > r2.y0;});
        
        vector<int> result;
        vector<line> down_lines;
        line last_up_line;
        last_up_line.idx = -1;
        vector<line>::iterator it;

        for (int i = 0; i < n; i++) {
            if (lines[i].y1 - lines[i].y0 >= 0) { // line is going up
                if (!down_lines.empty()) { // we have some down lines
                    for (it = down_lines.end()-1; it != down_lines.begin()-1; it--) {
                        auto r = CGAL::compare_slope(K::Line_2(K::Point_2(0, lines[i].y0), K::Point_2(lines[i].x1, lines[i].y1)), 
                                K::Line_2(K::Point_2(0, it->y0), K::Point_2((it->x1)*(-1), it->y1)));
                        if (r == CGAL::LARGER) {
                            break;
                        }
                    }
                    down_lines.erase(it+1, down_lines.end());
                }
                if (down_lines.empty()) { // no previous down lines
                    if (last_up_line.idx == -1) {
                        result.push_back(lines[i].idx);
                        last_up_line = lines[i];
                    } else {
                        auto r = CGAL::compare_slope(K::Line_2(K::Point_2(0, lines[i].y0), K::Point_2(lines[i].x1, lines[i].y1)), 
                            K::Line_2(K::Point_2(0, last_up_line.y0), K::Point_2((last_up_line.x1), last_up_line.y1)));
                        if (r != CGAL::LARGER) {
                            result.push_back(lines[i].idx);
                            last_up_line = lines[i];
                        }
                    }
                }
            } else { // line is going down
                if (!down_lines.empty()) {
                    for (it = down_lines.end() - 1; it != down_lines.begin() - 1; it--) {
                        auto r = CGAL::compare_slope(K::Line_2(K::Point_2(0, lines[i].y0), K::Point_2(lines[i].x1*(-1), lines[i].y1)), 
                                    K::Line_2(K::Point_2(0, it->y0), K::Point_2((it->x1)*(-1), it->y1)));
                        if (r != CGAL::SMALLER) {
                            break;
                        }
                    }
                    down_lines.erase(it+1, down_lines.end());
                }
                down_lines.push_back(lines[i]);
            }
        }

        for (it = down_lines.begin(); it != down_lines.end(); it++) {
            result.push_back(it->idx);
        }
        sort(result.begin(), result.end());
        for(auto i : result)
            cout << i << ' ';
        cout << "\n";
    }
}