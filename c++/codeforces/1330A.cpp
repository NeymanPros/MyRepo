#include <iostream>
#include <vector>
int main() {
    std::vector<bool> cont;
    int times, n, x, a, j;
    std::cin >> times;
    for(int i = 0; i < times; i++){

        std::cin >> n >> x;
        cont.clear();
        for(int run = 0; run <= n + x; run++)
            cont.push_back(false);

        for(j = 0; j < n; j++){
            std::cin >> a;
            cont[a] = true;
        }

        a = 0;
        for(j = 1; x != -1; j++){
            if(!cont[j] && x > 0){
                x--;
            }
            else if(!cont[j] && x == 0){
                std::cout << j - 1 << "\n";
                x = -1;
            }
        }

    }
}
