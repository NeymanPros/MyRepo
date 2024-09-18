#include <iostream>
#include <vector>
int main() {
    int a = 0, b = 0, c = 0, j = -1, i3 = 1, i = 0;
    std::vector<int>a3;
    std::vector<int>b3;
    std::vector<int>c3;
    std::cin >> a >> c;
    for(j = 0; a > 0 || c > 0; j++){
        a3.push_back(a%3);
        a /= 3;
        c3.push_back(c%3);
        c /= 3;
    }

    for(i = 0; i < j; i++){
        b3.push_back(c3[i] - a3[i]);
        if(b3[i] < 0)
            b3[i] += 3;
        if(b3[i] >= 3)
            b3[i] -= 3;
    }

    for(i = 0; i < j; i++){
        b += b3[i]*i3;
        i3*=3;
    }
    std::cout << b;
}