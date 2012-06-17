#include <iostream>

#include "vector.hpp"

using namespace std;

int main()
{
    cout << "This Programm is to test the general 2d vector class." << endl;

    vector v1(0, 1);
    vector v2(1, 0);

    cout << v1.getAngle(v2) << endl;

	return 0;
}
