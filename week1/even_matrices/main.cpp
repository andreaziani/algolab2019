#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;


int evenMatrix(vector<vector<int> > matrix, int n){
    vector<vector<int> > pref_sum(n, vector<int>(n+1, 0));
    for(int y = 0; y < n; y++) {
        int partial = 0;
        for(int x = 0; x < n; x++) {
            partial += matrix[y][x];
            pref_sum[y][x+1] = partial;
        }
    }
    int res = 0;
    for(int x1 = 0; x1 < n; x1++) { 
        for(int x2 = x1; x2 < n; x2++) {
            int even = 0, odd = 0, tmp;
            for(int y = 0; y < n; y++) {
                if((pref_sum[y][x2 + 1] - pref_sum[y][x1]) % 2 != 0) {
                    tmp = even;
                    even = odd;
                    odd = tmp+1;
                } else {
                    even++;
                }
                //cout << "Even: " << even << " Odd: " << odd << endl;
                res += even;
                //cout << "Res " << res << endl; 
            }
            //cout << "turn" << endl;
        }
        //cout << "Ext turn" << endl;
    }
    return res;
}

int main(){ 
    //read input
    int t;
    cin >> t;
    vector<vector<vector<int> > > matrices;
    for(int i = 0; i < t; i++){
        int n; 
        cin >> n;
        vector<vector<int> > matrix(n, vector<int>(n));
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++)
                cin >> matrix[j][k];
        }
        matrices.push_back(matrix);
    }

    for(int i = 0; i < t; i++)
        cout << evenMatrix(matrices[i], matrices[i][0].size()) << endl;

    return 0; 
}
