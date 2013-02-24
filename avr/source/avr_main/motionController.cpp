
// motionController.cpp


#include "motionController.h"

MotionController::MotionController() {

}

MotionController::~MotionController() {
    free (motorSpeeds)
}

void MotionController::drive(Direction dir, unsigned short speed); {

	if(speed > 255) {
		uart_debug("Bad motor speed");
		return;
	}

	switch(dir)
	{
		case FRONT:
			motor.setSpeed(0, speed);
			motor.setSpeed(1, speed);
			motor.setSpeed(2, speed);
			motor.setSpeed(3, speed);
			break;
		case FRONTRIGHT:
			motor.setSpeed(0, 0);
			motor.setSpeed(1, speed);
			motor.setSpeed(2, 0);
			motor.setSpeed(3, speed);
			break;
		case RIGHT:
			motor.setSpeed(0, -speed);
			motor.setSpeed(1, speed);
			motor.setSpeed(2, -speed);
			motor.setSpeed(3, speed);
			break;
		case BACKRIGHT:
			motor.setSpeed(0, -speed);
			motor.setSpeed(1, 0);
			motor.setSpeed(2, -speed);
			motor.setSpeed(3, 0);
			break;
		case BACK:
			motor.setSpeed(0, -speed);
			motor.setSpeed(1, -speed);
			motor.setSpeed(2, -speed);
			motor.setSpeed(3, -speed);
			break;
		case BACKLEFT:
			motor.setSpeed(0, 0);
			motor.setSpeed(1, -speed);
			motor.setSpeed(2, 0);
			motor.setSpeed(3, -speed);
			break;
		case LEFT:
			motor.setSpeed(0, speed);
			motor.setSpeed(1, -speed);
			motor.setSpeed(2, speed);
			motor.setSpeed(3, -speed);
			break;
		case FRONTLEFT:
			motor.setSpeed(0, speed);
			motor.setSpeed(1, 0);
			motor.setSpeed(2, speed);
			motor.setSpeed(3, 0);
			break;

	}


    return;
}

void MotionController::pbreak() {
    motor.setSpeed(0, 0);
    motor.setSpeed(1, 0);
    motor.setSpeed(2, 0);
    motor.setSpeed(3, 0);

    return;
}

