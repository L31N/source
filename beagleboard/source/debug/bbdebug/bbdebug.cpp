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

    senCon = new ipcSendingConnection(ipcconf->getIpcIDToProcessSyn(moduleName), ipcconf->getIpcIDToProcessSyn("DEBUG"), 64, ipcSendingConnection::local);
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

    if (senCon->sendData(str)) return true;
    else return false;
}
