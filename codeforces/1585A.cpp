#include <iostream>

int main() {
    int times = 0;
    int n = 0;
    int h = 1;
    char op = '0';
    char op0 = '0';
    std::cin >> times;

    for(; times > 0; times--){

        h = 1;
        std::cin >> n;
        for(int i = 0; i < n; i++){
                op0 = op;
                std::cin >> op;
                if(h != -1) {
                if (op == '1') {

                    h++;
                    if (i != 0 && op0 == '1') {
                        h += 4;
                    }

                } else if (op == '0') {
                    if (i != 0 && op0 == '0') {
                        h = -1;
                    }
                }
            }
        }
        std::cout << h << "\n";

    }
}
