#include <stdlib.h>
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int main(){ 
    int t;
    cin >> t;
    vector<vector<int> > sequences;
    //read input 
    for(int i = 0; i < t; i++){
        int length;
        cin >> length;
        vector<int> seq(length); 
        for(int j = 0; j < length; j++){
            cin >> seq[j];
        }
        sequences.push_back(seq);
    } 

    for(int i = 0; i < t; i++){
        cout << accumulate(sequences[i].begin(), sequences[i].end(), 0) << endl;
    }
    return 0; 
}