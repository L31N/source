#include <iostream>

#include "bbvector.h"

using namespace std;

int main()
{
    cout << "This Programm is to test the general 2d vector class." << endl;
    Vector v1(0, 1);
	Vector v2(1, 0);

	cout << v1.getAngle() << endl;

    v1.setAngle(v2, 90);

    v1.print();

	return 0;
}
