#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

struct boat{
    int length, ring;
    boat(int a, int b){
        length = a;
        ring = b;
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n; cin >> n;
        vector<boat> boats;

        for(int i = 0; i < n; i++){
            int length, ring;
            cin >> length >> ring;
            boats.push_back(boat(length, ring));
        }

        sort(boats.begin(), boats.end(), [](boat &b1, boat &b2) -> bool{ return b1.ring < b2.ring;});
        
        int count = 1;
        int last1 = boats[0].ring; //left sure
        int last2 = INT_MAX; // left candidate

        for(int i = 1; i < n; i++){
            if(boats[i].ring >= last2){
                count++;
                last1 = last2;
                last2 = max(last1 + boats[i].length, boats[i].ring);
            } else {
                int tmp = max(last1 + boats[i].length, boats[i].ring);
                last2 = min(last2, tmp);
            }
        }

        if(n > 1) count++; // add last
        cout << count << "\n";
        
    }


    return 0;
}