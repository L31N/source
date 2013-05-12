
#include <iostream>
//#include <curses.h>

#include "bbvector.h"
#include "motion/motionController.h"
#include "motion/motor.h"

int main () {
    MotionController* mc = new MotionController;

    double x, y, rotspeed;

    std::cout << "x: " << std::flush; std::cin >> x;
    std::cout << "y: " << std::flush; std::cin >> y;
    std::cout << "rotation-speed: " << std::flush; std::cin >> rotspeed;
    std::cout << std::endl;

    sleep(1);

    mc->drive(Vector(x, y), rotspeed);
    //Motors motors("MOTORS", "MOTORS");
    //motors.setSpeed(255, 255, 255, 255);

    sleep(1);
    mc->pbreak();

    //motors.setSpeed(0, 0, 0, 0);

    //delete mc;
    return 0;
}


//
//#include <iostream>
//#include <cmath>
//
//#include "bbvector.h"
//
//short* motorSpeeds = new short[4];
//
//void drive(Vector vector, short rotationSpeed) {
//    Vector g;
//    Vector h;
//
//    double length_g, length_h;
//    length_g = g.abs();
//    length_h = h.abs();
//
//
//    if (vector.getAngle() > (315) || vector.getAngle() <= 45) {
//        g.set(1, 1);
//        h.set(-1, 1);
//        length_g = 1;
//        length_h = 1;
//    }
//    else if (vector.getAngle() > 45 && vector.getAngle() <= 135) {
//        g.set(1, 1);
//        h.set(1, -1);
//        length_g = 1;
//        length_h = -1;
//    }
//    else if (vector.getAngle() > 135 && vector.getAngle() <= 225) {
//        g.set(-1, -1);
//        h.set(1, -1);
//        length_g = -1;
//        length_h = -1;
//    }
//    else if (vector.getAngle() > 225 && vector.getAngle() <= 315) {
//        g.set(-1, -1);
//        h.set(-1, 1);
//        length_g = -1;
//        length_h = 1;
//    }
//    else {
//        std::cerr << "wrong vector" << std::endl;
//        //debug->send("MotionController::drive --> wrong Vector !!!");
//    }
//
//    g.setLenght(cos(abs(vector.getAngle(g, false, false))) * vector.abs());
//    h.setLenght(sin(abs(vector.getAngle(g, false, false))) * vector.abs());
//
//    length_g *= g.abs();
//    length_h *= h.abs();
//
//    if (length_g > 255 || length_g < (-255) || length_h > 255 || length_h < (-255)) {
//        std::cerr << "MotorController::drive --> vector length out of range !!!" << std::endl;
//        //debug->send("MotorController::drive --> vector length out of range !!!");
//
//        return; /// Error occured !
//    }
//
//    motorSpeeds[0] = length_h;
//    motorSpeeds[1] = length_g;
//    motorSpeeds[2] = length_h;
//    motorSpeeds[3] = length_g;
//
//
//    /// calculate motor-values dependend to the turn
//
//    /// check here for speed overflows and step down the values if neccessary ...
//    if (rotationSpeed < 0) {
//        std::cout << "kleiner 0" << std::endl;
//        if (motorSpeeds[0] + rotationSpeed > 255) {
//            double factor = (motorSpeeds[0] - (abs(rotationSpeed) - 255 + motorSpeeds[0])) / abs(motorSpeeds[0]);
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//        if (motorSpeeds[1] + rotationSpeed > 255) {
//            double factor = (motorSpeeds[1] - (abs(rotationSpeed) - 255 + motorSpeeds[1])) / abs(motorSpeeds[1]);
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//        if (motorSpeeds[2] - rotationSpeed < (-255)) {
//            double factor = (motorSpeeds[2] - (abs(rotationSpeed) - 255 + motorSpeeds[2])) / abs(motorSpeeds[2]);
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//        if (motorSpeeds[3] - rotationSpeed < (-255)) {
//            double factor = (motorSpeeds[3] - (abs(rotationSpeed) - 255 + motorSpeeds[3])) / abs(motorSpeeds[3]);
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//    }
//    else {
//        std::cout << "groesser 0" << std::endl;
//        if (motorSpeeds[0] - rotationSpeed < (-255)) {
//            std::cout << "motor[0] !" << std::endl;
//            double factor = (motorSpeeds[0] - (abs(rotationSpeed) - 255 + motorSpeeds[0])) / abs(motorSpeeds[0]);
//            std::cout << "factor: " << factor << std::endl;
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//        if (motorSpeeds[1] - rotationSpeed < (-255)) {
//            std::cout << "motor[1] !" << std::endl;
//            double factor = (motorSpeeds[1] - (abs(rotationSpeed) - 255 + motorSpeeds[1])) / abs(motorSpeeds[1]);
//            std::cout << "factor: " << factor << std::endl;
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//        if (motorSpeeds[2] + rotationSpeed > 255) {
//            std::cout << "motor[2] !" << std::endl;
//            double factor = (motorSpeeds[2] - (abs(rotationSpeed) - 255 + motorSpeeds[2])) / abs(motorSpeeds[2]);
//            std::cout << "factor: " << factor << std::endl;
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//        if (motorSpeeds[3] + rotationSpeed > 255) {
//            std::cout << "motor[3] !" << std::endl;
//            double factor = (motorSpeeds[3] - (abs(rotationSpeed) - 255 + motorSpeeds[3])) / abs(motorSpeeds[3]);
//            std::cout << "factor: " << factor << std::endl;
//
//            for (int i = 0; i < 4; i++) {
//                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
//            }
//        }
//    }
//
//    motorSpeeds[0] -= rotationSpeed;
//    motorSpeeds[1] -= rotationSpeed;
//    motorSpeeds[2] += rotationSpeed;
//    motorSpeeds[3] += rotationSpeed;
//
//    //for (int i = 0; i < 4; i++) motors[i]->setSpeed(motorSpeeds[i]);
//
//    return;
//}
//
//int main () {
//    drive(Vector(115, 156), 100);
//
//    for (int i = 0; i < 4; i++) {
//        std::cout << "motorSpeeds[" << i << "]: " << motorSpeeds[i] << std::endl;
//    }
//
//    return 0;
//}
//
///*
//int main () {
//    double x,y;
//
//    std::cout << "X: ";
//    std::cin >> x;
//    std::cout << "Y: ";
//    std::cin >> y;
//
//    Vector m(x,y);
//
//    Vector g;
//    Vector h;
//
//    double length_g, length_h;
//    length_g = g.abs();
//    length_h = h.abs();
//
//
//    if (m.getAngle() > (315) || m.getAngle() <= 45) {
//        g.set(1, 1);
//        h.set(-1, 1);
//        length_g = 1;
//        length_h = 1;
//    }
//    else if (m.getAngle() > 45 && m.getAngle() <= 135) {
//        g.set(1, 1);
//        h.set(1, -1);
//        length_g = 1;
//        length_h = -1;
//    }
//    else if (m.getAngle() > 135 && m.getAngle() <= 225) {
//        g.set(-1, -1);
//        h.set(1, -1);
//        length_g = -1;
//        length_h = -1;
//    }
//    else if (m.getAngle() > 225 && m.getAngle() <= 315) {
//        g.set(-1, -1);
//        h.set(-1, 1);
//        length_g = -1;
//        length_h = 1;
//    }
//    else {
//        std::cerr << "wrong vector" << std::endl;
//    }
//
//    g.setLenght(cos(abs(m.getAngle(g, false, false))) * m.abs());
//    h.setLenght(sin(abs(m.getAngle(g, false, false))) * m.abs());
//
//
//    length_g *= g.abs();
//    length_h *= h.abs();
//
//    std::cout << "Length(g): " << (int)length_g << std::endl;
//    std::cout << "Length(h): " << (int)length_h << std::endl;
//
//    std::cout << "Length(res): " << (int)sqrt((g.abs() * g.abs())+ (h.abs() * h.abs())) << std::endl;
//    std::cout << "Length(m): " << (int)m.abs() << std::endl;
//}
//*/
