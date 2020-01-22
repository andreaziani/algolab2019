#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int UP_TIME = 1;
const int DOWN_TIME = 2;

bool possible(int k, std::vector<int> &w, std::vector<int> &s)
{
    int n = s.size(), m = w.size();
    int i = n - 1, j = m - 1;
    // every friend takes k boxes
    while (i >= 0 && j >= 0)
    {
        if (s[i] >= w[j])
        {
            i--;
            j -= k;
        }
        else
        {
            return false;
        }
    }
    return (j < 0);
}

using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--)
    {
        int n, m;
        cin >> n >> m;
        vector<int> s(n), w(m);
        for (int i = 0; i < n; ++i)
            cin >> s[i];
     
        for (int i = 0; i < m; ++i)
        {
            cin >> w[i];
        }
        
        sort(s.begin(), s.end());
        sort(w.begin(), w.end());

        if (s[n - 1] < w[m - 1])
        {
            cout << "impossible\n";
            continue;
        }

        int left = 1, right = m; //max m turns one for each box, min 1 turn

        //binary search on rounds
        while (left < right)
        {
            int middle = (left + right) / 2;
            if (!possible(middle, w, s))
                left = middle + 1;
            else
                right = middle;
        }

        cout << left * (UP_TIME + DOWN_TIME) - UP_TIME << "\n";
    }
    return 0;
}
