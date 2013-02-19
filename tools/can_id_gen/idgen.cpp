
#include <iostream>

int main () {
    unsigned int value;
    unsigned int num;
    std::cout << "base: "; std::cin >> value;
    std::cout << "num: "; std::cin >> num;

    value = (value << 5);

    for (int i = 1; i <= num; i++) {

        std::cout << i << " : " << (value | i) << std::endl;
    }

    return 0;
}
