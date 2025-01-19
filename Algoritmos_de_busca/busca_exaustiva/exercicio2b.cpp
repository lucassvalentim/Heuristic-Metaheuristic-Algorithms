#include <bits/stdc++.h>
// #include "../grafos/grafos2b.h" 

using namespace std;

#define endl '\n'

void ler_grafico_listaMatrizAdj(vector<vector<int>>&graph, int a){
    int v1, v2, p;
    for(int i = 0; i < a; i++){
        cin >> v1 >> v2 >> p; v1--;v2--;

        graph[v1][v2] = p;
        graph[v2][v1] = p;
    }
}

void imprimir_grafico_listaMatrizAdj(vector<vector<int>>&graph, int v){
    cout << "Matriz de adjacencia" << endl;
    int v1, v2, p;
    for(int i = 0; i < v; i++){
        for(int j = 0; j < v; j++){
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Grafico da Matriz de adjacencia" << endl;
    for(int i = 0; i < v; i++){
        cout << i + 1 << " -> ";
        for(int j = 0; j < v; j++){
            if(graph[i][j] != 0)
                cout << "{" << j + 1 << ", " << graph[i][j] << "}"<< " ";
        }
        cout << endl;
    }
}

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