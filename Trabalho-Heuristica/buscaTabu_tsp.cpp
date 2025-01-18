#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

// Função para calcular o custo total de uma rota
double calculateRouteCost(const vector<int>& route, const vector<vector<double>>& graph) {
    double cost = 0;
    for (size_t i = 0; i < route.size() - 1; i++) {
        cost += graph[route[i]][route[i + 1]];
    }
    cost += graph[route.back()][route[0]]; // Volta ao ponto inicial
    return cost;
}

// Função para gerar um vizinho trocando dois vértices
vector<int> generateNeighbor(const vector<int>& currentRoute) {
    vector<int> neighbor = currentRoute;
    int n = neighbor.size();
    int i = rand() % n;
    int j = rand() % n;
    while (i == j) {
        j = rand() % n;
    }
    swap(neighbor[i], neighbor[j]);
    return neighbor;
}

// Função para selecionar o melhor vizinho considerando a lista tabu
vector<int> selectBestNeighbor(const vector<int>& currentRoute, const vector<int>& bestRoute, 
                               const vector<vector<int>>& tabuList, const vector<vector<double>>& graph, int tabuDuration) {
    vector<int> bestNeighbor = currentRoute;
    double bestCost = calculateRouteCost(currentRoute, graph);

    for (int i = 0; i < currentRoute.size(); i++) {
        for (int j = i + 1; j < currentRoute.size(); j++) {
            vector<int> neighbor = currentRoute;
            swap(neighbor[i], neighbor[j]);

            double neighborCost = calculateRouteCost(neighbor, graph);

            if (tabuList[i][j] > 0 && neighborCost >= calculateRouteCost(bestRoute, graph)) {
                continue; // Ignorar movimentos tabu que não melhoram a solução global
            }

            if (neighborCost < bestCost) {
                bestNeighbor = neighbor;
                bestCost = neighborCost;
            }
        }
    }

    return bestNeighbor;
}

// Atualizar lista tabu
void updateTabuList(vector<vector<int>>& tabuList) {
    for (auto& row : tabuList) {
        for (auto& entry : row) {
            if (entry > 0) {
                --entry;
            }
        }
    }
}

// Implementação da Busca Tabu
double tabuSearch(const vector<vector<double>>& graph, int vertexCount) {
    // Inicializa rota inicial (ciclo trivial 0 -> 1 -> 2 -> ... -> n -> 0)
    vector<int> currentRoute(vertexCount);
    iota(currentRoute.begin(), currentRoute.end(), 0);
    vector<int> bestRoute = currentRoute;

    double bestCost = calculateRouteCost(bestRoute, graph);

    // Lista tabu (matriz para trocas entre vértices)
    vector<vector<int>> tabuList(vertexCount, vector<int>(vertexCount, 0));
    int tabuDuration = 10;
    int iterationsWithoutImprovement = 0;

    while (iterationsWithoutImprovement < 100) {
        // Seleciona o melhor vizinho
        vector<int> neighbor = selectBestNeighbor(currentRoute, bestRoute, tabuList, graph, tabuDuration);

        // Atualiza lista tabu para o movimento escolhido
        for (int i = 0; i < currentRoute.size(); i++) {
            for (int j = i + 1; j < currentRoute.size(); j++) {
                if (currentRoute[i] != neighbor[i] || currentRoute[j] != neighbor[j]) {
                    tabuList[i][j] = tabuDuration;
                }
            }
        }

        currentRoute = neighbor;
        double currentCost = calculateRouteCost(currentRoute, graph);

        if (currentCost < bestCost) {
            bestRoute = currentRoute;
            bestCost = currentCost;
            iterationsWithoutImprovement = 0;
        } else {
            ++iterationsWithoutImprovement;
        }

        updateTabuList(tabuList);
    }

    return bestCost;
}

vector<vector<double>> readGraph(vector<vector<double>>& graph, int vertexCount) {
    vector<pair<ll, ll>> coordinates(vertexCount);

    ll index, x, y;
    for (int i = 0; i < vertexCount; i++) {
        cin >> index >> x >> y;
        index--; // Ajusta índice para zero-based
        coordinates[index] = make_pair(x, y);
    }

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

int main() {
    int vertexCount;
    cin >> vertexCount;

    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);

    double bestCost = tabuSearch(graph, vertexCount);
    cout << "Best TSP Cost: " << fixed << setprecision(2) << bestCost << endl;

    return 0;
}
