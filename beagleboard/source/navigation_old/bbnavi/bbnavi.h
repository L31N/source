
#ifndef _BB_NAVI_H_
#define _BB_NAVI_H_

#include <string>
#include <iostream>

#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"

#include "bbdebug.h"
#include "bbvector.h"

class Navi {
    public:
        Navi();
        ~Navi();

        Vector getPosition();
        Vector getDirection();
        Vector getSpeed();

    private:
        void requestData();

        // ------------------- //

        Vector position;
        Vector direction;
        Vector speed;

        ipcSendingConnection* scon;
        ipcReceivingConnection* rcon;

        Debug* debug;
};

#endif // _BB_NAVI_H_
