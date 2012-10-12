#include <iostream>

#include "vector.hpp"

using namespace std;

int main()
{
    cout << "This Programm is to test the general 2d vector class." << endl;
    vector v1(1, 0);
    vector v2(1, 1);

    cout << v1.getAngle(v2) << endl;

	return 0;
}
