#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'
const double erro = 1e-2;

random_device rd;
mt19937 generator(rd());

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

vector<int> choose_neighbor(vector<int> &path){
    uniform_int_distribution<int> distribution(0, path.size() - 1);
    int i, j; 
    do{
        i = distribution(generator);
        j = distribution(generator);
    }while(i == j);

    vector<int> result = path;
    swap(result[i], result[j]);

    return result;
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

double calculate_path(vector<vector<double>> &graph, vector<int> &path){
    double total = 0;
    for(int i = 1; i < path.size(); i++){
        total += graph[path[i-1]][path[i]];
    }

    total += graph[path.size() - 1][0];
    return total;
}

double random_number(){
    srand(time(nullptr));
    return static_cast<double>(rand()) / RAND_MAX;
}

double simulated_annealing(vector<vector<double>> &graph, vector<int> &inicial_solution){
    double T = 100000;
    double alpha = 0.999;
    vector<int> s = inicial_solution;
    vector<int> best_solution = inicial_solution;

    vector<int> s_linha;
    vector<vector<int>> neighbors;

    ll i = 0;
    while(T > erro){
        ll sa_max = 100;
        while(i < sa_max){
            s_linha = choose_neighbor(s);

            double value_path_s = calculate_path(graph, s);
            double value_path_s_linha = calculate_path(graph, s_linha);
            
            if(value_path_s_linha < value_path_s){
                s = s_linha;
                if(value_path_s_linha < calculate_path(graph, best_solution)){
                    best_solution = s_linha;
                }
            }else{

                double fsL = value_path_s_linha;
                double fs = value_path_s;
                double taxa = (fs - fsL) / T;

                uniform_real_distribution<> dis(0.0, 1.0);
                double r = dis(generator);
                if(r < exp(taxa)){
                    s = s_linha;
                }
            }

            i++;
        }
        
        T *= alpha;
        i = 0;
    }

    return calculate_path(graph, best_solution);
}

int main(){

    int number_of_vertices;
    cin >> number_of_vertices;

    vector<vector<double>> graph(number_of_vertices, vector<double>(number_of_vertices));
    read_graph(graph, number_of_vertices);
    
    vector<int> inicial_solution = nearest_neighbor(graph, number_of_vertices);
    
    double result_simulated_annealing = simulated_annealing(graph, inicial_solution);
    cout << result_simulated_annealing << endl;
    
    return 0;
}