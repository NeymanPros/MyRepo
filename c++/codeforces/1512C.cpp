#include <iostream>
#include <vector>

int main() {
    int times, a0, a1;
    std::string s;
    std::vector<int>used;
    std::cin >> times;
    for(; times > 0; times--){

        used.clear();
        std::cin >> a0 >> a1 >> s;
        int p0 = a0, p1 = a1;
        used.resize((a0 + a1)/2);
        for(int i = 0; i <= (a0 + a1)/2; i++){
            if(s[i] == '1' && s[(a0 + a1 - 1)] == '0'){
                std::cout << -1;
                return 0;
            }
            else if(s[i] )
        }

    }
}