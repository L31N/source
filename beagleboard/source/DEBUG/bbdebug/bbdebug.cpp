
// bbdebug.cpp

#include "bbdebug.hpp"

/** CLASS DEBUG_CONTROLLER **/

DebugController::DebugController(std::string ProcessSynonym) {
    ipcConfig* ipcconf = new ipcConfig;
    ipcSendingConnection* DBGSendingCon = new ipcSendingConnection(ipcconf->getUDS_FILE_PATH, ipcconf->getIpcIDToProcessSyn(ProcessSynonym), ipcconf->getIpcIDToProcessSyn("DEBUG_MODULE"));
}

DebugController::~DebugController() {
    delete ipcconf;
    delete DBGSendingCon;
}

void DebugController::pushBack(std::string dbg_string) {
    vector.push_back(dbg_string);
}

bool DebugController::checkObjectForDebug(short objectID) {
    std::ifstream ifs("")
}

void DebugController::send() {
    while(!vector.empty()) {
        std::string dbg_string = vector.front();
        short ID = dbg_string[0];

        // check weather the Object is set for Debugging ...

    }
}

/** CLASS DEBUG_OBJECT **/

DebugObject::DebugObject(std::string _objectIdentifier) {
    objectIdentifier = _objectIdentifier;
}

DebugObject::~DebugObject() {}

void DebugObject::push(std::string data) {

}
