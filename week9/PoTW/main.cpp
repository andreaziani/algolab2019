#include <iostream>
#include <vector>
#include <climits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

class Point
{
public:
  double x;
  double y;
  Point(double x, double y)
  {
    this->x = x;
    this->y = y;
  }
  Point() {}
};

int main()
{
  std::ios_base::sync_with_stdio(false);
  cout << fixed << setprecision(0);
  int t;
  cin >> t;
  while (t--)
  {
    double n, m, h, w;
    cin >> n >> m >> h >> w;
    vector<Point> free_nails(n);
    vector<Point> old_nails(m);

    Program lp(CGAL::SMALLER, true, 1, false, 0);

    for (int i = 0; i < n; i++)
    {
      double x, y;
      cin >> x >> y;
      free_nails[i] = Point(x, y);
    }

    double poster_slope = h / w;
    for (int i = 0; i < m; i++)
    {
      double x, y;
      cin >> x >> y;
      old_nails[i] = Point(x, y);
    }

    int constraint_row = 0;
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        double slope = (free_nails[i].y - free_nails[j].y) / (free_nails[i].x - free_nails[j].x);
        // TOP OR BOTTOM
        if (slope > poster_slope || slope < (-poster_slope))
        {
          int y1 = free_nails[i].y > free_nails[j].y ? free_nails[i].y : free_nails[j].y;
          int y2 = free_nails[i].y > free_nails[j].y ? free_nails[j].y : free_nails[i].y;
          lp.set_a(i, constraint_row, h);
          lp.set_a(j, constraint_row, h);
          lp.set_b(constraint_row, 2.0 * y1 - 2.0 * y2); //  alfa1 * h/2 + alfa2 * h/2 <= y1 - y2
          constraint_row++;
        }
        // RIGHT OR LEFT
        else
        {
          int x1 = free_nails[i].x > free_nails[j].x ? free_nails[i].x : free_nails[j].x;
          int x2 = free_nails[i].x > free_nails[j].x ? free_nails[j].x : free_nails[i].x;
          lp.set_a(i, constraint_row, w);
          lp.set_a(j, constraint_row, w);
          lp.set_b(constraint_row, 2.0 * x1 - 2.0 * x2); //  alfa1 * h/2 + alfa2 * h/2 <= y1 - y2
          constraint_row++;
        }
      }
    }

    for (int i = 0; i < n; i++)
    {
      //old closest posters to the considered one
      pair<Point, int> best_right{Point(INT32_MIN, INT32_MIN), -1}; //{nail, constraint_row}
      pair<Point, int> best_left{Point(INT32_MAX, INT32_MAX), -1};
      pair<Point, int> best_up{Point(INT32_MIN, INT32_MIN), -1};
      pair<Point, int> best_down{Point(INT32_MAX, INT32_MAX), -1};

      lp.set_c(i, -1);

      Point new_nail = free_nails[i];

      //Find 4 old posters that limit the new poster.
      for (int y = 0; y < m; y++)
      {
        Point old_nail = old_nails[y];
        double slope = (free_nails[i].y - old_nail.y) / (free_nails[i].x - old_nail.x);
        if (slope > poster_slope || slope < (-poster_slope))
        {
          //TOP
          if (new_nail.y > old_nail.y)
          {
            if (best_up.second == -1)
            {
              best_up = {old_nail, constraint_row};
              lp.set_a(i, constraint_row, h);
              lp.set_b(constraint_row, 2 * new_nail.y - 2 * old_nail.y - h);
              constraint_row++;
            }
            else if (old_nail.y > best_up.first.y)
            {
              best_up.first = old_nail;
              lp.set_b(best_up.second, 2 * new_nail.y - 2 * old_nail.y - h);
            }
          }
          //BOTTOM
          else
          {
            if (best_down.second == -1)
            {
              best_down = {old_nail, constraint_row};
              lp.set_a(i, constraint_row, h);
              lp.set_b(constraint_row, -2 * new_nail.y + 2 * old_nail.y - h);
              constraint_row++;
            }
            else if (old_nail.y < best_down.first.y)
            {
              best_down.first = old_nail;
              lp.set_b(best_down.second, -2 * new_nail.y + 2 * old_nail.y - h);
            }
          }
        }
        else
        {
          //RIGHT
          if (new_nail.x > old_nail.x)
          {
            if (best_right.second == -1)
            {
              best_right = {old_nail, constraint_row};
              lp.set_a(i, constraint_row, w);
              lp.set_b(constraint_row, 2 * new_nail.x - 2 * old_nail.x - w);
              constraint_row++;
            }
            else if (old_nail.x > best_right.first.x)
            {
              best_right.first = old_nail;
              lp.set_b(best_right.second, 2 * new_nail.x - 2 * old_nail.x - w);
            }
          }
          //LEFT
          else
          {
            if (best_left.second == -1)
            {
              best_left = {old_nail, constraint_row};
              lp.set_a(i, constraint_row, w);
              lp.set_b(constraint_row, -2 * new_nail.x + 2 * old_nail.x - w);
              constraint_row++;
            }
            else if (old_nail.x < best_left.first.x)
            {
              best_left.first = old_nail;
              lp.set_b(best_left.second, -2 * new_nail.x + 2 * old_nail.x - w);
            }
          }
        }
      }
    }

    lp.set_c0(0);
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    std::cout << ceil(CGAL::to_double(s.objective_value() * ET(-2.0 * (w + h)))) << "\n";
  }
  return 0;
}