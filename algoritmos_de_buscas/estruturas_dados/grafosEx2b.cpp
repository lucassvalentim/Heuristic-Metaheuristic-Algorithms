#include "bits/stdc++.h"
using namespace std;

#define ll long long

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

void ler_grafico_listaAdj(vector<vector<pair<int,int>>>&graph, int a){
    int v1, v2, p;
    for(int i = 0; i < a; i++){
        cin >> v1 >> v2 >> p; v1--;v2--;

        graph[v1].push_back({v2, p});
        graph[v2].push_back({v1, p});
    }
}

void imprimir_grafico_listaAdj(vector<vector<pair<int,int>>>&graph, int v){
    cout << "Grafico da lista de adjacencia" << endl;

    for(int i = 0; i < v; i++){
        cout << i + 1 << " -> ";
        for(auto edge : graph[i]){
            cout << "{"<< edge.first + 1 <<", " << edge.second << "}" << " ";
        }
        cout << endl;
    }
}