#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_index_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

bool haveFMatches(const vector<string> &s1, const vector<string> &s2, int f){ // O(n)
    int i1 = 0, i2 = 0;
    while(i1 < s1.size() && i2 < s2.size() && f > 0){
        if(s1[i1] == s2[i2]) f--;
        if(s1[i1] > s2[i2]) i2++;
        else i1++;
    }
    return f <= 0;
}

bool perfect_matching(const Graph &G) { //O(m*n * alfa(m,n))
    int n = num_vertices(G);
    const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
    std::vector<Vertex> mate(n); 
    boost::edmonds_maximum_cardinality_matching(G, &mate[0]);

    for(int i = 0; i < n; ++i)  // if one vertex is not matched, it's not a perfect matching
        if (mate[i] == NULL_VERTEX) return false;
    
    return true;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    while(t-- > 0){
        int n, c, f;
        cin >> n >> c >> f;
        vector<vector<string>> student_chars(n, vector<string>(c));
        
        for(int i = 0; i < n; i++){
            for(int j = 0; j < c; j++)
                cin >> student_chars[i][j];
            sort(student_chars[i].begin(), student_chars[i].end()); // O(nlogn)
        }
            
        Graph g(n);
        for(int i = 0; i < n; i++) //O(n^2 * c)
            for(int j = i + 1; j < n; j++)
                if(haveFMatches(student_chars[i], student_chars[j], f + 1))  
                    add_edge(i, j, g); // add to the graph only the edges which have more than f equal characteristics
                
        
        cout << (perfect_matching(g) ? "not optimal\n" : "optimal\n");
    }
    return 0;
}
