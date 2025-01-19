#include <bits/stdc++.h>
using namespace std;

#define endl '\n'


void all_permutation(vector<int> &cities, int n){
    do{
        for(int i = 0; i < n; i++){
            cout << cities[i] << " ";
        }
        cout << endl;
    }while(next_permutation(cities.begin(), cities.end()));
}

int main(){

    int n;
    cin >> n;

    if(n == 0){
        cout << "forneca um n >= 1" << endl;
        return 1;
    }
    
    vector<int>cities(n);
    for(int i = 0; i < n; i++)
        cities[i] = i + 1;
    
    clock_t t = clock();
    all_permutation(cities, n);
    t = clock() - t;
    
    cout << endl;

    cout << "Tempo de execucao: " << ((float)t)/CLOCKS_PER_SEC << endl;
    return 0;
}