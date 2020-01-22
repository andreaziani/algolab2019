#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
bool comparePair(const pair<int, int> &a, const pair<int, int> &b)
{
    return a.second < b.second;
}

bool between(int a, int b, int c)
{
    if ((a <= c && (a <= b && b <= c)) || (c < a && (b <= c || b >= a)))
        return true;
    return false;
}

bool intersect(int a1, int a2, int b1, int b2)
{
    if (between(a1, b1, a2) || between(a1, b2, a2) || between(b1, a1, b2) || between(b1, a2, b2))
        return true;
    return false;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t-- > 0)
    {
        int n, m;
        cin >> n >> m;

        vector<pair<int, int>> jedi(n);
        vector<pair<int, bool>> sums;

        for (int i = 0; i < n; ++i)
        {
            int begin, end;
            cin >> begin >> end;
            jedi[i] = {begin, end};
            sums.push_back({begin, true});
            sums.push_back({end + 1, false});
        }

        sort(sums.begin(), sums.end());
        sort(jedi.begin(), jedi.end(), comparePair);

        int partial_sum = 0;
        int min = 0;
        int pos = 1;
        for (int i = 0; i < sums.size(); ++i) // find minimum intersection point
        {
            if (sums[i].second)
            {
                partial_sum++;
            }
            else
            {
                partial_sum--;
                if (partial_sum < min)
                {
                    min = partial_sum;
                    pos = sums[i].first;
                }
            }
        }

        vector<int> jedi_to_choose;
        for (int i = 0; i < n; i++) // find overlapped jedis for this point
        {
            if (between(jedi[i].first, pos, jedi[i].second))
            {
                jedi_to_choose.push_back(i);
            }
        }

        int max_jedi = 0;
        // try to take one overlapped jedi each time
        for (int curr_jedi_ind : jedi_to_choose) 
        {
            pair<int, int> curr_jedi = jedi[curr_jedi_ind];
            int blocked_until = curr_jedi.second;
            int curr_max = 1;
            for (int i = curr_jedi_ind + 1; i < n; ++i)
            {
                if (!intersect(curr_jedi.first, blocked_until, jedi[i].first, jedi[i].second))
                {
                    curr_max++;
                    blocked_until = jedi[i].second;
                }
            }
            for (int i = 0; i < curr_jedi_ind; ++i)
            {
                if (!intersect(curr_jedi.first, blocked_until, jedi[i].first, jedi[i].second))
                {
                    curr_max++;
                    blocked_until = jedi[i].second;
                }
            }
            if (curr_max > max_jedi)
            {
                max_jedi = curr_max;
            }
        }

        int curr_max = 1;
        int found_ind = 0;
        // Do not take overlapped jedis
        // try to find the first jedi which ends after the minumum intersect point
        while (found_ind < n && jedi[found_ind].second <= pos) 
            found_ind++;
        
        if (found_ind == n)
            found_ind = 0;
        
        while (found_ind < n && between(jedi[found_ind].first, pos, jedi[found_ind].second))
            found_ind++;
        
        if (found_ind == n)
            found_ind = 0;
    
        while (found_ind < n && between(jedi[found_ind].first, pos, jedi[found_ind].second))
            found_ind++;
        
        if (found_ind == n)
            found_ind = 0;
        
        
        pair<int, int> curr_jedi = jedi[found_ind];
        int blocked_until = curr_jedi.second;
        for (int i = found_ind; i < n; ++i)
        {
            if (!intersect(curr_jedi.first, blocked_until, jedi[i].first, jedi[i].second))
            {
                curr_max++;
                blocked_until = jedi[i].second;
            }
        }
        for (int i = 0; i < found_ind; ++i)
        {
            if (!intersect(curr_jedi.first, blocked_until, jedi[i].first, jedi[i].second))
            {
                curr_max++;
                blocked_until = jedi[i].second;
            }
        }

        if (curr_max > max_jedi)
            max_jedi = curr_max;

        cout << max_jedi << endl;
    }
    return 0;
}