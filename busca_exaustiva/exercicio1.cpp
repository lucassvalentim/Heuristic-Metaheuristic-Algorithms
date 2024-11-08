#include <bits/stdc++.h>
using namespace std;

void binary(int n, string *s){
    if(n == 0){
        *(s) += "0";
        return;
    }
    if(n == 1){
        *(s) += "1";
        return;
    }

    binary(n / 2, s);
    *(s) += n % 2 ? "1" : "0";
}

void all_possibilities(int n){
    long long tam = pow(2, n);
    string s;
    for(int i = 0; i < tam; i++){
        s = "";
        binary(i, &s);
        
        while(s.size() < n){
            s = "0" + s;
        }

        cout << s << endl;
    }
}

int main(){

    int n;
    cin >> n;
    
    all_possibilities(n);
    return 0;
}