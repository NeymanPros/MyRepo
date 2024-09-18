#include <iostream>

int main() {
    int k = 0, n = 0, t = 0, f = 0, max = -2000000001;
    std::cin >> n >> k;
    for(; n > 0; --n){
        std::cin >> f >> t;
        if(t > k){
            f = f - t + k;
        }
        if(max < f){
            max = f;
        }
    }
    std::cout << max;
}
