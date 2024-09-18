#include <iostream>
#include <vector>
int main() {
    int times, n, a, index = 0, fvec = 0, op = 0;
    double ans = 0;
    std::vector<int> dev;
    std::cin >> times;
    for (; times > 0; times--) {

        dev.clear();
        op = 0;
        std::cin >> n;
        ans = -n;
        dev.resize(33, 0);

        for (int i = 0; i < n; i++) {
            std::cin >> a;
            while (a % 2 == 0 && a > 0) {
                ans++;
                a /= 2;
            }

            index = i + 1;
            fvec = 0;
            while (index % 2 == 0 && index > 0) {
                index /= 2;
                fvec++;
            }
            dev[fvec]++;

        }

        for (int d = n + 1; ans < 0 && d > 0; d--) {
            while(dev[d] > 0 && ans < 0){
                ans += d;
                dev[d]--;
                op++;
            }
        }

        if (ans < 0) {
            std::cout << "-1\n";
        }
        else {
            std::cout << op << "\n";
        }

    }
}