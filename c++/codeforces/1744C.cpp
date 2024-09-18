#include <iostream>

int main() {
    int times = 0;
    int n = 0;
    int max = 0;
    int ans = 0;
    char c;
    std::string pr;
    std::cin >> times;
    for(; times > 0; times--){

        ans = 0;
        max = 0;
        std::cin >> n;
        std::cin >> c;
        std::cin >> pr;

        if(c == 'g'){
            std::cout << "0\n";
        }

        else{

            pr = pr + pr;
            for(int i = 0; i < n; i++){

                if(pr[i] == c){
                    ans = 1;
                    for(i++; pr[i] != 'g'; i++){
                        ans++;
                    }
                }
                if(max < ans){
                    max = ans;
                }
            }
            std::cout << max << "\n";

        }

    }
}