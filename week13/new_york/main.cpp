#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <unordered_map>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
    {
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> temps;
        unordered_map<int,int> count;// used to keep track of what should be cleaned from the heap, everything at 0 will be popped
        for (int i = 0; i < n; i++)
        {
            int t;
            cin >> t;
            if (count.find(t) == count.end())
                count.insert(pair<int, int>(t, 0));
            
            temps.push_back(t);
        }

        vector<vector<int>> g(n);
        for (int i = 0; i < n - 1; i++)
        {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
        }

        priority_queue<int> maxheap;
        priority_queue<int, vector<int>, greater<int>> minheap;
        vector<int> path;
        set<int> stamps;
        vector<bool> visited(n, false);
        vector<int> deque;
        
        deque.push_back(0);
        while (deque.size() > 0)
        {
            int v = deque.back();
            deque.pop_back();

            if (visited[v]) //if already visited
            {            
                int curr_temp = temps[v];

                if (path.size() > m) // if window > m increment element at pos size - m - 1
                {
                    count.find(temps[path[path.size() - m - 1]])->second += 1;
                    minheap.push(temps[path[path.size() - m - 1]]);
                    maxheap.push(temps[path[path.size() - m - 1]]);
                }

                //remove first element
                count.find(curr_temp)->second -= 1;
                path.pop_back(); // pop from path -> only if already visited

                continue;
            }
            
            visited[v] = true;
            deque.push_back(v);
            path.push_back(v);

            for (int adj : g[v])
                deque.push_back(adj);
    
            int curr_temps = temps[v];
            minheap.push(curr_temps);
            maxheap.push(curr_temps);
            count.find(curr_temps)->second += 1;

            if (path.size() > m) // if window > m delete first element from count
                count.find(temps[path[path.size() - m - 1]])->second -= 1;

            // clean everything not considered
            while (count.find(minheap.top())->second == 0)
                minheap.pop();
            
            while (count.find(maxheap.top())->second == 0)
                maxheap.pop();


            if (path.size() >= m)
            {
                int maxt = maxheap.top();
                int mint = minheap.top();
                if (maxt - mint <= k)
                    stamps.insert(path[path.size() - m]);
            }
        }

        if (stamps.size() == 0)
            cout << "Abort mission";
        else
            for (int elem : stamps)
                cout << elem << " ";
            
        cout << "\n";
        
    }
    return 0;
}