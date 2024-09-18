#include <iostream>
#include <map>
int main() {
    bool exit = false;
    int a, m, mod = 0;
    std::map<int, int>was;
    std::cin >> a >> m;
    while(!exit){
        mod = a % m;
        if(mod == 0){
            std::cout << "Yes";
            exit = true;
        }
        else if(was.contains(mod)){
            std::cout << "No";
            exit = true;
        }
        was[mod] = 1;
        a += mod;
    }
}
