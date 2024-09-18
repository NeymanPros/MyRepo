#include <iostream>

int main() {
    int t, counter = 0;
    std::cin >> t;
    while(t > 0){
        counter += (t & 1);
        t = t >> 1;
    }
    std::cout << counter;
}
