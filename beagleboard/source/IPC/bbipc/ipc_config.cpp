
#include "ipc_config.h"

ipcConfig::ipcConfig(std::string _IPC_CONFIG_FILE) : IPC_CONFIG_FILE(_IPC_CONFIG_FILE) {
    ifConfig = new std::ifstream(IPC_CONFIG_FILE.c_str());
    if(!ifConfig->is_open()) {
        std::cout << "error cannot open stream to: " << IPC_CONFIG_FILE << std::endl;
        return;
    }

    /// setup uds-file-path --> search for line beginning with '+'
    UDS_FILE_PATH = "";
    ifConfig->clear();
    while(!ifConfig->eof()) {
        #ifdef DEBUG
        //    std::cout << "in ipcConfig::ipcConfig --> while(!ifConfig->eof())" << std::endl;
        #endif
        char cline[256];
        ifConfig->getline(cline, 256);
        std::string line (cline);
        #ifdef DEBUG
        //    std::cout << "line: " << line << std::endl;
        #endif

        if (line[0] == '+') {   /// line containing file-path found
            #ifdef DEBUG
            //    std::cout << "UDS_FILE_PATH in config-file found ..." << std::endl;
            #endif
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

    short ID = -1;
    std::string syn = "";

    ifConfig->clear();
    ifConfig->seekg(0);
    while(ifConfig->eof() == false) {

        char cline[256];
        ifConfig->getline(cline, 256);
        std::string line(cline);

        if (line[0] == '-') {
            line.erase(0,1);
            size_t doublePoint = line.find_first_of(':');
            size_t id_length = line.length() - doublePoint-1;

            std::string id_str = line.substr(doublePoint + 1, id_length);
            syn = line.substr(0, doublePoint);

            std::stringstream ss;
            ss << id_str;
            ss >> ID;

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
            size_t id_length = line.length() - doublePoint-1;

            std::string id_str = line.substr(doublePoint + 1, id_length);
            syn = line.substr(0, doublePoint);

            std::stringstream ss;
            ss << id_str;
            ss >> ID;

            if (ID == ipcID) return syn;
        }
    }
    return "";
}
