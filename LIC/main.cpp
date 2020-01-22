#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;
int main(){
    std::ios_base::sync_with_stdio(false);
    int n; cin >> n;
    vector<int> array(n);
    for(int i = 0; i < n; i++){
        cin >> array[i];
    }
    
    //Longest increasing subsequence O(n^2)
    vector<int> memo(n, 1);
    for(int j = 1; j < n; j++){
        for(int i = 0; i < j; i++){
            if(array[j] > array[i]){
                memo[j] = max(memo[j], memo[i] + 1);
            }
        }
    }

    //Longest increasing subsequence O(nlogn)
    vector<int> tail;
    tail.push_back(array[0]);
    for(int i = 1; i < n; i++){
        if(array[i] < tail[0]){
            tail[0] = array[i]; // smallest among all candidates of active list -> start a new list

        } else if(array[i] > tail[tail.size() - 1]){
            tail.push_back(array[i]); // largest among all candidates of active list -> push in active list
        }
        else { // value in between. find the largest element smaller than ai and update element
            auto it = lower_bound(tail.begin(), tail.end(), array[i]); // log(n) find the element
            *it = array[i];
        }
    }

    //this are lengths
    
    int max = 1;
    for(int i : memo)
        max = std::max(max, i);
    cout << "first approach " << max << endl;

    //this are values, length is the n of elements
    cout << "second approach " << tail.size() << endl;
    return 0;
}