#include <iostream>

int main() {
    int times = 0, n = 0, k = 0, row = 0, max = -1, now = 0;
    bool w = false;
    std::cin >> times;
    std::string line;
    for(; times > 0; times--){

        max = -1;
        std::cin >> n >> k >> line;

        for(int i = k - 1; i <= n; i++){

            row = 0;
            now = 0;
            w = false;

            for(int j = i - k; j < i; j++){
                if(line[j] == 'B'){
                    now++;
                    if(!w) {
                        row++;
                    }
                }
                else {
                    w = true;
                }
            }

            if(row == k){
                i = n + 1;
            }
            if(max < now){
                max = now;
            }
            i += row;

        }

        std::cout << k - max << "\n";

    }
}
