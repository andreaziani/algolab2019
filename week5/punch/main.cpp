#include<iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int MAXN = 100 + 2;
const int MAXK = 10 * 1000 + 2;

#define TAKE 1
#define NOT_TAKE 0

typedef pair<int,int> pii;

int v[MAXN];
int price[MAXN];
pii impossible = {-2,-2};
pii NOT_CALC = {-1,-1};

pii dp(int k, int n, bool state, vector<vector<vector<pii>>> &memo)
{
    if(k <= 0)
        return {0,0};
    if(n < 0)
        return impossible;
    
    if(memo[k][n][state] != NOT_CALC)
        return memo[k][n][state];

    memo[k][n][state] = impossible;    
    
    //use none
    if(dp(k, n-1, NOT_TAKE, memo)!=impossible)
        memo[k][n][state] = dp(k, n-1, NOT_TAKE, memo);
        
    //use one
    if(dp(k - v[n], n, TAKE, memo) != impossible)
    {
        pii t = dp(k - v[n], n, TAKE, memo);
        t.first += price[n]; // add cost
        if(!state)
            t.second--; // decrease number of state because not calc is -1
        if(memo[k][n][state] == impossible || memo[k][n][state] > t)
            memo[k][n][state] = t;
    }
    return memo[k][n][state];
}


int main()
{
    int t;
    cin >> t;
    while(t--){
        int n,k;
        cin >> n>> k;
        vector<vector<vector<pii>>> memo(MAXK, vector<vector<pair<int,int>>>(MAXN, vector<pii>(2, {-1, -1})));

        for(int i = 0 ;i < n; i++)
            cin >> price[i] >> v[i];
 
        pii t = dp(k,n-1, NOT_TAKE, memo);
        cout << t.first <<' '<< -t.second <<endl; // state is neg
    }
    return 0;
}
    

