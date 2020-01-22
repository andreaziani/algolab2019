#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

bool hasKmatching(vector<string> &v1, vector<string> &v2, int k){
    int i1 = 0, i2 = 0;
    while(i1 != v1.size() && i2 != v2.size()){
        if(v1[i1] == v2[i2]){
            k--;
            i1++; 
            i2++;
        } else if(v1[i1] < v2[i2]) {
            i1++;
        }else if(v1[i1] > v2[i2]){ 
            i2++;
        }
    }
    return k <= 0;
}


bool perfect_maximum_matching(const graph &G, int n) {
    std::vector<vertex_desc> mate_map(n);  // exterior property map
    const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

    boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
    //is a perfect matching if every node is paired with someone
    for (int i = 0; i < n; ++i) {
        // mate_map[i] != NULL_VERTEX: the vertex is matched
        if (mate_map[i] == NULL_VERTEX && i < mate_map[i]) 
            return false;
    }

    return true;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        int n, c, f;
        cin >> n >> c >> f;
        vector<vector<string>> stud_char(n, vector<string>(c));
        for(int i = 0; i < n; i++)
            for(int j = 0; j < c; j++)
                cin >> stud_char[i][j];
        
        for(int i = 0; i < n; i++)
            sort(stud_char[i].begin(), stud_char[i].end());

        graph G(n);
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++) { // check if each pair of buddy has at least f + 1 matches
                if(hasKmatching(stud_char[i], stud_char[j], f + 1)){
                    boost::add_edge(i, j, G); 
                }
            }
        }

        if(perfect_maximum_matching(G, n))
            cout << "not optimal\n";
        else 
            cout << "optimal\n";

    }
    return 0;
}