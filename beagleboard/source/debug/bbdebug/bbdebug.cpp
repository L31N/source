// bbdebug.cpp

#include "bbdebug.h"

Debug::Debug(std::string moduleName) {
    #ifdef DEBUG
    //    std::cout << "Debug::Debug --> ipcConfig()" << std::endl;
    #endif
    ipcconf = new ipcConfig(IPC_CONFIG_FILE_PATH);
    #ifdef DEBUG
    //    std::cout << "Debug::Debug --> ipcSendingConnection()" << std::endl;
    #endif

    senCon = new ipcSendingConnection(ipcconf->getUDS_FILE_PATH(), ipcconf->getIpcIDToProcessSyn(moduleName), ipcconf->getIpcIDToProcessSyn("DEBUG"));

    /// setting up ifstream, necessary to check weather the debug module is already running
    dif = new std::ifstream(DEBUG_TMP_FILE_PATH.c_str());
    if (!dif->is_open()) {
        std::cout << "error ... could not open ifstream to " << DEBUG_TMP_FILE_PATH << std::endl;
    }
}

Debug::~Debug() {
    delete senCon;
}

bool Debug::send(const char* format, ...) {
    static char buffer[16000];
    va_list  argptr;
    va_start( argptr, format );
    vsprintf( buffer, format, argptr );
    va_end  ( argptr );

    std::string str(buffer);

    dif->clear();
    dif->seekg(0);
    char debug_status = '0';

    dif->get(debug_status);

    #ifdef DEBUG
        std::cout << "debug_status: " << debug_status << std::endl;
    #endif
    if (debug_status == '1') {
            senCon->reconnect();
            if (senCon->sendData(str)) return true;
            else return false;

    }
    else return false;
}
