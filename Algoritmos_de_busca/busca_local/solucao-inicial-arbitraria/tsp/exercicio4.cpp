#include <bits/stdc++.h>
using namespace std;

#define ll long long

void read_graph_coordinates(vector<vector<double>> &graph, ll numb_of_vertices){
    vector<pair<ll, ll>> coordinates(numb_of_vertices);
    for(int i = 0; i < numb_of_vertices; i++){
        ll label;
        ll x;
        ll y;
        cin >> label >> x >> y;

        coordinates[label - 1] = make_pair(x, y);
    }

    for(int i = 0; i < numb_of_vertices; i++){
        ll x1 = coordinates[i].first;
        ll y1 = coordinates[i].second;

        for(int j = 0; j < numb_of_vertices; j++){
            ll x2 = coordinates[j].first;
            ll y2 = coordinates[j].second;

            graph[i][j] = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
        }
    }
}

double calculate_path(vector<vector<double>> &graph, vector<ll> &path){
    double total = 0;
    for(ll i = 1; i < path.size(); i++){
        total += graph[path[i - 1]][path[i]];
    }

    total += graph[path.size() - 1][path[0]];
    return total;
}

vector<vector<ll>> generate_permutations(vector<ll> &path){
    vector<vector<ll>> neighbors;
    for(ll i = 0; i < path.size(); i++){
        for(ll j = i + 1; j < path.size(); j++){
            swap(path[i], path[j]);
            neighbors.push_back(path);
            swap(path[i], path[j]);
        }
    }

    return neighbors;
}

bool path_equal(vector<ll> &previous_path, vector<ll> &current_path){
    for(int i = 0; i < previous_path.size(); i++){
        if(previous_path[i] != current_path[i])
            return false;
    }

    return true;
}

double tsp_search_local(vector<vector<double>> &graph){
    vector<ll> previous_path(graph.size());
    vector<ll> current_path(graph.size());

    for(ll i = 0; i < graph.size(); i++){
        previous_path[i] = i;
        current_path[i] = i;
    }

    double min = calculate_path(graph, current_path);
    vector<vector<ll>> neighbors = generate_permutations(current_path);

    do{
        copy(current_path.begin(), current_path.end(), previous_path.begin());
        vector<vector<ll>> neighbors_permutation = generate_permutations(current_path);
        min = calculate_path(graph, current_path);
        for(auto neighbor :  neighbors_permutation){
            double cost = calculate_path(graph, neighbor);
            if(cost < min){
                min = cost;
                copy(neighbor.begin(), neighbor.end(), current_path.begin());
                break;
            }
        }
    }while(!path_equal(previous_path, current_path));

    return min;
}


int main(){

    ll numb_of_vertices;
    cin >> numb_of_vertices;

    vector<vector<double>> graph(numb_of_vertices, vector<double>(numb_of_vertices));
    read_graph_coordinates(graph, numb_of_vertices);

    double result_tsp = tsp_search_local(graph);
    
    cout << "Custo minimo do caminho: " << result_tsp << endl;
    return 0;
}