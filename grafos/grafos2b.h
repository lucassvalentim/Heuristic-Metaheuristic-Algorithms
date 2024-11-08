#ifndef __GRAFOS__H
#define __GRAFOS__H

#include "bits/stdc++.h"
using namespace std;

#define ll long long

void ler_grafico_listaMatrizAdj(vector<vector<int>>&graph, int a);
void imprimir_grafico_listaMatrizAdj(vector<vector<int>>&graph, int v);
void ler_grafico_listaAdj(vector<vector<pair<int,int>>>&graph, int a);
void imprimir_grafico_listaAdj(vector<vector<pair<int,int>>>&graph, int v);

#endif