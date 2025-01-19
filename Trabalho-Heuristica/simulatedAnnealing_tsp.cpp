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

// Algoritmo do Vizinho Mais Próximo para gerar um caminho inicial
vector<int> nearestNeighborPath(vector<vector<double>> &graph, int vertexCount) {
    vector<bool> visited(vertexCount, false);
    vector<int> path;

    int startVertex = 0; // Começa pelo primeiro vértice
    visited[startVertex] = true;
    path.push_back(startVertex);

    while (true) {
        int currentVertex = path.back();
        int nearestVertex = -1;
        int nearestDistance = -1;

        // Busca o vértice mais próximo não visitado
        for (int i = 0; i < vertexCount; i++) {
            if (visited[i]) continue;
            if (nearestDistance == -1 || graph[currentVertex][i] < nearestDistance) {
                nearestDistance = graph[currentVertex][i];
                nearestVertex = i;
            }
        }

        if (nearestVertex == -1) break; // Todos os vértices foram visitados

        path.push_back(nearestVertex);
        visited[nearestVertex] = true;
    }

    return path;
}

// Gera um vizinho aleatório trocando dois vértices no caminho
vector<int> generateNeighbor(vector<int> &path) {
    uniform_int_distribution<int> distribution(0, path.size() - 1);
    int i, j;
    do {
        i = distribution(generator);
        j = distribution(generator);
    } while (i == j);

    vector<int> neighborPath = path;
    swap(neighborPath[i], neighborPath[j]);
    return neighborPath;
}

// Calcula o custo total do caminho
double calculatePathCost(vector<vector<double>> &graph, vector<int> &path) {
    double totalCost = 0;
    for (int i = 1; i < path.size(); i++) {
        totalCost += graph[path[i - 1]][path[i]];
    }
    totalCost += graph[path.back()][path[0]]; // Retorna ao ponto inicial
    return totalCost;
}

// Implementa o algoritmo de Simulated Annealing para minimizar o custo do caminho
double simulatedAnnealing(vector<vector<double>> &graph, double temperature, double coolingRate, int maxIterations, vector<int> &initialSolution) {
    vector<int> currentSolution = initialSolution;
    vector<int> bestSolution = initialSolution;

    ll iteration = 0;
    while (temperature > ERROR_THRESHOLD) {
        while (iteration < maxIterations) {
            vector<int> neighborSolution = generateNeighbor(currentSolution);

            double currentCost = calculatePathCost(graph, currentSolution);
            double neighborCost = calculatePathCost(graph, neighborSolution);

            if (neighborCost < currentCost) {
                currentSolution = neighborSolution;
                if (neighborCost < calculatePathCost(graph, bestSolution)) {
                    bestSolution = neighborSolution;
                }
            } else {
                double acceptanceProbability = exp((currentCost - neighborCost) / temperature);
                uniform_real_distribution<> distribution(0.0, 1.0);
                if (distribution(generator) < acceptanceProbability) {
                    currentSolution = neighborSolution;
                }
            }

            iteration++;
        }

        temperature *= coolingRate; // Resfria a temperatura
        iteration = 0;
    }

    return calculatePathCost(graph, bestSolution);
}

int main() {
    int vertexCount;
    cin >> vertexCount;

    // Inicializa a matriz de adjacência
    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);

    // Gera a solução inicial com o algoritmo do Vizinho Mais Próximo
    vector<int> initialSolution(vertexCount);
    for(int i = 0; i < vertexCount; i++){
        initialSolution[i] = i;
    }

    vector<int> temperature = {100, 1000, 10000};
    vector<double> coolingRate = {0.5, 0.75, 0.9, 0.999};
    vector<int> maxIterations = {5, 10, 50, 100};

    // Cria o arquivo de saída
    ofstream file("results_satsp.csv");

    // Cabeçalho do CSV
    file << "Temperatura,Coeficiente de Resfriamento,Máximo de Iterações,Execução,Função Objetivo,Tempo (ms)\n";

    // Loops para variar os parâmetros
    for (int l = 0; l < temperature.size(); l++) {
        for (int j = 0; j < coolingRate.size(); j++) {
            for (int k = 0; k < maxIterations.size(); k++) {
                for (int i = 0; i < 10; i++) {
                    // Medir tempo de execução
                    auto start = chrono::high_resolution_clock::now();
                    double optimalCost = simulatedAnnealing(graph, temperature[l], coolingRate[j], maxIterations[k], initialSolution);
                    auto end = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> temp = end - start;

                    // Escreve os resultados no arquivo
                    file << temperature[l] << ","
                         << coolingRate[j] << ","
                         << maxIterations[k] << ","
                         << i + 1 << ","
                         << fixed << setprecision(2) << optimalCost << ","
                         << temp.count() << "\n";
                }
            }
        }
    }

    file.close();
    cout << "Resultados salvos em 'results_satsp.csv'" << endl;
    return 0;
}