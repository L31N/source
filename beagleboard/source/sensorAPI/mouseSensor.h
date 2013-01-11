
#ifndef _MOUSE_SENSOR_H_
#define _MOUSE_SENSOR_H_

#include "ipc/ipc_connection.h"
#include "vector.hpp"

class MouseSensor {
    public:
        enum Side {
            left = 0,
            right = 1
        };

        MouseSensor(std::string _ipcName, Side _side);
        ~MouseSensor();

        Vector getNullVector();
        Vector getRelativeVector();

    private:
        ipcReceivingConnection* ipcRCon;
        Vector nullVector;
        Vector relativeVector;
        std::string ipcName;
        Side side;

};

#endif // _MOUSE_SENSOR_H_

