#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

class Bomb{
    public:
        int i, t;
        int p1, p2;
        bool active;
    public: Bomb(int index, int timer){
        this->i = index;
        this->t = timer;
        this->p1 = -1;
        this->p2 = -1;
        this->active = true;
    }

    public: Bomb(int index, int timer, int p1, int p2){
        this->i = index;
        this->t = timer;
        this->p1 = p1;
        this->p2 = p2;
        this->active = true;
    }

    bool operator< (const Bomb &d2) const{ 
        if(this->t == d2.t)
            return this->i > d2.i;
        return this->t < d2.t;
    }
};


bool deactivateChilds(vector<Bomb> &bombs, int i, int &timer){
    while(bombs[i].active){
        if(bombs[i].p1 == -1 || bombs[i].p2 == -1){
            if(bombs[i].t > timer){
                bombs[i].active = false;
                timer++;
                //cout << "deactivate " << bombs[i].t << " no childs" <<endl;
                return true;
            }
            return false;
        } else {
            int p1 = bombs[i].p1;
            int p2 = bombs[i].p2;
            if(bombs[p1].t > bombs[p2].t){ // deactivate always the smallest timer first
                int tmp = p1;
                p1 = p2;
                p2 = tmp;
            }
            if(deactivateChilds(bombs, p1, timer) && deactivateChilds(bombs, p2, timer)){
                if(bombs[i].t > timer){
                    bombs[i].active = false;
                    timer++;
                    //cout << "Func deactivate " << bombs[i].t << endl;
                    return true;
                }
            } 
            return false;
            
        }
    }
    return true; 
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;

    while(t-- > 0){
        int n; cin >> n;
        vector<Bomb> bombs(n, Bomb(0, 0));
        vector<Bomb> sortBomb(n, Bomb(0,0));
        for(int i = 0; i < n; i++){ // create tree
            int time; cin >> time;
            if(i <= (n-3)/2)
                sortBomb[i] = bombs[i] = Bomb(i, time, i*2 + 1, i * 2 + 2);
            else sortBomb[i] = bombs[i] = Bomb(i, time);
        }

        sort(sortBomb.begin(), sortBomb.end()); //O(nlogn)
        int timer = 0;
        for(int i = 0; i < n; i++){ //O(N^2 in the worst case scenario: smallest value on the top)
            //cout << bombs[i].t << endl;
            if(!bombs[sortBomb[i].i].active)
                continue;
            int p1 = sortBomb[i].p1;
            int p2 = sortBomb[i].p2;
            if(p1 != -1 && p2 != -1){
                if(bombs[p1].active || bombs[p2].active){ // if I'am standing on some balls deactivate these first
                    if(bombs[p1].t > bombs[p2].t){ // deactivate always the smallest timer first
                        int tmp = p1;
                        p1 = p2;
                        p2 = tmp;
                    }
                    if(!deactivateChilds(bombs, p1, timer) || !deactivateChilds(bombs, p2, timer)){ 
                        cout << "no\n";
                        break;
                    }
                }
            }
            if(bombs[sortBomb[i].i].t > timer){
                    bombs[sortBomb[i].i].active = false;
                    //cout << "deactivate " << bombs[sortBomb[i].i].t << endl;
                    timer++;
            } else{
                cout << "no\n";
                break;
            }  
        }
        if(timer == n) cout << "yes\n";
    }
    return 0;
}