#include <bits/stdc++.h>
using namespace std;

#define ll long long

string cost_benefit(vector<ll> &profits, vector<ll> &weights, ll number_of_itens, ll capacity){
    string current_config(number_of_itens, '0');
    vector<pair<double, int>> benefit(number_of_itens);

    for(int i = 0; i < number_of_itens; i++){
        benefit[i] = make_pair((double) weights[i] / profits[i], i);
    }

    sort(benefit.rbegin(), benefit.rend());

    ll current_capacity = 0;
    for(auto x : benefit){
        if(current_capacity + weights[x.second] <= capacity){
            current_capacity += weights[x.second];
            current_config[x.second] = '1';
        }
    }

    return current_config;
}

string lightest_item(vector<ll> &profit, vector<ll> weights, ll number_of_itens, ll capacity){
    string current_config(number_of_itens, '0');
    vector<pair<ll, int>> weights_sorts(number_of_itens);

    for(int i = 0; i < number_of_itens; i++){
        weights_sorts[i] = make_pair(weights[i], i);
    }

    sort(weights_sorts.begin(), weights_sorts.end());

    ll current_capacity = 0;
    for(auto x : weights_sorts){
        if(current_capacity + x.first <= capacity){
            current_capacity += x.first;
            current_config[x.second] = '1';
        }
    }

    return current_config;
}

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

ll knapsack_local_search(vector<ll> &profits, vector<ll>weights, string current_config, ll capacity, ll number_of_intens){
    
    ll current_value = 0;
    for(int i = 0; i < current_config.size(); i++){
        if(current_config[i] == '1')
            current_value += profits[i];
    }
    
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

    ll number_of_itens, capacity;
    cin >> number_of_itens >> capacity;

    vector<ll> profits(number_of_itens);
    vector<ll> weights(number_of_itens);

    for(ll i = 0; i < number_of_itens; i++){
        cin >> profits[i] >> weights[i];
    }

    string cost_benefit_config = cost_benefit(profits, weights, number_of_itens, capacity);
    string lightest_item_config = lightest_item(profits, weights, number_of_itens, capacity);

    ll result_cost_benefit_config = knapsack_local_search(profits, weights, cost_benefit_config, capacity, number_of_itens);
    ll result_lightest_item_config = knapsack_local_search(profits, weights, lightest_item_config, capacity, number_of_itens);

    cout << "Custo beneficio: " << result_cost_benefit_config << endl;
    cout << "Mais leve: " << result_lightest_item_config << endl;

    return 0;
}