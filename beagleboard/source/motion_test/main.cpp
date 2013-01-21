
#include <iostream>
#include <cmath>

#include "bbvector.h"

int main () {
    double x,y;

    std::cout << "X: ";
    std::cin >> x;
    std::cout << "Y: ";
    std::cin >> y;

    Vector m(x,y);

    Vector g;
    Vector h;

    double length_g, length_h;
    length_g = g.abs();
    length_h = h.abs();


    if (m.getAngle() > (315) || m.getAngle() <= 45) {
        g.set(1, 1);
        h.set(-1, 1);
        length_g = 1;
        length_h = 1;
    }
    else if (m.getAngle() > 45 && m.getAngle() <= 135) {
        g.set(1, 1);
        h.set(1, -1);
        length_g = 1;
        length_h = -1;
    }
    else if (m.getAngle() > 135 && m.getAngle() <= 225) {
        g.set(-1, -1);
        h.set(1, -1);
        length_g = -1;
        length_h = -1;
    }
    else if (m.getAngle() > 225 && m.getAngle() <= 315) {
        g.set(-1, -1);
        h.set(-1, 1);
        length_g = -1;
        length_h = 1;
    }
    else {
        std::cerr << "wrong vector" << std::endl;
    }

    g.setLenght(cos(abs(m.getAngle(g, false, false))) * m.abs());
    h.setLenght(sin(abs(m.getAngle(g, false, false))) * m.abs());


    length_g *= g.abs();
    length_h *= h.abs();

    std::cout << "Length(g): " << (int)length_g << std::endl;
    std::cout << "Length(h): " << (int)length_h << std::endl;

    std::cout << "Length(res): " << (int)sqrt((g.abs() * g.abs())+ (h.abs() * h.abs())) << std::endl;
    std::cout << "Length(m): " << (int)m.abs() << std::endl;
}
