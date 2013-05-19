
#ifndef _MOTION_CONTROL_H_
#define _MOTION_CONTROL_H_

#include <boost/thread/thread.hpp>

#include "ipc/ipc_connection.h"
#include "extendedMotionController.h"

class MotionControl {
    public:
        MotionControl();
        ~MotionControl();

        void run();

    private:
        static void thReceive_fctn(ipcReceivingConnection* rcon);
        static void thControl_fctn(ExtendedMotionController* emctrlr);

        /** ++++++++++++++++++ **/

        boost::thread* thReceive;
        boost::thread* thControl;

        ExtendedMotionController* extMtnCtrlr;
        ipcReceivingConnection* ipcRcon;
};

#endif // _MOTION_CONTROL_H_
