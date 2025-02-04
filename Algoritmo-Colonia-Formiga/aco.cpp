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

double calculatePathCost(vector<vector<double>> &graph, vector<int> &path) {
    double totalCost = 0;
    for (int i = 1; i < path.size(); i++) {
        totalCost += graph[path[i - 1]][path[i]];
    }
    totalCost += graph[path.back()][path[0]]; // Retorna ao ponto inicial
    return totalCost;
}

vector<int> melhor_caminho(vector<vector<double>> &graph, vector<vector<int>> &caminhos){
    vector<pair<double, int>> custo_caminhos(caminhos.size());
    for(int i = 0; i < caminhos.size(); i++){
        custo_caminhos[i] = {calculatePathCost(graph, caminhos[i]), i};
    }

    sort(custo_caminhos.begin(), custo_caminhos.end());
    return caminhos[custo_caminhos[0].second];
}

void evaporar(vector<vector<double>> &feromonios, double taxaEvaporacao, int vertexCount){
    for(int i = 0; i < vertexCount; i++){
        for(int j = 0; j < vertexCount; j++){
            feromonios[i][j] = (1 - taxaEvaporacao) * feromonios[i][j];
        }
    }
}

void reforcar(vector<vector<double>> &feromonios, vector<int> &melhorCaminho, double delta, int vertexCount){
    for(int i = 0; i < melhorCaminho.size() - 1; i++){
        feromonios[melhorCaminho[i]][melhorCaminho[i + 1]] += delta;
        feromonios[melhorCaminho[i+1]][melhorCaminho[i]] += delta;
    }
}

double ACO(vector<vector<double>> &graph, int vertexCount){
    int Kformigas = vertexCount;
    double taxaEvaporacao = 0.01;

    const int maxIteracoes = 100;
    int inter = 0;

    vector<vector<double>> feromonios(vertexCount, vector<double>(vertexCount));
    vector<vector<double>> heuristica(vertexCount, vector<double>(vertexCount));
    inicializa_feromonios(feromonios, 0.1, vertexCount);
    inicializa_heuristica(heuristica, graph, vertexCount);

    vector<int> bestSolution;
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
                double alpha = 2.5;
                double beta = 1;

                vector<pair<int, double>> p = probabilidade(feromonios, heuristica, visitados, current, alpha, beta, vertexCount);
                int j = metodo_roleta(p);
                
                s.erase(j);
                visitados[current] = true;
                
                current = j;
                caminho_corrent.push_back(current);
            }
            caminho_corrent.push_back(formiga);
            caminhos_formigas.push_back(caminho_corrent);
        }

        evaporar(feromonios, taxaEvaporacao, vertexCount);

        vector<int> pi = melhor_caminho(graph, caminhos_formigas);
        if(bestSolution.size() == 0 || calculatePathCost(graph, pi) <   calculatePathCost(graph, bestSolution)){
            bestSolution = pi;
        }

        double delta = 10/calculatePathCost(graph, pi);
        reforcar(feromonios, pi, delta, vertexCount);

        inter++;
    }

    return calculatePathCost(graph ,bestSolution);
}

int main() {
    int vertexCount;
    cin >> vertexCount;

    // Inicializa a matriz de adjacência
    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);
    
    double result = ACO(graph, vertexCount);
    cout << result << endl;   

    return 0;
}
