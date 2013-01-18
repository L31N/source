
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


    if (m.getAngle() > (315) || m.getAngle() <= 45) {
        g.set(1, 1);
        h.set(-1, 1);
    }
    else if (m.getAngle() > 45 && m.getAngle() <= 135) {
        g.set(1, 1);
        h.set(1, -1);
    }
    else if (m.getAngle() > 135 && m.getAngle() <= 225) {
        g.set(-1, -1);
        h.set(1, -1);
    }
    else if (m.getAngle() > 225 && m.getAngle() <= 315) {
        g.set(-1, -1);
        h.set(-1, 1);
    }
    else {
        std::cerr << "wrong vector" << std::endl;
    }

    g.setLenght(cos(m.getAngle(g, false, false)) * m.abs());
    h.setLenght(sin(m.getAngle(g, false, false)) * m.abs());

    std::cout << "Length(g): " << (int)g.abs() << std::endl;
    std::cout << "Length(h): " << (int)h.abs() << std::endl;

    std::cout << "Length(res): " << (int)sqrt((g.abs() * g.abs())+ (h.abs() * h.abs())) << std::endl;
    std::cout << "Length(m): " << (int)m.abs() << std::endl;
}
