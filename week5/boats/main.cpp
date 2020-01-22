#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

class Boat{
    public: 
        int l, ring, min;
    Boat(int length, int ring){
        this->l = length;
        this->ring = ring;
        this->min = ring;
    }

    bool operator< (const Boat &d2) const{ //defined less then as contrary to have a min heap
        if(this->min == d2.min)
            return this->ring > d2.ring;
        return this->min > d2.min;
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t-- > 0){
        int n; cin >> n;
        priority_queue<Boat> boats; //min heap
        for(int i = 0; i < n; i++){
            int l, p; cin >> l >> p;
            boats.push(Boat(l, p));
        }

        int last = boats.top().ring; boats.pop(); //at the beginning the first boat is entirely on the left
        int count = 1;
        
        while(!boats.empty()){
            Boat b = boats.top(); boats.pop();
            if(b.min - b.l >= last){ // the last boat and the current don't touch
                count++;
                last = b.min;
            } else if(last <= b.ring){ // if they touch, update the min of the current boat 
                b.min = last + b.l;
                if(b.min >= b.ring) boats.push(b);
            }
        }

        cout << count << "\n";

    }
    return 0;
}