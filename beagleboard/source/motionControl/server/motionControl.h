
#ifndef _MOTION_CONTROL_H_
#define _MOTION_CONTROL_H_

#include <boost/thread/thread.hpp>

#include "ipc/ipc_connection.h"
#include "extendedMotionController.h"

#include "sensor/gyroSensor.h"

class MotionControl {
    public:
        MotionControl();
        ~MotionControl();

        void run();

    private:

        static void thDrive_fctn(ExtendedMotionController* emCtrlr, Vector vector, short rotationSpeed, GyroSensor* gyro);
        static void thMoveto_fctn(ExtendedMotionController* emCtrlr, Vector vector, unsigned char speed, Vector dir);
        static void thMove_fctn(ExtendedMotionController* emCtrlr, Vector vector, unsigned char speed, Vector dir);
        static void thTurnto_fctn(ExtendedMotionController* emCtrlr, Vector dir, unsigned char speed, ExtendedMotionController::Direction turndir);
        static void thTurn_fctn(ExtendedMotionController* emCtrlr, Vector dir, unsigned char speed, ExtendedMotionController::Direction turndir);
        static void thPBreak_fctn(ExtendedMotionController* emCtrlr);
        static void thIdle_fctn(ExtendedMotionController* emCtrlr);
        static void thTest_fctn(ExtendedMotionController* emCtrlr);

        /** ++++++++++++++++++ **/

        ExtendedMotionController* extMtnCtrlr;
        ipcReceivingConnection* ipcRcon;

        GyroSensor* gyro;

        Debug* dbg;

        boost::thread* thActive;

};

#endif // _MOTION_CONTROL_H_
