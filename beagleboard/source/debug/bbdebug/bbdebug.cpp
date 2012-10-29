// bbdebug.cpp

#include "bbdebug.h"

Debug::Debug(std::string moduleName) {
    ipcconf = new ipcConfig("../../../../../etc/");
    senCon = new ipcSendingConnection(ipcconf->getUDS_FILE_PATH(), ipcconf->getIpcIDToProcessSyn(moduleName), ipcconf->getIpcIDToProcessSyn("DEBUG"));
}

Debug::~Debug() {
    delete senCon;
}

void Debug::send(const char* format, ...) {
    static char buffer[16000];
    va_list  argptr;
    va_start( argptr, format );
    vsprintf( buffer, format, argptr );
    va_end  ( argptr );

    std::string str(buffer);

    senCon->sendData(str);
}
