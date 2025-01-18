#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'
const double ERROR_THRESHOLD = 1e-2;

random_device rd;
mt19937 generator(rd());

string generate_neighbor(string knapsack, vector<ll> &profits, vector<ll> weights, ll capacity){
    map<int, int> swap = {{'1', '0'}, {'0', '1'}};
    uniform_int_distribution<int>dis(0, knapsack.size() - 1);

    bool finded_neighbor;
    do{
        finded_neighbor = true;
        
        ll total_value = 0;
        ll total_weight = 0;

        int index = dis(generator);
        knapsack[index] = swap[knapsack[index]];

        for(int i = 0; i < profits.size(); i++){
            if(knapsack[i] == '1'){
                total_value += profits[i];
                total_weight += weights[i];

                if(total_weight > capacity){
                    knapsack[index] = swap[knapsack[index]];
                    finded_neighbor = false;
                    break;
                }
            }
        }  
    }while(!finded_neighbor);

    return knapsack;
}

ll evaluate_configuration(string config, vector<ll> &profits, vector<ll> &weights, ll capacity){
    ll total_value = 0;
    ll total_weight = 0;

    for(int i = 0; i < profits.size(); i++){
        if(config[i] == '1'){
            total_value += profits[i];
            total_weight += weights[i];
        }
    }

    return total_value;
}

ll simulated_annealing(string inicial_solution, vector<ll> &profits, vector<ll> &weights, ll capacity){
    double temperature = 1000;
    double coolingRate = 0.999;
    
    string current_solution = inicial_solution;
    string best_solution = inicial_solution;

    ll iteration = 0;
    while(temperature > ERROR_THRESHOLD){
        ll maxIterations = 100;
        while(iteration < maxIterations){
            string neighbor_solution = generate_neighbor(current_solution, profits, weights, capacity);

            ll current_cost = evaluate_configuration(current_solution, profits, weights, capacity);
            ll neighbor_cost = evaluate_configuration(neighbor_solution, profits, weights, capacity);

            cout << current_cost << endl;
            if(neighbor_cost > current_cost){
                current_solution = neighbor_solution;
                if(neighbor_cost > evaluate_configuration(best_solution, profits, weights, capacity)){
                    best_solution = neighbor_solution;
                }
            } else {
                double acceptanceProbability = exp((neighbor_cost - current_cost) / temperature);
                uniform_real_distribution<>dis(0.0, 1.0);
                if(dis(generator) < acceptanceProbability){
                    current_solution = neighbor_solution;
                }
            }

            iteration++;
        }

        temperature *= coolingRate;
        iteration = 0;
    }

    return evaluate_configuration(best_solution, profits, weights, capacity);
}

int main(){

    ll number_of_itens, capacity;
    cin >> number_of_itens >> capacity;

    vector<ll> profits(number_of_itens), weights(number_of_itens);
    for(int i = 0; i < number_of_itens; i++){
        cin >> profits[i] >> weights[i];
    }

    string inicial_solution(number_of_itens, '0');
    
    ll optimal_cost = simulated_annealing(inicial_solution, profits, weights, capacity);
    cout << optimal_cost << endl;

    return 0;
}