#include <bits/stdc++.h>
#include "../grafos/grafos2b.h"

using namespace std;

#define endl '\n'

void all_permutation(vector<int> &cities, vector<vector<int>> &graph, int n){
    do{
        int entrou = false;
        for(int i = 0; i < n - 1; i++){
            int x = cities[i];
            int y = cities[i + 1];

            if(graph[x][y] == 0){
                entrou = true;
                break;
            }
        }

        if(entrou) continue;
        
        int soma = 0;
        for(int i = 0; i < n; i++){
            if(graph[cities[i]][cities[i + 1]]) 
                soma += graph[cities[i]][cities[i + 1]];
            
            cout << cities[i] + 1 << " ";
        }
        cout << "peso: " << soma << endl;
    }while(next_permutation(cities.begin(), cities.end()));
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    
    int v, a;
    cin >> v >> a;

    vector<int>cities(v);
    for(int i = 0; i < v; i++)
        cities[i] = i;

    vector<vector<int>>graph(v, vector<int>(v, 0));

    ler_grafico_listaMatrizAdj(graph, a);
    imprimir_grafico_listaMatrizAdj(graph, v);

    clock_t t = clock();
    all_permutation(cities, graph, v);
    t = clock() - t;

    cout << "Tempo de execucao: " << ((float)t)/CLOCKS_PER_SEC << endl;
    return 0;
}