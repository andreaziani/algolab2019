#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;

class Person{
    public: 
        K::Point_2 coord;
        int radius;
        int index;
    Person(K::Point_2 c, int r, int i){
        coord = c;
        radius = r;
        index = i;
    }
    Person(){}
};


vector<Person> binarySearchOnLamps(vector<Person> &p, vector<K::Point_2> &l, K::FT l_rad, int start, int end, vector<bool> &visited_lamps){    
    int m = (end + start) / 2;        
    Triangulation t;
    t.insert(l.begin(), l.begin() + m + 1);
    vector<Person> still_alive;
    
    for(int i = 0; i < p.size(); i++){
        t.locate(p[i].coord);
        auto ne = t.nearest_vertex(p[i].coord);
        K::Point_2 nearest = ne->point();
        //cout << "nearest " << nearest.x() << " " << nearest.y() << endl;
        K::FT dist = CGAL::squared_distance(p[i].coord, nearest);
        K::FT p_rad = p[i].radius;

        if(dist >= (p_rad + l_rad) * (p_rad + l_rad))
            still_alive.push_back(p[i]);
    }
    

    visited_lamps[m] = true;
    if(still_alive.empty()){
        if(m == 0 || visited_lamps[m - 1] ){
            return p;
        }

        return binarySearchOnLamps(p, l, l_rad, start, m, visited_lamps);
    }
    else {
        if((m + 1 < l.size() && visited_lamps[m + 1]) || m == l.size() - 1){
            return still_alive;
        }

        return binarySearchOnLamps(still_alive, l, l_rad, m + 1, end, visited_lamps);
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int m, n, h; 
        cin >> m >> n;
        vector<Person> pers;
        vector<K::Point_2> lamps(n); 

        for(int i = 0; i < m; i++){
            double x, y;
            int r;
            cin >> x >> y >> r;
            pers.push_back(Person(K::Point_2(x, y), r, i));
        }

        cin >> h;

        for(int i = 0; i < n; i++){
            double x, y;
            cin >> x >> y;
            lamps[i] = K::Point_2(x, y);
        }

        K::FT l_rad = h;
        vector<bool> visited_lamps(n, false);
        Triangulation t;
        t.insert(lamps.begin(), lamps.end());
        vector<Person> still_alive;
    
        for(int i = 0; i < pers.size(); i++){
            t.locate(pers[i].coord);
            auto ne = t.nearest_vertex(pers[i].coord);
            K::Point_2 nearest = ne->point();

            K::FT dist = CGAL::squared_distance(pers[i].coord, nearest);
            K::FT p_rad = pers[i].radius;

            if(dist >= (p_rad + l_rad) * (p_rad + l_rad))
                still_alive.push_back(pers[i]);
        }

        vector<Person> res;
        if(still_alive.empty())
            res = binarySearchOnLamps(pers, lamps, l_rad, 0, n - 1, visited_lamps);
        else 
            res = still_alive;

        for(int i = 0; i < res.size(); i++)
            cout << res[i].index << " ";
        
        cout << "\n";
    }
    return 0;
}