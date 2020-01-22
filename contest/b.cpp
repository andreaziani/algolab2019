#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

bool comp(int a, int b)
{
    return a > b;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    vector<int> sweets(n);
    for(int i = 0; i < n; i++)
        cin >> sweets[i];

    sort(sweets.begin(), sweets.end(), comp);

    vector<int> psum;
    int count = 0;
    int t_m = 0;
    for(int i = n - 1; i >= 0; i--){
        count += sweets[i];
        t_m++;
        psum.push_back(count);
        if(t_m == m){
            count = 0;
            t_m = 0;
        }
    }
    reverse(psum.begin(), psum.end());

    /*for(auto e : psum)
        cout << e << " ";
    */
    for(int k = 1; k <= n; k++)
    {
        int res = 0;
        int blocks = k / m;
        int partial = k % m;
        if(partial == 0){
            int day = 1;
            for(int i = psum.size() - (blocks * m); i < psum.size(); i += m, day++){
                res += day * psum[i];
            }
        } else{
            int day = 1;
            for(int i = psum.size() - (blocks * m); i < psum.size(); i += m, day++){
                res += day * psum[i];
            }
            res += psum[psum.size() - 1 - partial];
        }
        cout << count << " ";
    }

    cout << "\n";

    return 0;
}