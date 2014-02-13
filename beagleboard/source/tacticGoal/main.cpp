
#include <unistd.h>

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
        Vector drive;
/*
        if(ir[0]->getValue() == 0 && ir[1]->getValue() == 0 && ir[2]->getValue() == 0 && ir[3]->getValue() == 0) {
            drive.set(0, 0);
        }
        else if(ir[3]->getValue() > 20) {
            drive.set(0, -1);
            drive.setLength(80);
        }
        else if(ir[0]->getValue() < ir[1]->getValue() && ir[2]->getValue() < ir[1]->getValue()) {
            drive.set(0, 1);
            drive.setLength(80);
        }
        else if(ir[0]->getValue() > ir[1]->getValue()) {
            drive.set(1, 1);
            drive.setLength(80);
        }
        else if(ir[2]->getValue() > ir[1]->getValue()) {
            drive.set(-1, 1);
            drive.setLength(80);
        }*/

        debug->send("Drive vector: %d | %d %d", drive.getX(), drive.getY(), ir[1]->getValue());
        debug->send("%d\t%d\t%d\t%d", ir[0]->getValue(), ir[1]->getValue(), ir[2]->getValue(), ir[3]->getValue());
        //motion->drive(drive, 0);



        usleep(100000);
    }


}
