#include <iostream>
#include <string>
int main() {
    int times = 0, ans = 0;
    std::string n;
    std::cin >> times;
    for(; times > 0; times--){

        ans = 0;
        std::cin >> n;

        for(int i = 0; i < n.length(); i++){
            if(n[i] == '1'){
                ans = 1;
            }

            else if(n[i] == '?'){
                ans++;
            }

            else if(n[i] == '0') {
                ans++;
                i = n.length();
            }

        }
        std::cout << ans << "\n";

    }
}
