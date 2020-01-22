#include <climits>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<int> pref_sum_A(n), pref_sum_B(n); // prefix sums

        int a;
        cin >> a;
        pref_sum_A[0] = a;
        for (int i = 1; i < n; i++)
        {
            cin >> a;
            pref_sum_A[i] = pref_sum_A[i - 1] + a;
        }

        int b;
        cin >> b;
        pref_sum_B[0] = b;
        for (int i = 1; i < n; i++)
        {
            cin >> b;
            pref_sum_B[i] = pref_sum_B[i - 1] + b;
        }

        vector<vector<int>> memo(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= n; i++) // startA
        {
            for (int j = 1; j <= n; j++) // startB
            {
                int min_cost = INT_MAX;
                {
                    int j_ = j - 1;
                    int pref_sum_B_ = pref_sum_B[j - 1]; 
                    if (j_ > 0)
                    {
                        pref_sum_B_ -= pref_sum_B[j_ - 1];
                    }
                    int costB = pref_sum_B_ - (j - j_); // cost B[j - 1]
                    for (int i_ = 0; i_ < i; i_++) // endA
                    {
                        // Make sure, not one stack is empty at the end (without the other being empty)
                        if ((i_ == 0 && j_ == 0) || (i_ != 0 && j_ != 0))
                        {
                            int pref_sum_A_ = pref_sum_A[i - 1];
                            if (i_ > 0)
                            {
                                pref_sum_A_ -= pref_sum_A[i_ - 1];
                            }
                            int cost = memo[i_][j_] + (pref_sum_A_ - (i - i_)) * costB;
                            min_cost = min(min_cost, cost);
                        }
                    }
                }
                {
                    int i_ = i - 1;
                    int pref_sum_A_ = pref_sum_A[i - 1];
                    if (i_ > 0)
                    {
                        pref_sum_A_ -= pref_sum_A[i_ - 1];
                    }
                    int costA = pref_sum_A_ - (i - i_);
                    for (int j_ = 0; j_ < j; j_++) //endB
                    {
                        if ((i_ == 0 && j_ == 0) || (i_ != 0 && j_ != 0))
                        {
                            int pref_sum_B_ = pref_sum_B[j - 1];
                            if (j_ > 0)
                            {
                                pref_sum_B_ -= pref_sum_B[j_ - 1];
                            }
                            int cost = memo[i_][j_] + (pref_sum_B_ - (j - j_)) * costA;
                            min_cost = min(min_cost, cost);
                        }
                    }
                }
                memo[i][j] = min_cost;
            }
        }
        cout << memo[n][n] << "\n";
    }
    return 0;
}
