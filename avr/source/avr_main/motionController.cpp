
// motionController.cpp

#include <util/delay.h>
#include <stdlib.h>

#include "motionController.h"

MotionController::MotionController() {
    speeds = (unsigned short*)malloc(4*sizeof(unsigned char));

    for (int i = 0; i < 4; i++) speeds[i] = 0;
}

MotionController::~MotionController() {
    free(speeds);
}

void MotionController::drive(Direction dir, unsigned short speed) {
	if(speed > 255) {
		uart_debug("Bad motor speed");
		return;
	}

	switch(dir)
	{
		case FRONT:
			motor.setSpeed(speed, speed, speed, speed);

			speeds[0] = speed;
			speeds[1] = speed;
			speeds[2] = speed;
			speeds[3] = speed;

			break;
		case FRONTRIGHT:
			motor.setSpeed(0, speed, 0, speed);

			speeds[0] = 0;
			speeds[1] = speed;
			speeds[2] = 0;
			speeds[3] = speed;

			break;
		case RIGHT:
			motor.setSpeed(-speed, speed, -speed, speed);

			speeds[0] = -speed;
			speeds[1] = speed;
			speeds[2] = -speed;
			speeds[3] = speed;

			break;
		case BACKRIGHT:
            motor.setSpeed(-speed, 0, -speed, 0);

			speeds[0] = -speed;
			speeds[1] = 0;
			speeds[2] = -speed;
			speeds[3] = 0;

			break;
		case BACK:
			motor.setSpeed(-speed, -speed, -speed, -speed);

			speeds[0] = -speed;
			speeds[1] = -speed;
			speeds[2] = -speed;
			speeds[3] = -speed;

			break;
		case BACKLEFT:
			motor.setSpeed(0, -speed, 0, -speed);

			speeds[0] = 0;
			speeds[1] = -speed;
			speeds[2] = 0;
			speeds[3] = -speed;

			break;
		case LEFT:
            motor.setSpeed(speed, -speed, speed, -speed);

			speeds[0] = speed;
			speeds[1] = -speed;
			speeds[2] = speed;
			speeds[3] = -speed;

			break;
		case FRONTLEFT:
			motor.setSpeed(speed, 0, speed, 0);

			speeds[0] = speed;
			speeds[1] = 0;
			speeds[2] = speed;
			speeds[3] = 0;

			break;
        case ROTATERIGHT:
			motor.setSpeed(-speed, -speed, speed, speed);

			speeds[0] = -speed;
			speeds[1] = -speed;
			speeds[2] = speed;
			speeds[3] = speed;

			break;
        case ROTATELEFT:
			motor.setSpeed(speed, speed, -speed, -speed);

			speeds[0] = speed;
			speeds[1] = speed;
			speeds[2] = -speed;
			speeds[3] = -speed;

			break;
	}

    return;
}

void MotionController::pbreak() {
    /*motor.setSpeed(0, 0);
    motor.setSpeed(1, 0);
    motor.setSpeed(2, 0);
    motor.setSpeed(3, 0);*/

    /*motor.setSpeed(0, -speeds[0]);
    motor.setSpeed(1, -speeds[1]);
    motor.setSpeed(2, -speeds[2]);
    motor.setSpeed(3, -speeds[3]);*/

    motor.setSpeed(-speeds[0], -speeds[1], -speeds[2], -speeds[3]);

    /*motor.setSpeed(0, -speeds[0]);
    motor.setSpeed(1, -speeds[1]);

    _delay_ms(1000);

    motor.setSpeed(2, -speeds[2]);
    motor.setSpeed(3, -speeds[3]);*/

    _delay_ms(60);

    motor.setSpeed(0, 0, 0, 0);

    for (int i = 0; i < 4; i++) speeds[i] = 0;


    return;
}
