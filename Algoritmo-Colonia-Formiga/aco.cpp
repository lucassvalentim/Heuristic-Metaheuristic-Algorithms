#include <bits/stdc++.h>
using namespace std;

// Define um alias para maior clareza no tipo de dado
#define ll long long
#define endl '\n'
const double ERROR_THRESHOLD = 1e-2;

// Gerador de números aleatórios
random_device rd;
mt19937 generator(rd());

// Lê as coordenadas dos vértices e calcula a matriz de adjacência
vector<vector<double>> readGraph(vector<vector<double>> &graph, int vertexCount) {
    vector<pair<ll, ll>> coordinates(vertexCount);

    // Leitura das coordenadas
    ll index, x, y;
    for (int i = 0; i < vertexCount; i++) {
        cin >> index >> x >> y;
        index--; // Ajusta o índice para zero-based
        coordinates[index] = make_pair(x, y);
    }

    // Preenche a matriz de adjacência com as distâncias euclidianas
    for (int i = 0; i < vertexCount; i++) {
        ll x1 = coordinates[i].first;
        ll y1 = coordinates[i].second;
        for (int j = 0; j < vertexCount; j++) {
            ll x2 = coordinates[j].first;
            ll y2 = coordinates[j].second;
            graph[i][j] = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        }
    }
    return graph;
}

void inicializa_feromonios(vector<vector<double>> &feromonios, double taxa, int vertexCount){
    for(int i = 0; i < vertexCount; i++){
        for(int j = 0; j < vertexCount; j++){
            if(i != j) 
                feromonios[i][j] = taxa;
        }
    }
}

void inicializa_heuristica(vector<vector<double>> &heuristica, vector<vector<double>> &graph, int vertexCount){
    for(int i = 0; i < vertexCount; i++){
        for(int j = 0; j < vertexCount; j++){
            if(i != j)
                heuristica[i][j] = 1 / graph[i][j];
        }
    }
}

vector<pair<int, double>> probabilidade(vector<vector<double>> &feromonios, vector<vector<double>> &heuristica, vector<bool> visitados, int i, double alpha, double beta, int vertexCount){
    vector<pair<int, double>> p;

    double somatorio = 0;
    for(int j = 0; j < vertexCount; j++){
        if(!visitados[j])
            somatorio += ((pow(feromonios[i][j], alpha)) * (pow(heuristica[i][j], beta)));
    }

    for(int j = 0; j < vertexCount; j++){
        if(j != i && !visitados[j]){
            double taxa = ((pow(feromonios[i][j], alpha)) * (pow(heuristica[i][j], beta))) / somatorio;
            p.push_back({j, taxa});
        }
    }

    if(p.size() == 0)
        p.push_back({i, 1});

    return p;
}

int metodo_roleta(vector<pair<int, double>> &probabilidades){
    vector<pair<int, double>> roleta(probabilidades.size());
    
    roleta[0].first = probabilidades[0].first;
    roleta[0].second = probabilidades[0].second;
    for(int i = 1; i < probabilidades.size(); i++){
        roleta[i].first = probabilidades[i].first;
        roleta[i].second = roleta[i - 1].second + probabilidades[i].second;
    }

    uniform_real_distribution<double> dis(0, 1);
    
    double random = dis(generator);
    int j = 0;
    for(auto x : roleta){
        if(random <= x.second){
            j = x.first;
            break;
        }
    }

    return j;
}

void ACO(vector<vector<double>> &graph, int vertexCount){
    int Kformigas = vertexCount;

    const int maxIteracoes = 1;
    int inter = 0;

    vector<vector<double>> feromonios(vertexCount, vector<double>(vertexCount));
    vector<vector<double>> heuristica(vertexCount, vector<double>(vertexCount));
    inicializa_feromonios(feromonios, 0.1, vertexCount);
    inicializa_heuristica(heuristica, graph, vertexCount);

    while(inter < maxIteracoes){
        vector<vector<int>> caminhos_formigas;
        for(int formiga = 0; formiga < Kformigas; formiga++){
            set<int> s;
            for(int i = 0; i < vertexCount; i++){
                if(i != formiga) {
                    s.insert(i);
                }
            }

            vector<bool> visitados(vertexCount, false);
            int current = formiga;
            visitados[current] = true;

            vector<int> caminho_corrent;
            caminho_corrent.push_back(current);

            while(!s.empty()){
                double alpha = 1;
                double beta = 1;

                vector<pair<int, double>> p = probabilidade(feromonios, heuristica, visitados, current, alpha, beta, vertexCount);
                int j = metodo_roleta(p);
                
                s.erase(j);
                visitados[current] = true;
                
                current = j;
                caminho_corrent.push_back(current);
            }
            caminho_corrent.push_back(formiga);

            for(auto fora : caminho_corrent)
                cout << fora << ' ';
            cout << endl;
            caminhos_formigas.push_back(caminho_corrent);
            /*
                Fazer uma função que receba caminhos_formigas e retorne o melhor dos caminhos
            */
        }

        inter++;
    }
}

int main() {
    int vertexCount = 5;
    cin >> vertexCount;

    // Inicializa a matriz de adjacência
    // vector<vector<double>> graph = {{0, 22, 50, 48, 29}, {22, 0, 30, 34, 32}, {50, 30, 0, 22, 23}, {48, 34, 22, 0, 35}, {29, 32, 23, 35, 0}};
    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);

    // vector<bool> visitados(vertexCount, false);
    // visitados[0] = true;

    // vector<vector<double>> feromonios(vertexCount, vector<double>(vertexCount));
    // vector<vector<double>> heuristica(vertexCount, vector<double>(vertexCount));
    // inicializa_feromonios(feromonios, 0.1, vertexCount);
    // inicializa_heuristica(heuristica, graph, vertexCount);

    // vector<pair<int, double>> p = probabilidade(feromonios, heuristica, visitados, 0, 1, 1, vertexCount);
    // metodo_roleta(p);
    
    ACO(graph, vertexCount);   

    return 0;
}
