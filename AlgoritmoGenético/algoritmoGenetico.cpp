#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

random_device rd;
mt19937 generator(rd());

int AptidaoFunciton(string s, vector<int> &values){
    int totalValues = 0;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == '1'){
            totalValues += values[i];
        }
    }

    return totalValues;
}

set<pair<int, string>> avliationPopulation(set<pair<int, string>> &population, vector<int> &values){
    set<pair<int, string>> newPopulation;
    for(auto x : population){
        newPopulation.insert({AptidaoFunciton(x.second, values), x.second});
    }

    return newPopulation;
}

set<pair<int, string>> generateRandomInicialSolution(vector<int> &values, vector<int> &weights, int capacity, int popSize){
    set<pair<int, string>> uniqueSolutions;
    uniform_real_distribution<double>dis(0, 1);

    while(uniqueSolutions.size() < popSize){
        int totalCapacity = 0;
        string inicialSolution(values.size(), '0');
        for(int i = 0; i < inicialSolution.size(); i++){
            if(dis(generator) < 0.5 && totalCapacity + weights[i] <= capacity){
                inicialSolution[i] = '1';
                totalCapacity += weights[i];
            }
        }
        uniqueSolutions.insert({AptidaoFunciton(inicialSolution, values), inicialSolution});
    }

    return uniqueSolutions;
}

vector<double> calculateProbabilities(set<pair<int, string>> &population){
    vector<double> probabilities;
    double totalFitness = 0.0;

    for(const pair<int, string> &individual : population){
        totalFitness += individual.first;
    }

    for(const pair<int, string> &individual : population){
        probabilities.push_back(individual.first / totalFitness);
    }

    return probabilities;
}

vector<pair<string, string>> formPairs(vector<string> &parents, double crossRate){
    uniform_real_distribution<double> dis(0, 1);

    vector<pair<string, string>> pairs;
    for(int i = 0; i < parents.size(); i+=2){
        if(i + 1 < parents.size() && dis(generator) <= crossRate){
            pairs.push_back({parents[i], parents[i+1]});
        }
    }

    if(pairs.size() == 0){
        pairs.push_back({parents[0], parents[1]});
    }

    return pairs;
}

vector<pair<string, string>> selectParents(set<pair<int, string>> &population, double numParentsRates, int popSize, double crossRate){

    int numParents = numParentsRates * popSize;

    vector<double> probabilities = calculateProbabilities(population);

    vector<double> cumulative(probabilities.size(), 0.0);
    cumulative[0] = probabilities[0];
    for(int i = 1; i < probabilities.size(); i++){
        cumulative[i] = cumulative[i - 1] + probabilities[i];
    }

    vector<string> individuals;
    for(const pair<int, string> &indiv : population){
        individuals.push_back(indiv.second);
    }

    uniform_real_distribution<double>dis(0, 1);

    vector<string> parents;
    for(int i = 0; i < numParents; i++){
        double randomNum = dis(generator);

        for(int j = 0; j < cumulative.size(); j++){
            if(randomNum <= cumulative[j]){
                parents.push_back(individuals[j]);
                break;
            }
        }
    }

    vector<pair<string, string>> pairs = formPairs(parents, crossRate);

    return pairs;
}

vector<string> crossFunction(vector<pair<string, string>> &pairs, int lenghtSolution){
    uniform_int_distribution<int> dis(1, lenghtSolution - 1);

    vector<string> offspring;
    for(const pair<string, string> &pair : pairs){
        int index = dis(generator);
        
        string child1 = pair.first.substr(0, index) + pair.second.substr(index);
        string child2 = pair.second.substr(0, index) + pair.first.substr(index);

        offspring.push_back(child1);
        offspring.push_back(child2);
    }

    return offspring;
}

void mutation(vector<string> &offspring, double mutationRate, const vector<int> &weights, int capacity) {
    uniform_real_distribution<double> disMutation(0, 1);
    for (string &child : offspring) {
        for (int j = 0; j < child.size(); j++) {
            if (disMutation(generator) <= mutationRate) {
                // Realiza a mutação no gene
                child[j] = (child[j] == '1') ? '0' : '1';

                // Recalcula o peso da solução e ajusta se necessário
                int totalWeight = 0;
                for (int k = 0; k < child.size(); k++) {
                    if (child[k] == '1') {
                        totalWeight += weights[k];
                    }
                }

                // Reverte a mutação se exceder a capacidade
                if (totalWeight > capacity) {
                    child[j] = (child[j] == '1') ? '0' : '1';
                }
            }
        }
    }
}

set<pair<int, string>> updatePopulation(set<pair<int, string>> &population, int popSize) {
    set<pair<int, string>> newPopulation;

    // Calcular as probabilidades
    vector<double> probabilities = calculateProbabilities(population);

    // Vetores para indivíduos e valores acumulados
    vector<pair<int, string>> individuals(population.begin(), population.end());
    vector<bool> selected(individuals.size(), false);

    int selectedCount = 0;

    while (selectedCount < popSize) {
        vector<double> cumulative(probabilities.size(), 0.0);
        cumulative[0] = probabilities[0];

        for (int i = 1; i < cumulative.size(); i++) {
            cumulative[i] = cumulative[i - 1] + probabilities[i];
        }

        uniform_real_distribution<double> dis(0, 1);
        double random = dis(generator);

        for (int k = 0; k < cumulative.size(); k++) {
            if (random <= cumulative[k] && !selected[k]) {
                newPopulation.insert(individuals[k]);
                selected[k] = true;
                selectedCount++;

                // Ajustar probabilidades
                double totalProb = 0.0;
                for (int l = 0; l < probabilities.size(); l++) {
                    if (!selected[l]) {
                        totalProb += probabilities[l];
                    }
                }

                for (int l = 0; l < probabilities.size(); l++) {
                    if (!selected[l]) {
                        probabilities[l] /= totalProb;
                    } else {
                        probabilities[l] = 0.0;
                    }
                }

                break;
            }
        }
    }

    return newPopulation;
}

int main(){

    int itemCount, capacity;
    cin >> itemCount >> capacity;

    vector<int> values(itemCount), weights(itemCount);

    for(int i = 0; i < itemCount; i++){
        cin >> values[i] >> weights[i];
    }

    int popSize = itemCount;
    set<pair<int, string>> RandomInicialSolution = generateRandomInicialSolution(values, weights, capacity, popSize);
    RandomInicialSolution = avliationPopulation(RandomInicialSolution, values);

    for(auto s : RandomInicialSolution){
        cout << s.second << " -> " << s.first << endl;
    }

    vector<pair<string, string>> pairs = selectParents(RandomInicialSolution, 0.7, popSize, 0.8);
    for(auto x : pairs){
        cout << "pai1: " << x.first << " pai2: " << x.second << endl;
    }

    cout << "filhos: ";
    vector<string> s = crossFunction(pairs, itemCount);
    for(auto x: s)
        cout << x << ' ';
    cout << endl;

    cout << "novo s: ";
    mutation(s, 0.3, weights, capacity);
    for(auto x: s)
        cout << x << ' ';
    cout << endl;
    return 0;
}