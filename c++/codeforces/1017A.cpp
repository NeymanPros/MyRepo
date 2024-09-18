#include <iostream>

int main() {
    int i = 0;
    int a = 0;
    int four = 0;
    int sum = 0;
    int s = 0;
    int ans = 1;
    std::cin >> i;
    for(int j = 0; j < i; j++){
        sum = 0;
        for(four = 4; four > 0; four--) {
            std::cin >> a;
            sum += a;
        }
        if(j == 0){
            s = sum;
        }
        if(sum > s){
            ans++;
        }

    }
    std::cout << ans;
}