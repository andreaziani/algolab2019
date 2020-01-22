#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>
using namespace std;
class Specie{
    public: 
        string name;
        int age;
        int parent;
        int prev_query;
        int prev_age;

    Specie(string name, int age){
        this->name = name;
        this->age = age;
        this->parent = -1;
        this->prev_query = -1;
        this->prev_age = -1;
    }
    Specie(){}
};

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n, q;
        cin >> n >> q;
        vector<Specie> tree;
        map<string, int> s_index;
        for(int i = 0; i < n; i++){
            string name; 
            int age;
            cin >> name >> age;
            tree.push_back(Specie(name, age));
            s_index[name] = i;
        }

        for(int i = 0; i < n - 1; i++){
            string curr, fat;
            cin >> curr >> fat;
            tree[s_index[curr]].parent = s_index[fat];
        }

        for(int i = 0; i < q; i++){
            string target;
            int max_age;
            cin >> target >> max_age;
            int target_index = s_index[target];

            int curr = target_index;

            while (tree[curr].parent != -1 && tree[tree[curr].parent].age <= max_age)
            {
                if (tree[curr].prev_age != -1 && tree[curr].prev_age <= max_age)
                {
                    curr = tree[curr].prev_query;
                }
                else
                {
                    curr = tree[curr].parent;
                }
            }
            
            tree[target_index].prev_age = max_age;
            tree[target_index].prev_query = curr;
            std::cout << tree[curr].name << " ";
        }
        cout << "\n";

    }
    return 0;
}