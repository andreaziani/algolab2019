#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
void back_track(int left, int right, VVI &s, VVI &assignment, const VI &planks)
{
    if (left >= right)
    {
        VI tuple(4, 0);
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < assignment[i].size(); ++j)
                tuple[i] += planks[assignment[i][j]];
        }
        s.push_back(tuple);
        return;
    }
    for (int i = 0; i < 4; ++i)
    {
        assignment[i].push_back(left); // Try to put id-th plank to i-th set 
        back_track(left + 1, right, s, assignment, planks); // Recurse 
        assignment[i].pop_back(); // Remove id-th plank from i-th set
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int n, sum = 0;
        long result = 0;
        cin >> n;
        VI planks(n);
        for (int i = 0; i < n; ++i)        
            cin >> planks[i];
       
        for (int i = 0; i < n; ++i)
            sum += planks[i];
        
        if (sum % 4 != 0) // impossible to build a square from these planks
        {
            cout << 0 << "\n";
            continue;
        }

        VVI s1, assignment(4);
        // Generate all 4-tuple for the first half of the set.
        back_track(0, n / 2, s1, assignment, planks);
        VVI s2, assignment2(4);
        // Generate all 4-tuple for the second half of the set.
        back_track(n / 2, n, s2, assignment2, planks);
        sort(s2.begin(), s2.end());
        for (int i = 0; i < s1.size(); ++i)
        {
            vector<int> member = s1[i];
            for(int j = 0; j < 4; ++j)
                member[j] = sum / 4 - member[j];
            pair<VVI::iterator, VVI::iterator> bounds;
            bounds = equal_range(s2.begin(), s2.end(), member); 
            long counter = distance(bounds.first, bounds.second); // the number of element between our bounds = n times we can build this side
            result += counter;
        }
        cout << result / 24 << "\n";
    }
}