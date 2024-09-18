#include <iostream>

int main() {
    int n, a, max = 0, row = 0;
    double k;
    std::cin >> n >> k;
    std::cin >> max;
    for (int i = 1; i < n && row < k; i++) {
        std::cin >> a;
        if (a > max) {
            max = a;
            row = 0;
        }
        row++;
    }
    std::cout << max;
}
