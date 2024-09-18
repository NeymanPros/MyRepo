#include <iostream>
#include <vector>
int main() {
    int times = 0;
    int counter = 0;
    char op = '0';
    bool found = false;
    std::vector<int>f;
    std::vector<int>n;
    std::cin >> times;

    for(; times > 0; times--){
        found = false;
        counter = -1;
        f.clear();
        n.clear();
        for(int i = 1; i <= 8; i++) {
            for(int j = 1; j <= 8; j++) {

                std::cin >> op;
                if(op == '#' && !found){
                    counter++;
                    f.push_back(i);
                    n.push_back(j);

                    if(counter >= 4){
                        if(f[counter - 1] == i && f[counter - 2] == i - 1 && n[counter - 1] == j - 2 && n[counter - 2] == j - 1){
                            found = true;
                        }
                    }
                }

            }
        }
        std::cout << f[counter - 2] << " " << n[counter - 2] << "\n";

    }
}
