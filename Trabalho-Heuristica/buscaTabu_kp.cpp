#include<bits/stdc++.h>

using Integer = long long;
using namespace std;

Integer calculateKnapsackValue(string config, const vector<Integer>& values, const vector<Integer>& weights, Integer capacity) {
    Integer totalValue = 0, totalWeight = 0;
    for (size_t i = 0; i < values.size(); ++i) {
        if (config[i] == '1') {
            totalValue += values[i];
            totalWeight += weights[i];
            if (totalWeight > capacity) return -1;
        }
    }
    return totalValue;
}

// Inicialização aleatória da solução
string knapsackByRandom(const vector<Integer>& values, const vector<Integer>& weights, Integer capacity) {
    string result(values.size(), '0');
    Integer currentWeight = 0;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);  // aleatório entre 0 e 1

    for (size_t i = 0; i < values.size(); ++i) {
        if (dis(gen) == 1 && currentWeight + weights[i] <= capacity) {
            result[i] = '1';
            currentWeight += weights[i];
        }
    }

    return result;
}

void updateTabuList(vector<Integer>& tabuList, Integer index, Integer duration) {
    for (auto& val : tabuList) {
        if (val > 0) --val;
    }
    tabuList[index] = duration;
}

Integer evaluateConfiguration(const string& config, const vector<Integer>& values, const vector<Integer>& weights, Integer capacity, Integer penalty) {
    Integer value = 0, weight = 0;
    for (size_t i = 0; i < config.size(); ++i) {
        if (config[i] == '1') {
            value += values[i];
            weight += weights[i];
        }
    }
    return value - penalty * max(0LL, weight - capacity);
}

string selectBestNeighbor(const string& current, const string& bestSoFar, const vector<Integer>& tabuList, const vector<Integer>& values, const vector<Integer>& weights, Integer capacity, Integer penalty) {
    string bestNeighbor = current;
    Integer bestValue = numeric_limits<Integer>::min();

    for (size_t i = 0; i < current.size(); ++i) {
        string neighbor = current;
        neighbor[i] = (current[i] == '1') ? '0' : '1';

        Integer neighborValue = evaluateConfiguration(neighbor, values, weights, capacity, penalty);

        if (tabuList[i] > 0 && neighborValue <= evaluateConfiguration(bestSoFar, values, weights, capacity, penalty)) {
            continue;
        }

        if (neighborValue > bestValue) {
            bestNeighbor = neighbor;
            bestValue = neighborValue;
        }
    }
    return bestNeighbor;
}

Integer tabuSearch(const vector<Integer>& values, const vector<Integer>& weights, Integer tabuDuration , Integer interationMAX, Integer capacity) {
    // Inicialização aleatória da solução
    string current = knapsackByRandom(values, weights, capacity);
    string bestSolution = current;

    vector<Integer> tabuList(values.size(), 0);
    Integer iterationsWithoutImprovement = 0, penaltyFactor = 0;

    for (const auto& weight : weights) {
        penaltyFactor += weight;
    }

    while (iterationsWithoutImprovement < interationMAX) {
        string neighbor = selectBestNeighbor(current, bestSolution, tabuList, values, weights, capacity, penaltyFactor);
        updateTabuList(tabuList, neighbor.find('1'), tabuDuration);

        current = neighbor;

        if (evaluateConfiguration(current, values, weights, capacity, penaltyFactor) > evaluateConfiguration(bestSolution, values, weights, capacity, penaltyFactor)) {
            bestSolution = current;
            iterationsWithoutImprovement = 0;
        } else {
            ++iterationsWithoutImprovement;
        }
    }
    return calculateKnapsackValue(bestSolution, values, weights, capacity);
}

int main() {
    Integer itemCount, capacity;
    cin >> itemCount >> capacity;

    vector<Integer> values(itemCount), weights(itemCount);

    for (Integer i = 0; i < itemCount; ++i) {
        cin >> values[i] >> weights[i];
    }

    vector<int> tabuDuration = {5, 10, 20};
    vector<int> interationMAX = {5, 100, 200};

    // Cria o arquivo de saída
    ofstream file("results_btkp.csv");

    // Cabeçalho do CSV
    file << "Duração Da Lista Tabu,Máximo de Iterações,Função Objetivo,Tempo (ms)\n";

    // Loops para variar os parâmetros
    for (int j = 0; j < tabuDuration.size(); j++) {
        for (int k = 0; k < interationMAX.size(); k++) {
            // Medir tempo de execução
            auto start = chrono::high_resolution_clock::now();
            Integer optimalCost = tabuSearch(values, weights, tabuDuration[j], interationMAX[k], capacity);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> temp = end - start;

            // Escreve os resultados no arquivo
            file << tabuDuration[j] << ","
                    <<  interationMAX[k] << ","
                    << optimalCost << "," << fixed << setprecision(2) 
                    << temp.count() << "\n";
        }
    }
    
    return 0;
}
