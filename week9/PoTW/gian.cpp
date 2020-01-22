#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <set>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct coordinate {
public:
    double x;
    double y;
    coordinate(double x1, double y1) {
        x = x1;
        y = y1;
    }

    coordinate() {}
};

struct best_point {
    coordinate coord{};
    int row;
    bool first;

    best_point(coordinate coord, int row, bool first = true) : coord(coord), row(row), first(first) {}
};

double calculate_slope(const coordinate &p0, const coordinate &p1) {
    return (p1.y - p0.y) / (p1.x - p0.x);
}

double organize_posters() {
    double new_poster, old_poster, h, w;
    cin >> new_poster >> old_poster >> h >> w;

    vector<coordinate> new_nails;
    vector<coordinate> old_nails;

    for (int i = 0; i < new_poster; i++) {
        double x, y;
        cin >> x >> y;
        new_nails.emplace_back(x, y);
    }

    for (int i = 0; i < old_poster; i++) {
        double x, y;
        cin >> x >> y;
        old_nails.emplace_back(x, y);
    }

    Program lp(CGAL::SMALLER, true, 1, false, 0);

    //Calculate general slope. Equal for all posters
    double max_slope = calculate_slope(coordinate(0, 0), coordinate(w / 2, h / 2));
    double min_slope = -max_slope;

    int row = 0;
    for (int x = 0; x < new_poster; x++) {
        //Saves the old closest poster to the considered one
        best_point best_right{{INT32_MIN, INT32_MIN}, 0};
        best_point best_left{{INT32_MAX, INT32_MAX}, 0};
        best_point best_up{{INT32_MIN, INT32_MIN}, 0};
        best_point best_down{{INT32_MAX, INT32_MAX}, 0};

        lp.set_c(x, -1);

        coordinate new_nail(new_nails[x].x, new_nails[x].y);

        //Calculate the max 4 old posters that limit the new poster.
        for (int y = 0; y < old_poster; y++) {
            coordinate old_nail(old_nails[y].x, old_nails[y].y);
            double slope_from_old = calculate_slope(old_nail, new_nail);

            //If the poster is on the right or the left of the old one
            if (slope_from_old >= min_slope && slope_from_old <= max_slope) {
                //If it is on the right
                if (new_nail.x > old_nail.x) {
                    if (best_right.first) {
                        best_right = best_point(coordinate(old_nail.x, old_nail.y), row, false);
                        lp.set_a(x, row, w);
                        lp.set_b(row, 2 * new_nail.x - 2 * old_nail.x - w);
                        row++;
                    } else if (old_nail.x > best_right.coord.x) {
                        best_right.coord = {old_nail.x, old_nail.y};
                        lp.set_b(best_right.row, 2 * new_nail.x - 2 * old_nail.x - w);
                    }
                } else {
                    if (best_left.first) {
                        best_left = best_point(coordinate(old_nail.x, old_nail.y), row, false);
                        lp.set_a(x, row, w);
                        lp.set_b(row, -2 * new_nail.x + 2 * old_nail.x - w);
                        row++;
                    } else if (old_nail.x < best_left.coord.x) {
                        best_left.coord = {old_nail.x, old_nail.y};
                        lp.set_b(best_left.row, -2 * new_nail.x + 2 * old_nail.x - w);
                    }
                }
            }
                //The new poster is up or down to the old poster
            else {
                //If it is up
                if (new_nail.y > old_nail.y) {
                    if (best_up.first) {
                        best_up = best_point(coordinate(old_nail.x, old_nail.y), row, false);
                        lp.set_a(x, row, h);
                        lp.set_b(row, 2 * new_nail.y - 2 * old_nail.y - h);
                        row++;
                    } else if (old_nail.y > best_up.coord.y) {
                        best_up.coord = {old_nail.x, old_nail.y};
                        lp.set_b(best_up.row, 2 * new_nail.y - 2 * old_nail.y - h);
                    }
                } else {
                    if (best_down.first) {
                        best_down = best_point(coordinate(old_nail.x, old_nail.y), row, false);
                        lp.set_a(x, row, h);
                        lp.set_b(row, -2 * new_nail.y + 2 * old_nail.y - h);
                        row++;
                    } else if (old_nail.y < best_down.coord.y) {
                        best_down.coord = {old_nail.x, old_nail.y};
                        lp.set_b(best_down.row, -2 * new_nail.y + 2 * old_nail.y - h);
                    }
                }
            }
        }

        //Set costraints for the new posters to not overlap on other new posters
        for (int y = x + 1; y < new_poster; y++) {
            coordinate old_nail(new_nails[y].x, new_nails[y].y);
            double new_slope = calculate_slope(new_nail, old_nail);

            if (new_slope >= min_slope && new_slope <= max_slope) {
                //If the poster is on the right
                if (new_nail.x > old_nail.x) {
                    lp.set_a(x, row, w);
                    lp.set_a(y, row, w);
                    lp.set_b(row, 2 * new_nail.x - 2 * old_nail.x);
                } else {
                    lp.set_a(x, row, w);
                    lp.set_a(y, row, w);
                    lp.set_b(row, -2 * new_nail.x + 2 * old_nail.x);
                }
            } else {
                //If the poster is on top
                if (new_nail.y > old_nail.y) {
                    lp.set_a(x, row, h);
                    lp.set_a(y, row, h);
                    lp.set_b(row, 2 * new_nail.y - 2 * old_nail.y);
                } else {
                    lp.set_a(x, row, h);
                    lp.set_a(y, row, h);
                    lp.set_b(row, -2 * new_nail.y + 2 * old_nail.y);
                }
            }
            row++;
        }
    }

    lp.set_c0(0);
    Solution s = CGAL::solve_linear_program(lp, ET());
    return ceil(CGAL::to_double(s.objective_value() * ET(-2.0 * (w + h))));
}

int main() {
    ios_base::sync_with_stdio(0);
    cout << fixed << setprecision(0);

    int num_test;
    cin >> num_test;

    for (int t = 0; t < num_test; t++) {
        cout << organize_posters() << "\n";
    }
}