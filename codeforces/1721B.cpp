#include <iostream>
int main() {

    int times, n, m, x, y, d;
    bool w, h;
    std::cin >> times;
    for(int i = 0; i < times; i++){

        w = true;
        h = true;
        std::cin >> n >> m >> x >> y >> d;

        if(x - d <= 1 && x + d >= n)
            h = false;

        if(y - d <= 1 && y + d >= m)
            w = false;

        if(w && h)
            std::cout << n + m - 2 << std::endl;

        else
            std::cout << "-1\n";

    }
}