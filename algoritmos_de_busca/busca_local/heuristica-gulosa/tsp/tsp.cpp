#include <bits/stdc++.h>
using namespace std;

#define ll long long

vector<vector<double>> read_graph(vector<vector<double>> &graph, int number_of_vertices){
    vector<pair<ll, ll>> coordinates(number_of_vertices);

    ll index, x, y;
    for(int i = 0; i < number_of_vertices; i++){
        cin >> index >> x >> y;
        index--;

        coordinates[index] = make_pair(x, y);
    }

    for(int i = 0; i < number_of_vertices; i++){
        ll x1 = coordinates[i].first;
        ll y1 = coordinates[i].second;
        for(int j = 0; j < number_of_vertices; j++){
            ll x2 = coordinates[j].first;
            ll y2 = coordinates[j].second;

            graph[i][j] = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        }
    }
    return graph;
}

int lowest_value(vector<double> &values, vector<bool> &visited, int number_of_vertices){
    double lowest = -1;
    int pos;
    for(int i = 0; i < number_of_vertices; i++){
        if(!visited[values[i]]) continue;
        if(lowest == -1 || values[i] < lowest){
            lowest = values[i];
            pos = i;
        }
    }

    return pos;
}

vector<int> nearest_neighbor(vector<vector<double>> &graph, int number_of_vertices){
    vector<bool> visited(number_of_vertices, false);
    vector<int> path;

    int first_vertice = 0;
    visited[first_vertice] = true;
    path.push_back(first_vertice);

    while(1){
        int current_vertice = path.back();
        int lowest = -1;
        int pos = -1;

        for(int i = 0; i < number_of_vertices; i++){
            if(visited[i])
                continue;
            
            if(lowest == -1 || graph[current_vertice][i] < lowest){
                lowest = graph[current_vertice][i];
                pos = i;
            }
        }
        
        if(pos == - 1)
            break;

        path.push_back(pos);
        visited[pos] = true;
    }

    return path;
}

vector<int> farthest_neighbor(vector<vector<double>> &graph, int number_of_vertices){
    vector<int> visited(number_of_vertices, false);
    vector<int> path;

    int first_vertice = 0;
    visited[first_vertice] = true;
    path.push_back(first_vertice);

    while(1){
        int current_vertice = path.back();
        int larger = -1;
        int pos = -1;

        for(int i = 0; i < number_of_vertices; i++){
            if(visited[i])  
                continue;

            if(larger == - 1 || graph[current_vertice][i] > larger){
                larger = graph[current_vertice][i];
                pos = i;
            }
        }

        if(pos == -1)
            break;

        path.push_back(pos);
        visited[pos] = true;
    }

    return path;
}

double calculate_path(vector<vector<double>> &graph, vector<int> &path){
    double total = 0;
    for(int i = 1; i < path.size(); i++){
        total += graph[path[i - 1]][path[i]];
    }
    total += graph[path.size() - 1][path[0]];
    return total;
}

vector<vector<int>> generate_permutations(vector<int> &path){
    vector<vector<int>> neighbors;
    for(int i = 0; i < path.size(); i++){
        for(int j = i + 1; j < path.size(); j++){
            swap(path[i], path[j]);
            neighbors.push_back(path);
            swap(path[i], path[j]);
        }
    }

    return neighbors;
}

bool path_equal(vector<int> &previous_path, vector<int> &current_path){
    for(int i = 0; i < previous_path.size(); i++){
        if(previous_path[i] != current_path[i])
            return false;
    }

    return true;
}

double tsp_search_local(vector<vector<double>> &graph, vector<int> &current_path){
    vector<int> previous_path(graph.size());
    double min;
    do{
        copy(current_path.begin(), current_path.end(), previous_path.begin());
        vector<vector<int>> neighbors_permutation = generate_permutations(current_path);
        min = calculate_path(graph, current_path);
        for(auto neighbor :  neighbors_permutation){
            double cost = calculate_path(graph, neighbor);
            if(cost < min){
                min = cost;
                copy(neighbor.begin(), neighbor.end(), current_path.begin());
            }
        }
    }while(!path_equal(previous_path, current_path));

    return min;
}

int main(){

    int number_of_vertices;
    cin >> number_of_vertices;

    vector<vector<double>>graph(number_of_vertices, vector<double>(number_of_vertices));
    read_graph(graph, number_of_vertices);

    vector<int> path_nearest = nearest_neighbor(graph, number_of_vertices);
    vector<int> path_larger = farthest_neighbor(graph, number_of_vertices);

    double result_tsp_nearest = tsp_search_local(graph, path_nearest);
    cout << "Visinhos mais proximos: " << result_tsp_nearest << endl;

    double result_tsp_larger = tsp_search_local(graph, path_larger);
    cout << "Visinhos mais distantes: " << result_tsp_larger << endl;
    
    return 0;
}