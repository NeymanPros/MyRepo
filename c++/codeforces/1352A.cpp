#include <iostream>

int main() {
    int a=0, a1=0, i=0, j=0;
    std::cin >> i;
    for(; i > 0; --i){
        j = 0;
        std::cin >> a;
        a1 = a;
        for(int s = 10000; s > 0; s /= 10) {
            if (a1 >= s) {
                j++;
                a1 %= s;
            }
        }
        std::cout << j << std::endl;
        for(int s = 10000; s > 0; s /= 10) {
            if (a >= s) {
                j++;
                std::cout << a / s * s << " ";
                a %= s;
            }
        }
        std::cout << std::endl;
    }
}