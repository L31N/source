
#include "ipc_config.h"

ipcConfig::ipcConfig(std::string _IPC_CONFIG_FILE) : IPC_CONFIG_FILE(_IPC_CONFIG_FILE) {
    ifConfig = new std::ifstream(IPC_CONFIG_FILE.c_str());
    if(!ifConfig->is_open()) {
        std::cout << "error cannot open stream to: " << IPC_CONFIG_FILE << std::endl;
        return;
    }

    /// setup uds-file-path --> search for line beginning with '+'
    UDS_FILE_PATH = "";
    while(!ifConfig->eof()) {
        char cline[256];
        ifConfig->getline(cline, 256);
        std::string line(cline);

        if (line[0] == '+') {   /// line containing file-path found
            line.erase(0,1);
            UDS_FILE_PATH = line;
            break;
        }
    }
}

ipcConfig::~ipcConfig() {
    ifConfig->close();
    delete ifConfig;
}

std::string ipcConfig::getIPC_CONFIG_FILE(void) { return IPC_CONFIG_FILE; }
std::string ipcConfig::getUDS_FILE_PATH(void) { return UDS_FILE_PATH; }

short ipcConfig::getIpcIDToProcessSyn(std::string processSyn) {

    #ifdef DEBUG
        std::cout << "in ipcConfig::getIpcIDToProcessSyn() " << std::endl;
    #endif

    short ID = -1;
    std::string syn = "";

    ifConfig->clear();
    ifConfig->seekg(0);
    while(ifConfig->eof() == false) {

        #ifdef DEBUG
            std::cout << "in while: eof():" << ifConfig->eof() << std::endl;
        #endif

        char cline[256];
        ifConfig->getline(cline, 256);
        std::string line(cline);

        if (line[0] == '-') {
            line.erase(0,1);
            size_t doublePoint = line.find_first_of(':');
            char cIPC_ID[1];
            line.copy(cIPC_ID, 1, doublePoint+1);
            line.resize(doublePoint);

            /*std::stringstream ss;
            ss << *cIPC_ID;
            ss >> ID;*/

            ID = *cIPC_ID;

            syn = line;

            if (syn == processSyn) return ID;
        }
    }
    return -1;
}

std::string ipcConfig::getProcessSynToIpcID(short ipcID) {
    short ID = -1;
    std::string syn = "";

    ifConfig->clear();
    ifConfig->seekg(0);
    while(ifConfig->eof() == false) {
       char cline[256];
        ifConfig->getline(cline, 256);
        std::string line(cline);

        if (line[0] == '-') { /// '-' found --> this line could contain the information
            line.erase(0,1);
            size_t doublePoint = line.find_first_of(':');
            char cIPC_ID[1];
            line.copy(cIPC_ID, 1, doublePoint+1);
            line.resize(doublePoint);

            /*std::stringstream ss;
            ss << *cIPC_ID;
            ss >> ID;*/

            ID = *cIPC_ID;

            syn = line;

            if (ID == ipcID) return syn;
        }
    }
    return "";
}
