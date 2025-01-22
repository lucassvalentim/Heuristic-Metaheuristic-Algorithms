#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <utility>

using namespace std;

#define ll long long
#define endl '\n'

// Gerador de números aleatórios
random_device rd;
mt19937 generator(rd());

// Função para ler o grafo e calcular a matriz de distâncias
vector<vector<double>> buildDistanceMatrix(vector<vector<double>> &distanceMatrix, int vertexCount) {
    vector<pair<ll, ll>> coordinates(vertexCount);

    // Leitura das coordenadas dos vértices
    ll index, x, y;
    for (int i = 0; i < vertexCount; i++) {
        cin >> index >> x >> y;
        index--; // Ajusta o índice para zero-based
        coordinates[index] = make_pair(x, y);
    }

    // Calcula as distâncias euclidianas entre os vértices
    for (int i = 0; i < vertexCount; i++) {
        ll x1 = coordinates[i].first, y1 = coordinates[i].second;
        for (int j = 0; j < vertexCount; j++) {
            ll x2 = coordinates[j].first, y2 = coordinates[j].second;
            distanceMatrix[i][j] = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        }
    }
    return distanceMatrix;
}

// Inverte uma parte do caminho entre os índices start e end
void reverseSegment(vector<ll> &path, ll start, ll end) {
    while (start < end) {
        swap(path[start], path[end]);
        start++;
        end--;
    }
}

// Gera vizinhos utilizando a estratégia 2-opt
vector<vector<ll>> generate2OptNeighbors(const vector<ll> &path) {
    vector<vector<ll>> neighbors;

    for (int i = 0; i < path.size() - 3; i++) {
        for (int j = i + 2; j < path.size() - 1; j++) {
            vector<ll> newPath = path;
            reverseSegment(newPath, i + 1, j);
            neighbors.push_back(newPath);
        }
    }

    return neighbors;
}

// Gera vizinhos utilizando a estratégia 2-swap
vector<vector<ll>> generate2SwapNeighbors(const vector<ll> &path) {
    vector<vector<ll>> neighbors;

    for (int i = 0; i < path.size(); i++) {
        for (int j = i + 1; j < path.size(); j++) {
            vector<ll> newPath = path;
            swap(newPath[i], newPath[j]);
            neighbors.push_back(newPath);
        }
    }

    return neighbors;
}

// Calcula o custo total de um caminho
double calculatePathCost(const vector<vector<double>> &distanceMatrix, const vector<ll> &path) {
    double totalCost = 0;
    for (size_t i = 1; i < path.size(); i++) {
        totalCost += distanceMatrix[path[i - 1]][path[i]];
    }
    totalCost += distanceMatrix[path[path.size() - 1]][path[0]]; // Última aresta de volta ao início
    return totalCost;
}

// Verifica se dois caminhos são iguais
bool arePathsEqual(const vector<ll> &path1, const vector<ll> &path2) {
    return equal(path1.begin(), path1.end(), path2.begin());
}

// Realiza uma perturbação na solução atual
vector<ll> applyDisturbance(const vector<ll> &currentPath, int neighborhoodType) {
    vector<vector<ll>> neighbors;

    if (neighborhoodType == 1) {
        neighbors = generate2SwapNeighbors(currentPath);
    } else if (neighborhoodType == 2) {
        neighbors = generate2OptNeighbors(currentPath);
    }

    uniform_int_distribution<int> distribution(0, neighbors.size() - 1);
    return neighbors[distribution(generator)];
}

// Realiza a busca local para encontrar uma solução melhor em um vizinho
vector<ll> localSearch(const vector<vector<double>> &distanceMatrix, vector<ll> currentPath, int neighborhoodType) {
    vector<ll> previousPath(currentPath.size());
    double currentCost, bestCost;

    do {
        previousPath = currentPath;
        vector<vector<ll>> neighbors;

        // Gera vizinhança com base no tipo especificado
        if (neighborhoodType == 1) {
            neighbors = generate2SwapNeighbors(currentPath);
        } else if (neighborhoodType == 2) {
            neighbors = generate2OptNeighbors(currentPath);
        }

        bestCost = calculatePathCost(distanceMatrix, currentPath);

        for (const auto &neighbor : neighbors) {
            currentCost = calculatePathCost(distanceMatrix, neighbor);
            if (currentCost < bestCost) {
                bestCost = currentCost;
                currentPath = neighbor;
            }
        }
    } while (!arePathsEqual(previousPath, currentPath));

    return currentPath;
}

double variableNeighborhoodSearch(const vector<vector<double>> &distanceMatrix, vector<ll> &initialSolution) {
    constexpr ll MAX_ITERATIONS = 100;  // Número máximo de iterações sem melhoria
    constexpr int MAX_NEIGHBORHOOD = 2; // Número máximo de estruturas de vizinhança

    vector<ll> currentSolution = initialSolution; // Solução corrente
    vector<ll> bestSolution = initialSolution;    // Melhor solução encontrada
    double bestCost = calculatePathCost(distanceMatrix, bestSolution); // Custo da melhor solução

    ll iterationsWithoutImprovement = 0;

    // Loop até atingir o número máximo de iterações sem melhoria
    while (iterationsWithoutImprovement < MAX_ITERATIONS) {
        int currentNeighborhood = 1;

        while (currentNeighborhood <= MAX_NEIGHBORHOOD) {
            // Perturbação na solução corrente
            vector<ll> perturbedSolution = applyDisturbance(currentSolution, currentNeighborhood);

            // Busca local na solução perturbada
            vector<ll> localOptimalSolution = localSearch(distanceMatrix, perturbedSolution, currentNeighborhood);
            double localOptimalCost = calculatePathCost(distanceMatrix, localOptimalSolution);

            // Atualiza a solução corrente se a solução local for melhor
            if (localOptimalCost < calculatePathCost(distanceMatrix, currentSolution)) {
                currentSolution = localOptimalSolution;
                currentNeighborhood = 1; // Reinicia a vizinhança
            } else {
                currentNeighborhood++;
            }
        }

        // Verifica se a solução corrente melhora a melhor solução global
        double currentCost = calculatePathCost(distanceMatrix, currentSolution);
        if (currentCost < bestCost) {
            bestSolution = currentSolution;
            bestCost = currentCost;
            iterationsWithoutImprovement = 0; // Reseta o contador de iterações sem melhoria
        } else {
            iterationsWithoutImprovement++;
        }
    }

    return bestCost; // Retorna o custo da melhor solução encontrada
}


int main() {
    ll vertexCount;
    cin >> vertexCount;

    vector<vector<double>> distanceMatrix(vertexCount, vector<double>(vertexCount));
    buildDistanceMatrix(distanceMatrix, vertexCount);

    // Inicializa solução inicial como sequência ordenada
    vector<ll> initialSolution(vertexCount);
    iota(initialSolution.begin(), initialSolution.end(), 0);

    double result = variableNeighborhoodSearch(distanceMatrix, initialSolution);
    cout << result << endl;

    return 0;
}
