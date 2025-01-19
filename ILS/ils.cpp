#include <bits/stdc++.h>
using namespace std;

// Definição de tipos e constantes
#define ll long long
#define endl '\n'

// Gerador de números aleatórios
random_device rd;
mt19937 generator(rd());

// Função para ler as coordenadas dos vértices e calcular a matriz de adjacência
vector<vector<double>> buildAdjacencyMatrix(vector<vector<double>> &adjMatrix, int vertexCount) {
    vector<pair<ll, ll>> coordinates(vertexCount);

    // Leitura das coordenadas dos vértices
    ll index, x, y;
    for (int i = 0; i < vertexCount; i++) {
        cin >> index >> x >> y;
        index--; // Ajusta o índice para zero-based
        coordinates[index] = make_pair(x, y);
    }

    // Calcula as distâncias euclidianas para preencher a matriz de adjacência
    for (int i = 0; i < vertexCount; i++) {
        ll x1 = coordinates[i].first;
        ll y1 = coordinates[i].second;
        for (int j = 0; j < vertexCount; j++) {
            ll x2 = coordinates[j].first;
            ll y2 = coordinates[j].second;
            adjMatrix[i][j] = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        }
    }
    return adjMatrix;
}

// Função para calcular o custo total de um caminho
double calculatePathCost(const vector<vector<double>> &adjMatrix, const vector<int> &path) {
    double totalCost = 0;

    // Soma os custos das arestas no caminho
    for (int i = 1; i < path.size(); i++) {
        totalCost += adjMatrix[path[i - 1]][path[i]];
    }

    // Adiciona o custo de retorno ao ponto inicial
    totalCost += adjMatrix[path.back()][path[0]];

    return totalCost;
}

// Função para gerar permutações de vizinhança do caminho
vector<vector<int>> generateNeighbors(const vector<int> &currentPath) {
    vector<vector<int>> neighbors;

    // Gera permutações por troca de dois vértices
    for (int i = 0; i < currentPath.size(); i++) {
        for (int j = i + 1; j < currentPath.size(); j++) {
            vector<int> neighbor = currentPath;
            swap(neighbor[i], neighbor[j]);
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
}

// Verifica se dois caminhos são iguais
bool arePathsEqual(const vector<int> &path1, const vector<int> &path2) {
    return path1 == path2;
}

// Função de busca local para resolver o TSP (Travelling Salesman Problem)
vector<int> localSearchTSP(const vector<vector<double>> &adjMatrix, vector<int> &currentPath) {
    vector<int> bestPath;
    double bestCost;

    do {
        bestPath = currentPath;
        bestCost = calculatePathCost(adjMatrix, currentPath);

        // Gera os vizinhos e busca pelo melhor custo
        vector<vector<int>> neighbors = generateNeighbors(currentPath);
        for (const auto &neighbor : neighbors) {
            double neighborCost = calculatePathCost(adjMatrix, neighbor);
            if (neighborCost < bestCost) {
                currentPath = neighbor;
                bestCost = neighborCost;
            }
        }
    } while (!arePathsEqual(bestPath, currentPath));

    return bestPath;
}

// Função para aplicar perturbações no caminho atual
vector<int> applyPerturbation(const vector<int> &currentPath, int level) {
    vector<int> perturbedPath = currentPath;
    set<pair<int, int>> swaps;
    uniform_int_distribution<int> distribution(0, currentPath.size() - 1);

    int appliedPerturbations = 0;

    // Realiza trocas aleatórias
    while (appliedPerturbations < level) {
        int i, j;
        do {
            i = distribution(generator);
            j = distribution(generator);
        } while ((swaps.count({i, j}) || swaps.count({j, i})) || i == j);

        swap(perturbedPath[i], perturbedPath[j]);
        swaps.insert({i, j});
        swaps.insert({j, i});
        appliedPerturbations++;
    }

    return perturbedPath;
}

// Implementação do algoritmo ILS (Iterated Local Search)
double performILS(const vector<vector<double>> &adjMatrix, vector<int> &initialSolution) {
    const int maxILSIterations = 50;
    int iteration = 0;
    int perturbationLevel = 1;

    vector<int> bestPath = localSearchTSP(adjMatrix, initialSolution);

    // Loop principal do ILS
    while (iteration < maxILSIterations) {
        iteration++;

        // Aplica perturbação e realiza busca local
        vector<int> perturbedSolution = applyPerturbation(bestPath, perturbationLevel);
        vector<int> newBestPath = localSearchTSP(adjMatrix, perturbedSolution);

        // Atualiza a melhor solução, se necessário
        if (calculatePathCost(adjMatrix, newBestPath) < calculatePathCost(adjMatrix, bestPath)) {
            bestPath = newBestPath;
            iteration = 0;
            perturbationLevel = 1;
        } else {
            perturbationLevel++;
        }
    }

    return calculatePathCost(adjMatrix, bestPath);
}

int main() {
    int vertexCount;
    cin >> vertexCount;

    // Inicializa a matriz de adjacência
    vector<vector<double>> adjMatrix(vertexCount, vector<double>(vertexCount));
    buildAdjacencyMatrix(adjMatrix, vertexCount);

    // Gera a solução inicial
    vector<int> initialSolution(vertexCount);
    iota(initialSolution.begin(), initialSolution.end(), 0);

    // Executa o algoritmo ILS
    double result = performILS(adjMatrix, initialSolution);

    // Exibe o resultado final
    cout << fixed << setprecision(2) << result << endl;
    
    return 0;
}
