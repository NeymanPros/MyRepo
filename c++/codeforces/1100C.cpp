#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

int main() {
    double n, r, a;
    std::cin >> n >> r;
    std::cout.precision(10);
    std::cout << (r * sin(M_PI / n))/(1 - sin(M_PI / n));
}