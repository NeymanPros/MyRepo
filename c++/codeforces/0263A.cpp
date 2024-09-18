#include <iostream>

int main() {
    int a = 0, num1 = 0, num2 = 0;
    for(int i = 1; i <= 5; ++i)
        for(int j = 1; j <= 5; ++j){
            std::cin >> a;
            if(a)
                num1 = i * 10 + j;
        }
    num2 = num1%10;
    num1 /= 10;
    num1 -= 3;
    num2 -= 3;

    num1 *= num1;
    if(num1 == 4) {
        num1 = 2;
    }
    num2 *= num2;
    if(num2 == 4) {
        num2 = 2;
    }

    a = num1 + num2;

    std::cout << a;
}
