#include <iostream>

#include "vector.hpp"

using namespace std;

int main()
{
    cout << "This Programm is to test the general 2d vector class." << endl;

    vector v1(1, 2);
    vector v2(2, 2);

    cout << (v1==v2);

    return 0;
}
