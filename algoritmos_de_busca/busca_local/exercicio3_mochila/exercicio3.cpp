#include <bits/stdc++.h>
using namespace std;

#define ll long long

vector<string> generate_all_neighbors_configuration(string config){
    map<char, char> swap = {{'1', '0'}, {'0', '1'}};
    vector<string>neighbors;
    for(ll i = 0; i < config.size(); i++){
        config[i] = swap[config[i]];
        neighbors.push_back(config);
        config[i] = swap[config[i]];
    }

    return neighbors;
}

ll evaluate_configuration(string config_bi, vector<ll> &profits, vector<ll> &weights, ll capacity){
    ll total_value = 0;
    ll total_weight = 0;

    for(ll i = 0; i < profits.size(); i++){
        if(config_bi[i] == '1'){
            total_value += profits[i];
            total_weight += weights[i];
            if(total_weight > capacity) return -1;
        }
    }

    return total_value;
}

ll knapsack_local_search(vector<ll> &profits, vector<ll>weights, ll capacity, ll number_of_intens){
    string current_config(number_of_intens, '0');
    ll current_value = 0;
    string aux_config;

    do{
        aux_config = current_config;
        vector<string> neighbors = generate_all_neighbors_configuration(current_config);

        for(auto neighbor : neighbors){
            ll value = evaluate_configuration(neighbor, profits, weights, capacity);
            if(value > current_value){
                current_value = value;
                current_config = neighbor;
                break;
            }
        }
    }while(aux_config != current_config);

    return current_value;
}

int main(){

    ll number_of_intens, capacity;
    cin >> number_of_intens >> capacity;

    vector<ll>profits(number_of_intens);
    vector<ll>weights(number_of_intens);
    
    for(int i = 0; i < number_of_intens; i++){
        cin >> profits[i] >> weights[i];
    }

    cout << "Maior capacidade: ";
    cout << knapsack_local_search(profits, weights, capacity, number_of_intens) << endl;
    return 0;
}