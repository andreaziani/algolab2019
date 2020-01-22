#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

/* O(n^2)
int evenPairs(vector<int> seq, int n){
    int res = 0;
    vector<int> pref_sum(n, 0);
    pref_sum[0] = seq[0];
    for(int i = 1; i < n; i++)
        pref_sum[i] = (seq[i] + pref_sum[i-1]);
    for(int j = 0; j < n; j++){
        if(pref_sum[j] % 2 == 0) res++;
        for(int i = 1; i <= j; i++){
            if((pref_sum[j] - pref_sum[i - 1]) % 2 == 0) res++;
        }
    }
    return res;
}
*/

int binomialCoeff(int n, int k)  
{  
    if(k > n) return 0;
    if (k == 0 || k == n)  return 1;  
    int ans = 1;
    for(int i = 0; i < k; i++){
        ans *= (n - i);
        ans /= (i + 1);
    }
    return ans;
}  

// With binomial coefficient we can calculate the result in O(n)
int evenPairs(vector<int> seq, int n){
    int res = 0;
    int even = 0;
    int odd = 0;
    vector<int> pref_sum(n, 0);
    
    pref_sum[0] = seq[0];
    if(pref_sum[0] % 2 == 0) even++;
    else odd++;
    for(int i = 1; i < n; i++){
        pref_sum[i] = (seq[i] + pref_sum[i-1]);
        if(pref_sum[i] % 2 == 0) even++;
        else odd++;
    }
    return binomialCoeff(even, 2) + binomialCoeff(odd, 2) + even;
}

int main(){ 
    int testcases;
    cin >> testcases;
    vector<vector<int> > sequences;
    
    // read input
    for(int i = 0; i < testcases; i++){
        int length;
        cin >> length;
        vector<int> seq(length);
        for(int i = 0; i < length; i++)
            cin >> seq[i];
        sequences.push_back(seq);
    }

    for(int i = 0; i < sequences.size(); i++){
        cout << evenPairs(sequences[i], sequences[i].size()) << endl;
    }
    return 0; 
}