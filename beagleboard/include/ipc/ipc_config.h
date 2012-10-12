
#ifndef _IPC_CONFIG_H_
#define _IPC_CONFIG_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class ipcConfig {
    public:
        ipcConfig(std::string _IPC_CONFIG_FILE = "../etc/ipc.conf");
        ~ipcConfig();

        short getIpcIDToProcessSyn(std::string processSyn);
        std::string getProcessSynToIpcID(short ipcID);

    private:
        std::string IPC_CONFIG_FILE;
        std::ifstream* ifConfig;

        std::string UDS_FILE_PATH;

    public:
        std::string getIPC_CONFIG_FILE(void);
        std::string getUDS_FILE_PATH(void);
};

#endif // _IPC_CONFIG_H_
