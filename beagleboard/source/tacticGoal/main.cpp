
#include <unistd.h>

#include "bbvector.h"

#include "debug/bbdebug.h"
#include "sensor/irSensor.h"
#include "motion/motion.h"

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

        if(ir[0]->getValue() < 30 && ir[1]->getValue() < 30 && ir[2]->getValue() < 30 && ir[3]->getValue() < 30) {
            //motion->pbreak();
            motion->drive(Vector(0,0), 0);
            debug->send("Ball not available");
        }
        else {
            Vector drive(0, 0);
            if(ir[3]->getValue() > (ir[0]->getValue() + ir[1]->getValue() + ir[2]->getValue())/3) {
                debug->send("Ball is behind");
                drive.set(0, -1);
                drive.setLength(30);
            }
            else if(ir[0]->getValue() <= ir[1]->getValue() && ir[2]->getValue() <= ir[1]->getValue()) {
                debug->send("Ball is in front");
                drive.set(0, 1);
                drive.setLength(30);
            }
            else if(ir[0]->getValue() > ir[1]->getValue()) {
                debug->send("Ball is front right");
                drive.set(1, 1);
                drive.setLength(30);
            }
            else if(ir[2]->getValue() > ir[1]->getValue()) {
                debug->send("Ball is front left");
                drive.set(-1, 1);
                drive.setLength(30);
            }

            //debug->send("Drive vector: %f | %f %d", drive.getX(), drive.getY(), ir[1]->getValue());

            debug->send("%d\t%d\t%d\t%d", ir[0]->getValue(), ir[1]->getValue(), ir[2]->getValue(), ir[3]->getValue());

            //motion->drive(drive, 0);

        }

        usleep(30000);
    }


}
