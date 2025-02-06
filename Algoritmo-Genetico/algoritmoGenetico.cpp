#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

random_device rd;
mt19937 generator(rd());

int comp (pair<string, int> &a, pair<string, int> &b){
    return (b.second < a.second);
}

vector<pair<string, int>> generateRandomInicialSolution(vector<int> &values, vector<int> &weights, int capacity, int popSize){
    vector<pair<string, int>> pop;
    set<string> uniqueSolutions;
    uniform_real_distribution<double>dis(0, 1);

    while(pop.size() < popSize){
        int totalCapacity = 0;
        string inicialSolution(values.size(), '0');
        // Percorre o gene de cada individuo e o escolhe como 0 e 1 de forma aleatória
        for(int i = 0; i < inicialSolution.size(); i++){
            if(dis(generator) < 0.5 && totalCapacity + weights[i] <= capacity){
                inicialSolution[i] = '1';
                totalCapacity += weights[i];
            }
        }
        // Permite não gerar soluções repetidas
        if(!uniqueSolutions.count(inicialSolution)){
            pop.push_back({inicialSolution, 0});
            uniqueSolutions.insert(inicialSolution);
        }
    }

    return pop;
}

int aptidaoFunciton(string s, vector<int> &values, vector<int> &weights, int capacity){
    int totalValues = 0;
    int totalCapacity = 0;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == '1'){
            totalValues += values[i];
            totalCapacity += weights[i];
        }
    }

    if(totalCapacity > capacity)
        totalValues *= 0.1;

    return totalValues;
}

void avliationPopulation(vector<pair<string, int>> &population, vector<int> &values, vector<int> &weights, int capacity){
    for(auto &x : population){
        x.second = aptidaoFunciton(x.first, values, weights, capacity);
    }
}

vector<pair<int, double>> calculateProbabilities(vector<pair<string, int>> &population){
    vector<pair<int, double>> probabilities;
    double totalFitness = 0.0;

    for(const pair<string, int> &individual : population){
        totalFitness += individual.second;
    }

    for(int i = 0; i < population.size(); i++){
        probabilities.push_back({i, population[i].second / totalFitness});
    }

    return probabilities;
}

int rouletteMethod(vector<pair<int, double>> &probabilities){
    // Construção da roleta
    vector<pair<int, double>> roullet(probabilities.size());
    roullet[0].first = probabilities[0].first;
    roullet[0].second = probabilities[0].second;
    for(int i = 1; i < probabilities.size(); i++){
        roullet[i].first = probabilities[i].first;
        roullet[i].second = roullet[i - 1].second + probabilities[i].second;
    }

    // Execução da roleta
    uniform_real_distribution<double> dis(0, 1);
    double random = dis(generator);
    int index = 0;
    for(int i = 0; i < roullet.size(); i++){
        if(random <= roullet[i].second){
            index = roullet[i].first;
            break;
        }
    }

    return index;
}

vector<pair<string, string>> formPairs(vector<string> &parents, double crossRate){
    uniform_real_distribution<double> dis(0, 1);

    vector<pair<string, string>> pairs;
    for(int i = 0; i < parents.size(); i+=2){
        if(i + 1 < parents.size() && dis(generator) <= crossRate){
            pairs.push_back({parents[i], parents[i+1]});
        }
    }

    return pairs;
}

vector<pair<string, string>> selectParents(vector<pair<string, int>> &population, double rateParents, double crossRate, int popSize){
    //Definição da quantidade de pais: A quantidade tem que ser par.
    int numParents = rateParents * popSize;
    if(numParents % 2 == 1) numParents--;

    // Probabilidade de cada individuo ser um pai
    vector<pair<int, double>> probabilities = calculateProbabilities(population);
    vector<int> selected(probabilities.size(), false); // Usado para controlar qual pai foi escolhido
    
    vector<string> potencialParents(numParents);// Pais que serão utilizados para formar pares.
    for(int i = 0; i < numParents; i++){
        // Seleciona o pai.        
        int indexParent = rouletteMethod(probabilities);
        potencialParents[i] = population[indexParent].first;
        selected[indexParent] = true;

        /*
            Escolhido o pai, é necessário descartá-lo da próxima escolha e refazer a distribuição
            de probabilidades.
        */
        double totalProb = 0.0;
        for(int i = 0; i < probabilities.size(); i++){
            if(!selected[probabilities[i].first]){
                totalProb += probabilities[i].second;
            }
        }

        if(totalProb == 0)
            break;

        vector<pair<int, double>> auxProb;
        for(int i = 0; i < probabilities.size(); i++){
            if(!selected[probabilities[i].first]){
                auxProb.push_back({probabilities[i].first, probabilities[i].second / totalProb});
            }
        }

        probabilities = auxProb;
    }

    vector<pair<string, string>> pairs = formPairs(potencialParents, crossRate);
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

void mutation(vector<string> &offspring, double mutationRate, vector<int> &weights, int capacity) {
    uniform_real_distribution<double> disMutation(0, 1);
    for (string &child : offspring) {
        for (int j = 0; j < child.size(); j++) {
            if (disMutation(generator) <= mutationRate) {
                child[j] = (child[j] == '1') ? '0' : '1';
            }
        }
    }
}

vector<pair<string, int>> updatePopulation(vector<pair<string, int>> &population, int popSize) {
    vector<pair<string, int>> newPopulation;

    vector<pair<int, double>> probabilities = calculateProbabilities(population);
    vector<bool> selected(probabilities.size(), false);

    for(int i = 0; i < popSize; i++){
        int indexIndivuo = rouletteMethod(probabilities);
        newPopulation.push_back(population[indexIndivuo]);
        selected[indexIndivuo] = true;

        double totalProb = 0.0;
        for(int i = 0; i < probabilities.size(); i++){
            if(!selected[probabilities[i].first]){
                totalProb += probabilities[i].second;
            }
        }

        if(totalProb == 0)
            break;

        vector<pair<int, double>> auxProb;
        for(int i = 0; i < probabilities.size(); i++){
            if(!selected[probabilities[i].first]){
                auxProb.push_back({probabilities[i].first, probabilities[i].second / totalProb});
            }
        }

        probabilities = auxProb;
    }

    return newPopulation;
}

int GeneticAlgorithm(vector<int> &values, vector<int> &weights, int itemCount, int capacity, 
                     double mutationRate, double numParentsRate, double crossRate, int popSize) {
    vector<pair<string, int>> population = generateRandomInicialSolution(values, weights, capacity, popSize);
    
    const int maxGeneration = 1000;
    int generation = 0;

    while (generation < maxGeneration) {
        avliationPopulation(population, values, weights, capacity);

        vector<pair<string, string>> parents = selectParents(population, numParentsRate, crossRate, popSize);

        vector<string> offspring = crossFunction(parents, itemCount);
        mutation(offspring, mutationRate, weights, capacity);

        for (const string &child : offspring) {
            population.push_back({child, 0});
        }
        avliationPopulation(population, values, weights, capacity);
        population = updatePopulation(population, popSize);

        generation++;
    }

    sort(population.begin(), population.end(), comp);

    return population[0].second;
}

int main() {
    int itemCount, capacity;
    cin >> itemCount >> capacity;

    vector<int> values(itemCount), weights(itemCount);
    for (int i = 0; i < itemCount; i++) {
        cin >> values[i] >> weights[i];
    }

    int result = GeneticAlgorithm(values, weights, itemCount, capacity, 0.01, 0.5, 0.9, itemCount * 2);
    cout << result << endl;
    return 0;
}