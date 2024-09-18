#include <iostream>
#include <vector>
int main() {
    int n = 0;
    int p = 0;
    bool f = false;
    std::vector<int>pres;
    std::cin >> n;
    for(int i = 0; i < n; i++){
        std::cin >> p;
        pres.push_back(p);
    }
    for(int i = 1; i <= n; i++){
        f = false;
        for(int j = 1; !f; j++){
            if(pres[j - 1] == i){
                f = true;
                std::cout << j << " ";
            }
        }
    }
}
