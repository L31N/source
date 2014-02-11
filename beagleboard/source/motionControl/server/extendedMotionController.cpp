
// extendedMotionController.cpp

#include "extendedMotionController.h"

ExtendedMotionController::ExtendedMotionController() {
    gyro = new GyroSensor("GYRO");
    /// create navigation-object here !
}

ExtendedMotionController::~ExtendedMotionController() {
    /// destroy navigation-object here !
}

void ExtendedMotionController::moveto(Vector vector, unsigned char speed, Vector dir) {

}

/*void ExtendedMotionController::moveto(Vector vector, unsigned char speed, Angle dir) {

}*/

void ExtendedMotionController::move(Vector vector, unsigned char speed, Vector dir) {

}

/*void ExtendedMotionController::move(Vector vector, unsigned char speed, Angle dir) {

}*/

void ExtendedMotionController::turnto(Vector dir, unsigned char speed, Direction turndir) {
    /*Vector now = gyro->getVector();
    if (turndir == automatic) {

    }*/
}

/*void ExtendedMotionController::turnto(Angle dir, unsigned char speed, Direction turndir) {

}*/
