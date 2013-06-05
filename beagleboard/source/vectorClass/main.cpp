#include <iostream>
#include <string>

#include "bbvector.h"

using namespace std;

int main()
{
    std::cout << "sizeof(Vector): " << sizeof(Vector) << std::endl;

    /*cout << "This Programm is to test the general 2d vector class." << endl;
    Vector v1(0, 1);
	Vector v2(1, 0);

	cout << v1.getAngle() << endl;

    v1.setAngle(v2, 90);

    v1.print();*/

    /*Vector vector('A', 'B');
    std::string str(vector);
    char* carray = new char[sizeof(Vector)];
    carray = (char*)str.c_str();

    for(unsigned int i = 0; i < sizeof(Vector); i++) {
        cout << i << ": " << hex << (short)carray[i] << endl;
    }

    delete carray;*/

	return 0;
}
