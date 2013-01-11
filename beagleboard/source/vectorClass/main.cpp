#include <iostream>

#include "bbvector.h"

using namespace std;

int main()
{
    cout << "This Programm is to test the general 2d vector class." << endl;
    Vector v1(1, 0);

    v1.setAngle(90);

    v1.print();

	return 0;
}
