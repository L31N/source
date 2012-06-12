#include <iostream>

#include "vector.hpp"

using namespace std;

int main()
{
    cout << "This Programm is to test the general 2d vector class." << endl;

    vector v1;
    vector v2;

    v1.setX(1);
    v1.setY(2);

    v2 = !v1;

    v2.print();

    cout << endl;

	return 0;
}
