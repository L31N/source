
// motionController.cpp

#include <cmath>

#include "motionController.h"

MotionController::MotionController() {
    motors = new Motor* [4];

    motors[0] = new Motor("MOTORG45", "MOTORG45");
    motors[1] = new Motor("MOTORG135", "MOTORG135");
    motors[2] = new Motor("MOTORG225", "MOTORG225");
    motors[3] = new Motor("MOTORG315", "MOTORG315");

    motorSpeeds = new short[4];
    for (int i = 0; i < 4; i++) motorSpeeds[i] = 0;

    debug = new Debug("MOTION_CONTROLLER");
}

MotionController::~MotionController() {
    delete motors[0];
    delete motors[1];
    delete motors[2];
    delete motors[3];

    delete motors;

    delete motorSpeeds;

    delete debug;
}

void MotionController::drive(Vector vector, short rotationSpeed) {
    Vector g;
    Vector h;

    double length_g, length_h;
    length_g = g.abs();
    length_h = h.abs();


    if (vector.getAngle() > (315) || vector.getAngle() <= 45) {
        g.set(1, 1);
        h.set(-1, 1);
        length_g = 1;
        length_h = 1;
    }
    else if (vector.getAngle() > 45 && vector.getAngle() <= 135) {
        g.set(1, 1);
        h.set(1, -1);
        length_g = 1;
        length_h = -1;
    }
    else if (vector.getAngle() > 135 && vector.getAngle() <= 225) {
        g.set(-1, -1);
        h.set(1, -1);
        length_g = -1;
        length_h = -1;
    }
    else if (vector.getAngle() > 225 && vector.getAngle() <= 315) {
        g.set(-1, -1);
        h.set(-1, 1);
        length_g = -1;
        length_h = 1;
    }
    else {
        std::cerr << "wrong vector" << std::endl;
        debug->send("MotionController::drive --> wrong Vector !!!");
    }

    g.setLenght(cos(abs(vector.getAngle(g, false, false))) * vector.abs());
    h.setLenght(sin(abs(vector.getAngle(g, false, false))) * vector.abs());

    length_g *= g.abs();
    length_h *= h.abs();

    if (length_g > 255 || length_g < (-255) || length_h > 255 || length_h < (-255)) {
        std::cerr << "MotorController::drive --> vector length out of range !!!" << std::endl;
        debug->send("MotorController::drive --> vector length out of range !!!");

        return; /// Error occured !
    }

    motorSpeeds[0] = length_h;
    motorSpeeds[1] = length_g;
    motorSpeeds[2] = length_h;
    motorSpeeds[3] = length_g;


    /// calculate motor-values dependend to the turn

    /// check here for speed overflows and step down the values if neccessary ...
    if (rotationSpeed < 0) {
        if (motorSpeeds[0] + rotationSpeed > 255) {
            double factor = (motorSpeeds[0] - (abs(rotationSpeed) - 255 + motorSpeeds[0])) / abs(motorSpeeds[0]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
        if (motorSpeeds[1] + rotationSpeed > 255) {
            double factor = (motorSpeeds[1] - (abs(rotationSpeed) - 255 + motorSpeeds[1])) / abs(motorSpeeds[1]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
        if (motorSpeeds[2] - rotationSpeed < (-255)) {
            double factor = (motorSpeeds[2] - (abs(rotationSpeed) - 255 + motorSpeeds[2])) / abs(motorSpeeds[2]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
        if (motorSpeeds[3] - rotationSpeed < (-255)) {
            double factor = (motorSpeeds[3] - (abs(rotationSpeed) - 255 + motorSpeeds[3])) / abs(motorSpeeds[3]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
    }
    else {
        if (motorSpeeds[0] - rotationSpeed < (-255)) {
            double factor = (motorSpeeds[0] - (abs(rotationSpeed) - 255 + motorSpeeds[0])) / abs(motorSpeeds[0]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
        if (motorSpeeds[1] - rotationSpeed < (-255)) {
            double factor = (motorSpeeds[1] - (abs(rotationSpeed) - 255 + motorSpeeds[1])) / abs(motorSpeeds[1]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
        if (motorSpeeds[2] + rotationSpeed > 255) {
            double factor = (motorSpeeds[2] - (abs(rotationSpeed) - 255 + motorSpeeds[2])) / abs(motorSpeeds[2]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
        if (motorSpeeds[3] + rotationSpeed > 255) {
            double factor = (motorSpeeds[3] - (abs(rotationSpeed) - 255 + motorSpeeds[3])) / abs(motorSpeeds[3]);

            for (int i = 0; i < 4; i++) {
                motorSpeeds[i] = double(motorSpeeds[i]) * factor;
            }
        }
    }

    motorSpeeds[0] -= rotationSpeed;
    motorSpeeds[1] -= rotationSpeed;
    motorSpeeds[2] += rotationSpeed;
    motorSpeeds[3] += rotationSpeed;

    for (int i = 0; i < 4; i++) motors[i]->setSpeed(motorSpeeds[i]);

    return;
}

void MotionController::drive(Angle angle, unsigned char speed, short rotationSpeed) {
    Vector vector(1,1);
    vector.setLength(speed);
    vector.setAngle(angle);

    drive(vector, rotationSpeed);
}

void MotionController::pbreak() {
    /// implement power-break functionality here !
}

