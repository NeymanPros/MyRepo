#include <iostream>

int main() {
    int times = 0, m = 0, n = 0;
    std::cin >> times;
    while(times--){

        std::cin >> m >> n;
        for(int r = 1; r <= m; r++){
            for(int h = 1; h <= n; h++){

                if(r % 4 == 1 || r % 4 == 0){
                    if(h % 4 == 1 || h % 4 == 0){
                        std::cout << "1 ";
                    }
                    else{
                        std::cout << "0 ";
                    }
                }
                else{
                    if(h % 4 == 1 || h % 4 == 0){
                        std::cout << "0 ";
                    }
                    else{
                        std::cout << "1 ";
                    }
                }

            }
            std::cout << "\n";
        }

    }
}