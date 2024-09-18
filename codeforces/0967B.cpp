#include <iostream>
#include <vector>
int main() {
    int n = 0;
    long long a = 0;
    long long b = 0;
    long long sum = 0;
    int first = 0;
    int x = 0;
    int s = 0;
    int i = 0;
    std::vector<int>hole;
    std::cin >> n >> a >> b >> x;
    first = x;
    s = x;
    for(i = 1; i < n; i++){
        std::cin >> x;
        hole.push_back(x);
        s += x;
    }
    std::sort(hole.rbegin(), hole.rend());
    sum = b*s;
    a = a*first;
    i = 0;
    for(; a < sum; i++){
        s -= hole[i];
        sum = b*s;
    }
    std::cout << i;
}
