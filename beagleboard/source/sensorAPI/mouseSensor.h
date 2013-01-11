
#ifndef _MOUSE_SENSOR_H_
#define _MOUSE_SENSOR_H_

#include "ipc/ipc_connection.h"
#include "vector.hpp"
#include "angle.h"

class MouseSensor {
    public:
        MouseSensor(std::string _ipcName);
        ~MouseSensor();

       Vector getRelativeVector();
       Vector getPositionVector();
       Vector getRelativeDirVector();
       Vector getAbsoluteDirVector();
       Angle getRelativeAngle();
       Angle getAbsoluteAngle();

    private:
        ipcReceivingConnection* ipcRCon;
        std::string ipcName;

        Vector relativeVector;
        Vector positionVector;
        Vector absDirVector;
        Vector relDirVector;

        void updateData();

};

#endif // _MOUSE_SENSOR_H_

