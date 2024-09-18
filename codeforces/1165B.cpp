#include <iostream>
#include <algorithm>
#include <vector>
int main() {
    int k = 0, n = 0, a = 0, ans = 0;
    std::vector<int> line;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> a;
        line.push_back(a);
    }
    std::sort(line.begin(), line.end());
    for (k = 0; n > k; k++) {
        if (line[k] >= ans + 1) {
            ans++;
        }
    }
    std::cout << ans;
}