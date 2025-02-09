#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

random_device rd;
mt19937 generator(rd());

int comp (pair<vector<int>, double> &a, pair<vector<int>, double> &b){
    return (b.second < a.second);
}

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

vector<pair<vector<int>, double>> generateRandomInicialSolution(vector<vector<double>> &graph, int vertexCount, int popSize){
    vector<pair<vector<int>, double>> pop;
    set<vector<int>> uniqueSolutions;
    uniform_int_distribution<int>dis(0, vertexCount - 1);

    while(pop.size() < popSize){
        vector<int> inicialSolution(vertexCount);
        vector<bool> sorted(vertexCount); // vetor que verifica se uma cidade já foi sorteada

        // Construção aleatória do indivíduo. A cada posição do indivíduo é escolhido seu gene
        for(int i = 0; i < vertexCount; i++){
            int index;
            do{
                index = dis(generator);
            }while(sorted[index]);
            inicialSolution[i] = index;
            sorted[index] = true;
        }

        // Permite não gerar soluções repetidas
        if(!uniqueSolutions.count(inicialSolution)){
            pop.push_back({inicialSolution, 0});
            uniqueSolutions.insert(inicialSolution);
        }
    }

    return pop;
}

double aptidaoFunciton(vector<vector<double>> &graph, vector<int> path){
    double totalCost = 0;
    for (int i = 1; i < path.size(); i++) {
        totalCost += graph[path[i - 1]][path[i]];
    }
    totalCost += graph[path.back()][path[0]]; // Retorna ao ponto inicial
    return totalCost;
}

void avliationPopulation(vector<vector<double>> &graph, vector<pair<vector<int>, double>> &population){
    for(auto &x : population){
        x.second = 1.0 / (1.0 + aptidaoFunciton(graph, x.first));
    }
}

vector<pair<int, double>> calculateProbabilities(vector<pair<vector<int>, double>> &population){
    vector<pair<int, double>> probabilities;
    double totalFitness = 0.0;

    for(const pair<vector<int>, double> &individual : population){
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

vector<pair<vector<int>, vector<int>>> formPairs(vector<vector<int>> &parents, double crossRate){
    uniform_real_distribution<double> dis(0, 1);

    vector<pair<vector<int>, vector<int>>> pairs;
    for(int i = 0; i < parents.size(); i+=2){
        if(i + 1 < parents.size() && dis(generator) <= crossRate){
            pairs.push_back({parents[i], parents[i+1]});
        }
    }

    return pairs;
}

vector<pair<vector<int>, vector<int>>> selectParents(vector<pair<vector<int>, double>> &population, double rateParents, double crossRate, int popSize){
    //Definição da quantidade de pais: A quantidade tem que ser par.
    int numParents = rateParents * popSize;
    if(numParents % 2 == 1) numParents--;

    // Probabilidade de cada individuo ser um pai
    vector<pair<int, double>> probabilities = calculateProbabilities(population);
    vector<int> selected(probabilities.size(), false); // Usado para controlar qual pai foi escolhido
    
    vector<vector<int>> potencialParents(numParents);// Pais que serão utilizados para formar pares.
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

    vector<pair<vector<int>, vector<int>>> pairs = formPairs(potencialParents, crossRate);
    return pairs;
}

vector<vector<int>> crossFunction(vector<pair<vector<int>, vector<int>>> &pairs, int lengthSolution) {
    vector<vector<int>> offspring;
    random_device rd;
    mt19937 generator(rd());
    
    for (auto &p : pairs) {
        vector<int> parent1 = p.first;
        vector<int> parent2 = p.second;
        vector<int> child1(lengthSolution, -1);
        vector<int> child2(lengthSolution, -1);
        
        // Escolher dois pontos de corte aleatórios
        uniform_int_distribution<int> distribution(0, lengthSolution - 1);
        int point1 = distribution(generator);
        int point2 = distribution(generator);
        if (point1 > point2) swap(point1, point2);

        set<int> genes1, genes2;
        
        // Copiar o segmento entre os pontos de corte
        for (int i = point1; i <= point2; i++) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
            genes1.insert(parent1[i]);
            genes2.insert(parent2[i]);
        }

        // Preencher os filhos com os genes restantes em ordem do outro pai
        int index1 = (point2 + 1) % lengthSolution;
        int index2 = (point2 + 1) % lengthSolution;

        for (int i = 0; i < lengthSolution; i++) {
            int pos = (point2 + 1 + i) % lengthSolution;

            if (genes1.count(parent2[pos]) == 0) {
                child1[index1] = parent2[pos];
                index1 = (index1 + 1) % lengthSolution;
            }

            if (genes2.count(parent1[pos]) == 0) {
                child2[index2] = parent1[pos];
                index2 = (index2 + 1) % lengthSolution;
            }
        }

        offspring.push_back(child1);
        offspring.push_back(child2);
    }
    
    return offspring;
}

void mutation(vector<vector<int>> &offspring, double mutationRate, int vertexCount) {
    uniform_real_distribution<double> disMutation(0, 1);
    uniform_int_distribution<int> disPostion(0, vertexCount - 1);

    for (vector<int> &child : offspring) {
        if (disMutation(generator) <= mutationRate) {
            int i;
            int j;
            do{
                i = disPostion(generator);
                j = disPostion(generator);
            }while(i == j);
            swap(child[i], child[j]);
        }
    }
}

vector<pair<vector<int>, double>> updatePopulation(vector<pair<vector<int>, double>> &population, int popSize) {
    sort(population.begin(), population.end(), comp);
    population.resize(popSize);
    return population;
}

double GeneticAlgorithm(vector<vector<double>> graph, int vertexCount, double mutationRate, double numParentsRate, double crossRate, int popSize) {
    vector<pair<vector<int>, double>> population = generateRandomInicialSolution(graph, vertexCount, popSize);
    
    const int maxGeneration = 1000;
    int generation = 0;

    while (generation < maxGeneration) {
        avliationPopulation(graph, population);

        vector<pair<vector<int>, vector<int>>> parents = selectParents(population, numParentsRate, crossRate, popSize);

        vector<vector<int>> offspring = crossFunction(parents, vertexCount);
        mutation(offspring, mutationRate, vertexCount);

        for (const vector<int> &child : offspring) {
            population.push_back({child, 0});
        }
        avliationPopulation(graph, population);
        population = updatePopulation(population, popSize);

        generation++;
    }

    sort(population.begin(), population.end(), comp);

    double resultado = aptidaoFunciton(graph, population[0].first);
    return resultado;
}

int main() {
    int vertexCount;
    cin >> vertexCount;

    // Inicializa a matriz de adjacência
    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);

    cout << GeneticAlgorithm(graph, vertexCount, 0.01, 0.5, 0.9, vertexCount * 10) << endl;
    return 0;
}
