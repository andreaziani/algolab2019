#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Circle_2 Circle;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> PSet;
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle Vertex_handle;

using namespace std;

int main() {
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);
  int test_count;
  cin >> test_count;

  for(int test = 0; test < test_count; test++) {
    int participant_count, lamp_count;
    cin >> participant_count >> lamp_count;

    vector<K::Point_2> participant_locations(participant_count);
    vector<int> participant_radi(participant_count, -1);
    for(int participant_index = 0; participant_index < participant_count; participant_index++) {
      double p_x, p_y, p_r;
      cin >> p_x >> p_y >> p_r;

      participant_locations.at(participant_index) = K::Point_2(p_x, p_y);
      participant_radi.at(participant_index) = p_r;
    }

    int lamp_height;
    cin >> lamp_height;

    vector<pair<K::Point_2, int> > lamp_locations(lamp_count);
    for(int lamp_index = 0; lamp_index < lamp_count; lamp_index++) {
      double l_x, l_y;
      cin >> l_x >> l_y;

      lamp_locations.at(lamp_index) = make_pair(K::Point_2(l_x, l_y), lamp_index);
    }

    PSet lamps_triang(lamp_locations.begin(), lamp_locations.end());

    // contains the ID of the light that hit a participant first, or MAX if he survived till the end
    vector<int> min_light_hit(participant_count, numeric_limits<int>::max());

    // keeps track of the max light that hit someone
    int max_light_hit = -1;

    // now we iterate over each person and find out which lamp hits each person first, i.e. how
    // long they survive
    for(int participant_index = 0; participant_index < participant_count; participant_index++) {
      K::Point_2 participant_point = participant_locations.at(participant_index); // we need it more than once :-)
      // get nearest lamp
      Vertex_handle lamp_vertex = lamps_triang.nearest_neighbor(participant_point);

      double max_dist_to_kill = pow(participant_radi.at(participant_index) + lamp_height, 2);

      // if the nearest lamp is already too far away, we don't have to search for other lamps,
      // as they will even farther away
      if(max_dist_to_kill <= squared_distance(lamp_vertex->point(), participant_point)) {
        continue;
      }

      // iterate over all lamps in the search for one that hits the participant first
      for(int check_lamp_index = 0; check_lamp_index < lamp_count; check_lamp_index++) {
        if(max_dist_to_kill > squared_distance(lamp_locations.at(check_lamp_index).first, participant_point)) {
          // found such a lamp, stop searching as we only interested in the minimum
          min_light_hit.at(participant_index) = lamp_locations.at(check_lamp_index).second;
          max_light_hit = max(max_light_hit, lamp_locations.at(check_lamp_index).second);
          break;
        }
      }
    }

    vector<int> rank_list;
    for(int participant_index = 0; participant_index < participant_count; participant_index++) {
      if(min_light_hit.at(participant_index) == numeric_limits<int>::max()) {
        rank_list.push_back(participant_index);
      }
    }

    if(rank_list.size() <= 0) {
      for(int participant_index = 0; participant_index < participant_count; participant_index++) {
        if(min_light_hit.at(participant_index) == max_light_hit) {
          rank_list.push_back(participant_index);
        }
      }
    }

    sort(rank_list.begin(), rank_list.end());
    for(int i = 0; i < rank_list.size(); i++) {
      cout << rank_list.at(i) << " ";
    }
    cout << "\n";
  }
}