#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'
#define GRASP_MAX 100

// Random number generator
random_device rd;
mt19937 generator(rd());

/**
 * Generate all neighbor configurations by flipping each bit in the current configuration.
 */
vector<string> generateAllNeighborConfigurations(const string &config) {
    map<char, char> flip = {{'1', '0'}, {'0', '1'}};
    vector<string> neighbors;

    for (ll i = 0; i < config.size(); i++) {
        string neighbor = config;
        neighbor[i] = flip[neighbor[i]];
        neighbors.push_back(neighbor);
    }

    return neighbors;
}

/**
 * Evaluate a given configuration by summing the profits of selected items.
 * If the total weight exceeds the capacity, return 0 as the configuration is invalid.
 */
ll evaluateConfiguration(const string &config, const vector<ll> &profits, const vector<ll> &weights, ll capacity) {
    ll total_value = 0;
    ll total_weight = 0;

    for (size_t i = 0; i < config.size(); i++) {
        if (config[i] == '1') {
            total_value += profits[i];
            total_weight += weights[i];
        }
    }

    return (total_weight <= capacity) ? total_value : 0;
}

/**
 * GRASP construction phase: Greedy randomized algorithm for the knapsack problem.
 */
string greedyRandomizedConstruction(const vector<ll> &profits, const vector<ll> &weights, ll num_items, ll capacity) {
    string solution(num_items, '0');
    ll remaining_capacity = capacity;
    double alpha = 0.1;

    set<ll> candidate_set;
    for (int i = 0; i < num_items; i++) {
        if (weights[i] <= remaining_capacity) {
            candidate_set.insert(i);
        }
    }

    while (!candidate_set.empty()) {
        // Compute greedy function: profit-to-weight ratio
        vector<pair<double, ll>> greedy_function;
        for (auto item : candidate_set) {
            greedy_function.push_back({(double)profits[item] / weights[item], item});
        }

        // Sort greedy function in descending order
        sort(greedy_function.rbegin(), greedy_function.rend());

        // Define RCL (Restricted Candidate List) threshold
        double c_max = greedy_function[0].first;
        double c_min = greedy_function.back().first;
        double threshold = c_min + alpha * (c_max - c_min);

        // Build the RCL
        vector<ll> restricted_candidates;
        for (auto &item : greedy_function) {
            if (item.first >= threshold) {
                restricted_candidates.push_back(item.second);
            }
        }

        // Check if RCL is empty (edge case)
        if (restricted_candidates.empty()) {
            break;
        }

        // Select a random item from the RCL
        uniform_int_distribution<int> dist(0, restricted_candidates.size() - 1);
        int selected = restricted_candidates[dist(generator)];

        // Update solution and remaining capacity
        solution[selected] = '1';
        remaining_capacity -= weights[selected];
        candidate_set.erase(selected);

        // Remove candidates that no longer fit
        for (auto it = candidate_set.begin(); it != candidate_set.end();) {
            if (weights[*it] > remaining_capacity) {
                it = candidate_set.erase(it);
            } else {
                ++it;
            }
        }
    }

    return solution;
}

/**
 * Local search for improving a given configuration using the neighborhood strategy.
 */
ll knapsackLocalSearch(const vector<ll> &profits, const vector<ll> &weights, string current_config, ll capacity) {
    ll current_value = evaluateConfiguration(current_config, profits, weights, capacity);
    string aux_config;

    do {
        aux_config = current_config;
        vector<string> neighbors = generateAllNeighborConfigurations(current_config);

        for (const auto &neighbor : neighbors) {
            ll neighbor_value = evaluateConfiguration(neighbor, profits, weights, capacity);
            if (neighbor_value > current_value) {
                current_value = neighbor_value;
                current_config = neighbor;
            }
        }
    } while (aux_config != current_config);

    return current_value;
}

/**
 * GRASP metaheuristic: Combines construction and local search phases.
 */
ll grasp(const vector<ll> &profits, const vector<ll> &weights, ll num_items, ll capacity) {
    string greedy_solution = greedyRandomizedConstruction(profits, weights, num_items, capacity);
    ll best_solution = evaluateConfiguration(greedy_solution, profits, weights, capacity);

    for (int iteration = 0; iteration < GRASP_MAX; iteration++) {
        string current_solution = greedyRandomizedConstruction(profits, weights, num_items, capacity);
        ll local_search_solution = knapsackLocalSearch(profits, weights, current_solution, capacity);

        // Update best solution if a better one is found
        if (local_search_solution > best_solution) {
            best_solution = local_search_solution;
        }
    }

    return best_solution;
}

int main() {
    ll num_items, capacity;
    cin >> num_items >> capacity;

    vector<ll> profits(num_items), weights(num_items);
    for (int i = 0; i < num_items; i++) {
        cin >> profits[i] >> weights[i];
    }

    cout << grasp(profits, weights, num_items, capacity) << endl;
    return 0;
}
