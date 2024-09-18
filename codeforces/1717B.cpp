#include <iostream>
int main() {
    int times, n, k, r, c;
    std::cin >> times;
    for (; times > 0; times--) {
        std::cin >> n >> k >> r >> c;

        for (int i = 1; i <= n; i++) {      //строки
            for (int j = 1; j <= n; j++) {  //столбы

                if ((k + (i - j)%k)%k == (k + (r + c)%k)%k) {//(i - j) == 0 -> (i - j)%k == 0 -> (i - j)%k == (r + c) -> (i - j)%k == (r + c)%k
                    std::cout << 'X';
                }
                else {
                    std::cout << '.';
                }
            }
            std::cout << std::endl;
        }
    }
}