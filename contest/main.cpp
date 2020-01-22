#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    while(t-- > 0){
        int n; 
        cin >>  n;
        vector<int> a(n), b(n);
        bool possible = true;
        for(int i = 0; i < n; i++)
            cin >> a[i];
        for(int i = 0; i < n; i++)
            cin >> b[i];
        
        int k = 0;
        int l = 0;
        for(int i = 0; i < n; i++){
            if(a[i] != b[i]){
                l = i;
                k = b[i] - a[i];
                if(k < 0) possible = false;
                break;
            }
        }
        if(possible){
            for(int i = l; i < n; i++){
                int curr = a[i] + k;
                if(curr == b[i]) continue;
                else{
                    if(b[i] - a[i] == 0)
                        k = 0;
                    else{
                        possible = false;
                        break;
                    }
                }
            }
        }
        
        if(possible) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}