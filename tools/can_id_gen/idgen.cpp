
#include <iostream>

int main () {
    unsigned int value = 0x0F;
    unsigned int priority;
    unsigned int num;
    std::cout << "the base used is 0x0F --> 0b001.111*.**** " << std::endl;
    std::cout << "priority: "; std::cin >> priority;
    std::cout << "range: "; std::cin >> num;

    value = priority;
    value = (value << 5);

    for (int i = 1; i <= num; i++) {

        std::cout << i << " : " << (value | i) << std::endl;
    }

    return 0;
}
