#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

int main()
{
    ios_base::sync_with_stdio(false);
    while (true)
    {
        int n;
        cin >> n;
        if (n == 0)
            break;

        vector<K::Point_2> pts;
        for (int i = 0; i < n; ++i)
        {
            int x, y;
            cin >> x >> y;
            pts.emplace_back(x, y);
        }

        Triangulation t;
        t.insert(pts.begin(), pts.end());

        typedef pair<K::FT, Triangulation::Face_handle> face_handle; // {dist, Face_handle}
        priority_queue<face_handle, vector<face_handle>, less<face_handle>> q; // min heap
        
        // for each face initilize info and push neighbor dist in min heap
        for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f)
        {
            if (t.is_infinite(f))
            {
                f->info() = -1;
            }
            else
            {
                f->info() = 0;
                continue;
            }

            for (int i = 0; i < 3; ++i)
            {
                auto neighbor = f->neighbor(i);
                if (t.is_infinite(neighbor))
                    continue;
                else
                    q.push({t.segment(f, i).squared_length(), neighbor});
            }
        }

        // Sort of DFS for shortest distance for each face until reach an infinite face
        // for each face handle in the min heap, check the dist for each neighbor face and keep the min until reach infinite face
        while (!q.empty())
        {
            auto p = q.top();
            q.pop();
            auto face = p.second;
            auto dist = p.first;

            if (face->info() != 0) // if already visited continue
                continue;

            face->info() = dist; // visit face
            for (int i = 0; i < 3; ++i)
            {
                auto neighbor = face->neighbor(i);
                if (t.is_infinite(neighbor) || neighbor->info() != 0) // if infinite or already visited
                    continue;
                q.push({min(t.segment(face, i).squared_length(), dist), neighbor}); // keep min distance of path
            }
        }

        int m;
        cin >> m;
        for (int i = 0; i < m; ++i)
        {
            double x, y;
            long d;
            cin >> x >> y >> d;
            K::Point_2 p(x, y);
            auto v = t.nearest_vertex(p);
            if (CGAL::squared_distance(p, v->point()) < d)
            {
                cout << "n";
                continue;
            }

            auto f = t.locate(p);
            if(t.is_infinite(f)){
                cout << "y";
            } else if (f->info() < d * 4.0)
                cout << "n";
            else
                cout << "y";
        }
        cout << "\n";
    }
    return 0;
}
