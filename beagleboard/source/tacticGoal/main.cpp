
#include <unistd.h>

#include "bbvector.h"

#include "debug/bbdebug.h"
#include "sensor/irSensor.h"
#include "motion/motion.h"
#include "navigation/navigation.h"

#define TACTIC_GOAL

#define ANGLE_TRESHOLD 10

#define BALL_SPEED 50
#define ADJUST_SPEED 35

#define X_MIN 900
#define X_MAX FIELD_WIDTH - X_MIN
#define X_TARGET FIELD_WIDTH / 2 + 100
#define X_TOLLERANCE 200

#ifdef TACTIC_GOAL
    #define Y_MIN 800
    #define Y_MAX FIELD_HEIGHT - Y_MIN
    #define Y_TARGET 700
    #define Y_TOLLERANCE 200
#else
    #define Y_MIN 800
    #define Y_MAX FIELD_HEIGHT - Y_MIN
    #define Y_TARGET 1000
    #define Y_TOLLERANCE 200
#endif


#define FIELD_WIDTH 1810
#define FIELD_HEIGHT 2425

#define TURNTO_COUNT_TRESHOLD 10


int main() {
    Debug *debug;
    debug = new Debug("TACTIC_GOAL");
    debug->send("Tactic for Goal starting...");

    IRSensor *ir[4];
    ir[0] = new IRSensor("BALL_SENSOR4", "BALL_SENSORS", 4);
    ir[1] = new IRSensor("BALL_SENSOR5", "BALL_SENSORS", 5);
    ir[2] = new IRSensor("BALL_SENSOR6", "BALL_SENSORS", 6);
    ir[3] = new IRSensor("BALL_SENSOR7", "BALL_SENSORS", 7);

    Motion* motion;
    motion = new Motion;

    //Main Loop
    while(true) {
        if (ir[0]->getValue()) motion->drive(Vector(60,0), 0);
        else motion->drive(Vector(0,0), 5);
    }


//        // position
//        navi.calculate();
//        Vector position = navi.getPosition();
//        debug->send("X: %f|Y: %f", position.getX(), position.getY());
//
//
//        // correct angle to zero degree
//        turnto_count ++;
//        if (turnto_count > TURNTO_COUNT_TRESHOLD && abs(gyro.getVector().getAngle(true, false)) > ANGLE_TRESHOLD) {
//            motion->turnto(Vector(0,1), 30, Motion::automatic);
//            turnto_count = 0;
//        }
//        //else {
//            Vector drive(0, 0);
//            if(ir[0]->getValue() < 30 && ir[1]->getValue() < 30 && ir[2]->getValue() < 30 && ir[3]->getValue() < 30) {     // no ball --> adjustment
//                //motion->pbreak();
//                motion->drive(Vector(0,0), 0);
//                ///debug->send("Ball not available - adjustment");
//
//                /** adjustment **/
//                if (position.getX() < X_TARGET - X_TOLLERANCE) drive.setX(1);
//                else if (position.getX() > X_MAX + X_TOLLERANCE) drive.setX(-1);
//
//                if (position.getY() < Y_TARGET - Y_TOLLERANCE) drive.setY(1);
//                else if (position.getY() > Y_TARGET + Y_TOLLERANCE) drive.setY(-1);
//
//                /// if already adjusted and no ball -> drive left - right to seek the ball
//
//
//                drive.setLength(ADJUST_SPEED);
//                motion->drive(drive, 0);
//            }
//            else {
//                if(position.getY() > Y_MIN && (ir[3]->getValue() > (ir[0]->getValue() + ir[1]->getValue() + ir[2]->getValue())/3)) {        // schraeg nach hinten
//                    debug->send("Ball is behind");
//                    drive.setY(-1);
//
//                    // schraeg, jenachdem wo mehr platz
//                    if (position.getX() > FIELD_WIDTH / 2) drive.setX(-1);
//                    else drive.setX(1);
//
//                    drive.setLength(BALL_SPEED);
//                }
//                else if(position.getY() < Y_MAX && (ir[0]->getValue() <= ir[1]->getValue() && ir[2]->getValue() <= ir[1]->getValue())) {
//                    debug->send("Ball is in front");
//                    drive.set(0, 1);
//                    drive.setLength(BALL_SPEED);
//                }
//                else if(position.getX() < X_MAX && (ir[0]->getValue() > ir[1]->getValue())) {    // drive right
//                    debug->send("Ball is front right");
//                    //drive.set(1.5, 1);
//                    drive.set(1, 0);
//                    drive.setLength(BALL_SPEED);
//                }
//                else if(position.getX() > X_MIN && (ir[2]->getValue() > ir[1]->getValue())) {    // drive left
//                    debug->send("Ball is front left");
//                    //drive.set(-1.5, 1);
//                    drive.set(-1, 0);
//                    drive.setLength(BALL_SPEED);
//                }
//
//                //debug->send("Drive vector: %f | %f %d", drive.getX(), drive.getY(), ir[1]->getValue());
//
//                ///debug->send("%d\t%d\t%d\t%d", ir[0]->getValue(), ir[1]->getValue(), ir[2]->getValue(), ir[3]->getValue());
//
//                motion->drive(drive, 0);
//
//            }
//        //}
//
//        usleep(30000);
//    }
//

}
