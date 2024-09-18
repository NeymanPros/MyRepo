#include <iostream>
#include <vector>
int main() {
    std::vector<int> lock(1,1,1,1,1);
    char op;
    int times, n, code, i, nil;
    std::cin >> times;
    for(; times > 0; times--){

        lock.clear();
        std::cin >> n;
        for(i = 0; i < n; i++){
            std::cin >> code;
            lock.push_back(code);
        }

        for(i = 0; i < n; i++){
            std::cin >> nil;
            for(; nil > 0; nil--){
                std::cin >> op;
                if(op == 'D'){
                    lock[i]++;
                    if(lock[i] == 10)
                        lock[i] = 0;
                }
                else if(op == 'U'){
                    lock[i]--;
                    if(lock[i] == -1)
                        lock[i] = 9;
                }
            }
        }

        for(i = 0; i < n; i++){
            std::cout << lock[i] << " ";
        }
    }
}