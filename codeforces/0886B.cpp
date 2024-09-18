#include <iostream>
#include <map>
int main() {
    int a, n;
    std::map<int, int> was;
    std::cin >> n >> a;
    was[a] = 0;
    int earliest = a;
    for (int i = 1; i < n; i++) {
        std::cin >> a;
        was[a] = i;
    }
    for (auto i: was) {
        if (i.second < was[earliest]) {
            earliest = i.first;
        }
    }
    std::cout << earliest;
}
