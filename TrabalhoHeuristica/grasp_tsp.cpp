#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'
#define GRASPmax 100

// Gerador de números aleatórios
random_device rd;
mt19937 generator(rd());

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

vector<ll> construcaoGulosaAleatoria(vector<vector<double>> &graph, int vertexCount){
    vector<ll>solucao_parcial;
    solucao_parcial.push_back(0);

    set<ll>lista_candidatos;
    for(int i = 1; i < vertexCount; i++){
        lista_candidatos.insert(i);
    }

    ll ultima_cidade; 
    double c_min, c_max;
    double limite, alpha = 0.1;

    while(!lista_candidatos.empty()){
        vector<pair<double, int>> funcao_gulosa;
        vector<ll> lista_melhores_candidatos;
        ultima_cidade = solucao_parcial.back();
        
        for(auto x1 : lista_candidatos){
            funcao_gulosa.push_back(make_pair(graph[ultima_cidade][x1], x1));
        }

        sort(funcao_gulosa.begin(), funcao_gulosa.end());

        c_min = funcao_gulosa[0].first;
        c_max = funcao_gulosa[funcao_gulosa.size() - 1].first;

        limite = c_min + (alpha * (c_max - c_min));
        for(auto cidade_candidata : lista_candidatos){
            for(auto x : funcao_gulosa){
                if(x.second == cidade_candidata){
                    if(x.first <= limite){
                        lista_melhores_candidatos.push_back(cidade_candidata);
                    }
                }
            }
        }
    
        uniform_int_distribution<int> distribution(0, lista_melhores_candidatos.size() - 1);
        int index = distribution(generator);

        int valor = lista_melhores_candidatos[index];
        solucao_parcial.push_back(valor);
        lista_candidatos.erase(valor);
    }

    return solucao_parcial;
}

double calculate_path(vector<vector<double>> &graph, vector<ll> &path){
    double total = 0;
    for(int i = 1; i < path.size(); i++){
        total += graph[path[i - 1]][path[i]];
    }
    total += graph[path.size() - 1][path[0]];
    return total;
}

vector<vector<ll>> generate_permutations(vector<ll> &path){
    vector<vector<ll>> neighbors;
    for(int i = 0; i < path.size(); i++){
        for(int j = i + 1; j < path.size(); j++){
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

double tsp_search_local(vector<vector<double>> &graph, vector<ll> &current_path){
    vector<ll> previous_path(graph.size());
    double min;
    do{
        copy(current_path.begin(), current_path.end(), previous_path.begin());
        vector<vector<ll>> neighbors_permutation = generate_permutations(current_path);
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

double grasp(vector<vector<double>> &graph, int vertexCount){
    vector<ll> s1 = construcaoGulosaAleatoria(graph, vertexCount);
    double s_best = calculate_path(graph, s1);
    int iter = 0;
    while(iter < GRASPmax){
        s1 = construcaoGulosaAleatoria(graph, vertexCount);
        double s_corrente = tsp_search_local(graph, s1);
        if(s_corrente < s_best){
            s_best = s_corrente;
        }
        iter++;
    }

    return s_best;
}

int main(){
    int vertexCount;
    cin >> vertexCount;

    //Inicializa a matriz de adjacência
    vector<vector<double>> graph(vertexCount, vector<double>(vertexCount));
    readGraph(graph, vertexCount);

    double result = grasp(graph, vertexCount);
    cout << result << endl;
    return 0;
}