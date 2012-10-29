// bbdebug.h

#ifndef _BB_DEBUG_H_
#define _BB_DEBUG_H_

#include <string>

#include <stdio.h>
#include <stdarg.h>

#include "ipc/bbipc.h"

class Debug {
    public:
        Debug(std::string moduleName);
        ~Debug();

        void send(const char* format, ...);

    private:
        ipcConfig* ipcconf;
        ipcSendingConnection* senCon;
};

#endif // _BB_DEBUG_H_
