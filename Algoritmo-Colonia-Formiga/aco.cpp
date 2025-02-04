#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'
const double ERROR_THRESHOLD = 1e-2;

random_device rd;
mt19937 generator(rd());

// Função para ler o grafo e calcular a matriz de distâncias entre os vértices
vector<vector<double>> readGraph(vector<vector<double>> &graph, int vertexCount) {
    vector<pair<ll, ll>> coordinates(vertexCount);

    ll index, x, y;
    for (int i = 0; i < vertexCount; i++) {
        cin >> index >> x >> y;
        index--;
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

// Inicializa os feromônios na matriz
void initializePheromones(vector<vector<double>> &pheromones, double initialValue, int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (i != j)
                pheromones[i][j] = initialValue;
        }
    }
}

// Inicializa a matriz heurística baseada na distância
void initializeHeuristic(vector<vector<double>> &heuristic, vector<vector<double>> &graph, int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (i != j)
                heuristic[i][j] = 1 / graph[i][j];
        }
    }
}

// Calcula as probabilidades de escolha de um vértice baseado em feromônio e heurística
vector<pair<int, double>> calculateProbabilities(vector<vector<double>> &pheromones, vector<vector<double>> &heuristic, vector<bool> visited, int current, double alpha, double beta, int vertexCount) {
    vector<pair<int, double>> probabilities;
    double sum = 0;
    for (int j = 0; j < vertexCount; j++) {
        if (!visited[j])
            sum += pow(pheromones[current][j], alpha) * pow(heuristic[current][j], beta);
    }

    for (int j = 0; j < vertexCount; j++) {
        if (!visited[j]) {
            double probability = (pow(pheromones[current][j], alpha) * pow(heuristic[current][j], beta)) / sum;
            probabilities.push_back({j, probability});
        }
    }

    if (probabilities.empty())
        probabilities.push_back({current, 1});

    return probabilities;
}

// Método de roleta para escolher próximo vértice
int rouletteWheelSelection(vector<pair<int, double>> &probabilities) {
    vector<pair<int, double>> roulette(probabilities.size());
    roulette[0] = probabilities[0];

    for (int i = 1; i < probabilities.size(); i++) {
        roulette[i].first = probabilities[i].first;
        roulette[i].second = roulette[i - 1].second + probabilities[i].second;
    }

    uniform_real_distribution<double> dis(0, 1);
    double randomValue = dis(generator);
    for (auto &entry : roulette) {
        if (randomValue <= entry.second) {
            return entry.first;
        }
    }

    return probabilities.back().first;
}

// Calcula o custo total de um caminho percorrido
double calculatePathCost(vector<vector<double>> &graph, vector<int> &path) {
    double totalCost = 0;
    for (int i = 1; i < path.size(); i++) {
        totalCost += graph[path[i - 1]][path[i]];
    }

    totalCost += graph[path.back()][path[0]];
    return totalCost;
}

// Retorna o melhor caminho encontrado
vector<int> findBestPath(vector<vector<double>> &graph, vector<vector<int>> &paths) {
    vector<pair<double, int>> pathCosts(paths.size());
    for (int i = 0; i < paths.size(); i++) {
        pathCosts[i] = {calculatePathCost(graph, paths[i]), i};
    }

    sort(pathCosts.begin(), pathCosts.end());
    return paths[pathCosts[0].second];
}

// Evaporação dos feromônios
void evaporatePheromones(vector<vector<double>> &pheromones, double evaporationRate, int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            pheromones[i][j] *= (1 - evaporationRate);
        }
    }
}

// Reforço dos feromônios baseado no melhor caminho encontrado
void reinforcePheromones(vector<vector<double>> &pheromones, vector<int> &bestPath, double delta) {
    for (int i = 0; i < bestPath.size() - 1; i++) {
        pheromones[bestPath[i]][bestPath[i + 1]] += delta;
        pheromones[bestPath[i + 1]][bestPath[i]] += delta;
    }
}

// Algoritmo de Otimização por Colônia de Formigas (ACO)
double ACO(vector<vector<double>> &graph, int vertexCount) {
    int antCount = vertexCount;
    double evaporationRate = 0.01;
    const int maxIterations = 100;
    int iteration = 0;

    vector<vector<double>> pheromones(vertexCount, vector<double>(vertexCount));
    vector<vector<double>> heuristic(vertexCount, vector<double>(vertexCount));
    initializePheromones(pheromones, 0.1, vertexCount);
    initializeHeuristic(heuristic, graph, vertexCount);

    vector<int> bestSolution;
    while (iteration < maxIterations) {
        vector<vector<int>> antPaths;
        for (int ant = 0; ant < antCount; ant++) {
            vector<bool> visited(vertexCount, false);
            int current = ant;
            visited[current] = true;
            vector<int> currentPath{current};

            while (currentPath.size() < vertexCount) {
                double alpha = 2.5, beta = 1;
                vector<pair<int, double>> probabilities = calculateProbabilities(pheromones, heuristic, visited, current, alpha, beta, vertexCount);
                int next = rouletteWheelSelection(probabilities);
                visited[next] = true;
                current = next;
                currentPath.push_back(current);
            }
            currentPath.push_back(ant);
            antPaths.push_back(currentPath);
        }

        evaporatePheromones(pheromones, evaporationRate, vertexCount);
        vector<int> bestPath = findBestPath(graph, antPaths);
        if (bestSolution.empty() || calculatePathCost(graph, bestPath) < calculatePathCost(graph, bestSolution)) {
            bestSolution = bestPath;
        }
        double delta = 10 / calculatePathCost(graph, bestPath);
        reinforcePheromones(pheromones, bestPath, delta);

        iteration++;
    }
    return calculatePathCost(graph, bestSolution);
}

int main() {
    int vertexCount;
    cin >> vertexCount;

    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);
    
    cout << ACO(graph, vertexCount) << endl;
    return 0;
}
