#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

using namespace std;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void update_laser_segments_segment(Segment &seg, CGAL::Object intersec_obj) {
  if (const Point* op = CGAL::object_cast<Point>(&intersec_obj)) { // ray hits a point
    seg = Segment(seg.source(), *op);
  } else if(const Segment* os = CGAL::object_cast<Segment>(&intersec_obj)) { // ray hits a segment
    if(CGAL::collinear_are_ordered_along_line(seg.source(), (*os).source(), (*os).target())) {
      // order is: laser source -> start of segment -> end of segment
      seg = Segment(seg.source(), (*os).source());
    } else {
      seg = Segment(seg.source(), (*os).target()); // degenerate segment
    }
  } else throw runtime_error("unexpected intersection");
}

int main() {
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while(true) {
    int n;
    cin >> n;
    if(n == 0) break;
    
    long r_x, r_y, r_a, r_b;
    cin >> r_x >> r_y >> r_a >> r_b;
    Ray r = Ray(Point(r_x, r_y), Point(r_a, r_b));

    vector<Segment> segments(n);
    for(int i = 0; i < n; i++) {
      long x, y, a, b;
      cin >> x >> y >> a >> b;
      segments[i] = Segment(Point(x, y), Point(a, b));
    }

    random_shuffle(segments.begin(), segments.end()); // generally a good idea

    // search for one intersection between the laser ray and an obsticle, create a segment that starts at the laser source and ends
    // at the point where the laser hits the obsticle
    bool found = false;
    Segment seg(r.source(), r.source());
    int i = 0; // jump over already checked segments in the second for-loop

    for(; i < n; i++) { // find the first hitting point
      if(do_intersect(segments[i], r)) {
        found = true;
        update_laser_segments_segment(seg, intersection(segments[i], r));
        break;
      }
    }

    // check if we hit something
    if(!found) {
      cout << "no" << '\n';
      continue;
    }

    // we search if our segment hits the other, if yes we update the target with the hitting point
    for(; i < n; i++) {
      if(do_intersect(seg, segments[i])) {
        update_laser_segments_segment(seg, intersection(segments[i], r));
      }
    }
    cout << floor_to_double(seg.target().x()) << " " << floor_to_double(seg.target().y()) << '\n';
  }
}