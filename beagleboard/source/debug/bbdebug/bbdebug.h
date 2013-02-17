// bbdebug.h

#ifndef _BB_DEBUG_H_
#define _BB_DEBUG_H_

#ifdef DEBUG
    #include <iostream>
#endif // DEBUG

#include <string>

#include <stdio.h>
#include <stdarg.h>
#include <fstream>

#include "ipc/bbipc.h"
#include "ipc/ipc_config.h"
#include "ipc/ipc_connection.h"

const std::string DEBUG_TMP_FILE_PATH = "/bbusr/tmp/debug.tmp";
//const std::string IPC_CONFIG_FILE_PATH = "../../../../../etc/ipc.conf";       // already defined in file: "ipc/bbipc.h"

class Debug {
    public:
        Debug(std::string moduleName);
        ~Debug();

        bool send(const char* format, ...);

    private:
        ipcConfig* ipcconf;
        ipcSendingConnection* senCon;

        std::ifstream* dif;

        bool fNeedsReconnect;
};

#endif // _BB_DEBUG_H_
