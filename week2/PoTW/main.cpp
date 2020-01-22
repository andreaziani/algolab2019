#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
pair<int,int> deckOfCards(vector<int> v, int k){
    int i = 0, j = 0;
    int min = INT_MAX, minI = 0, minJ = 0;
    int sum = v[0];
    while(i <= j && j < v.size()){
        //cout << i << " " << j << " S: " << sum <<  " M: " << min <<endl;
        if(abs(k - sum) < min) {
            min = abs(k - sum);
            minI = i;
            minJ = j;
        }

        if(sum < k){
           j++;
           sum += v[j];
        } else if(sum > k){
            sum -= v[i]; 
            if(i == j){
                j++;
                sum += v[j];
            }
            i++;
        } else{
            break;
        }
    }
    return make_pair(minI, minJ);
}
int main(){ 
    int t;
    cin >> t;
    vector<pair<int,int> > res(t);
    for(int i = 0; i < t; i++){
        int n, k;
        cin >> n >> k;
        vector<int> v(n);
        for(int j = 0; j < n; j++){
            cin >> v[j];
        }
        res[i] = deckOfCards(v, k);
    }
    for(int i = 0; i < t; i++) cout << res[i].first << " " << res[i].second << endl;
    return 0; 
}