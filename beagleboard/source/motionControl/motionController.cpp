
// motionController.cpp

#include "motionController.h"

MotionController::MotionController() {
    motors = new Motor* [4];

    motors[0] = new Motor("MOTORG45", "MOTORG45");
    motors[1] = new Motor("MOTORG135", "MOTORG135");
    motors[2] = new Motor("MOTORG225", "MOTORG225");
    motors[3] = new Motor("MOTORG315", "MOTORG315");

    debug = new Debug("MOTION_CONTROLLER");
}

MotionController::~MotionController() {
    delete motors[0];
    delete motors[1];
    delete motors[2];
    delete motors[3];

    delete debug;
}

void MotionController::drive(Vector vector, short rotationSpeed) {

}

void MotionController::drive(Angle angle, unsigned char speed, short rotationSpeed) {

}

void MotionController::pbreak() {
    /// implement power-break functionality here !
}

