#include<bits/stdc++.h>

using Integer = long long;
using namespace std;

string convertToBinary(Integer number, Integer length) {
    string result(length, '0');
    for (Integer i = length - 1; i >= 0; --i) {
        if (number & (1LL << i)) {
            result[length - 1 - i] = '1';
        }
    }
    return result;
}

vector<string> generateBinaryCombinations(Integer bitCount) {
    vector<string> combinations;
    for (Integer i = 0; i < (1LL << bitCount); ++i) {
        combinations.push_back(convertToBinary(i, bitCount));
    }
    return combinations;
}

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

string knapsackByValueDensity(const vector<Integer>& values, const vector<Integer>& weights, Integer capacity) {
    vector<pair<double, Integer>> densityIndex;
    for (size_t i = 0; i < values.size(); ++i) {
        densityIndex.emplace_back(static_cast<double>(values[i]) / weights[i], i);
    }

    sort(densityIndex.rbegin(), densityIndex.rend());

    string result(values.size(), '0');
    Integer currentWeight = 0;

    for (const auto& item : densityIndex) {
        Integer index = item.second;
        if (currentWeight + weights[index] <= capacity) {
            result[index] = '1';
            currentWeight += weights[index];
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

Integer tabuSearch(const vector<Integer>& values, const vector<Integer>& weights, Integer capacity) {
    string current = knapsackByValueDensity(values, weights, capacity);
    string bestSolution = current;

    vector<Integer> tabuList(values.size(), 0);
    Integer iterationsWithoutImprovement = 0, penaltyFactor = 0, tabuDuration = 10;

    for (const auto& weight : weights) {
        penaltyFactor += weight;
    }

    while (iterationsWithoutImprovement < 100) {
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

    cout << tabuSearch(values, weights, capacity) << endl;
    return 0;
}
