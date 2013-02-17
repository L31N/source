

#include <iostream>
#include <fstream>

#include <signal.h>

#include <ipc/bbipc.h>
#include <ipc/ipc_connection.h>

using namespace std;

const std::string DEBUG_TMP_FILE_PATH = "/bbusr/tmp/debug.tmp";


void exit_handler(int signum);

int main(int argc, char* argv[])
{
    /// setup exit_handler ...
    if (signal (SIGINT, exit_handler) == SIG_ERR) cerr << "error: cant catch SIGTERM" << endl;

    //test ob argumente passen
    if(argc < 2)
    {
        cout << "To few parameters!" << endl;
        return -1;
    }

    int *showTable = new int[argc-1];
    int showTableCount = 0;
    bool showAll = false;
    bool invalid = false;

    ipcConfig ipcconfig("/bbusr/etc/ipc.conf");

    //parameter durchsuchen
    for(int i = 0; i < argc-1; i++)
    {

            string tmp(argv[i+1]);

            if(!tmp.compare("-a"))
            {
                showAll=true;
                delete(showTable);
                break;
            }
            else
            {
                showTable[i]=ipcconfig.getIpcIDToProcessSyn(tmp);
                if(showTable[i]==-1)
                {
                    cout << "Invalid Process Syn!: " << tmp << endl;
                    invalid=true;
                }
                showTableCount++;
            }
    }

    //Invalide Parameter abbrechen
    if(invalid)
    {
        cout << "Invalid Process Syns found!" << endl;
        return -2;
    }


    ipcReceivingConnection receivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("DEBUG"), 500);

    ofstream ofs(DEBUG_TMP_FILE_PATH.c_str(), ios_base::out | ios_base::trunc);
    if (!ofs.is_open()) {
        cerr << "error: cant open stream to " << DEBUG_TMP_FILE_PATH.c_str() << endl;
    }
    else {
        ofs.put('1');
        ofs.close();
    }

    while(true)
    {
        Data *data = receivingConnection.readDataFromBuffer();
        if(data != NULL)
        {
            int senderId = data->getSenderID();
            string message = data->getData();
            bool transfer = false;

            for(int i = 0; i < showTableCount; i++)
            {
                if(showTable[i]==senderId)
                {
                    transfer = true;
                    break;
                }
            }

            if(transfer || showAll)
            {
                cout << "[" << ipcconfig.getProcessSynToIpcID(senderId) << "] " << message << endl;
            }

        }

        usleep(1000);
    }

    ofs.open(DEBUG_TMP_FILE_PATH.c_str(), ios_base::out | ios_base::trunc);
    if (!ofs.is_open()) {
        cerr << "error: cant open stream to " << DEBUG_TMP_FILE_PATH.c_str() << endl;
    }
    else {
        ofs.put('0');
        ofs.close();
    }

    return 0;
}


void exit_handler(int signum) {

    #ifdef DEBUG
        cout << "in exithandler ..." << endl;
    #endif

    ofstream ofs(DEBUG_TMP_FILE_PATH.c_str());
    if (!ofs.is_open()) {
        cerr << "error: cant open stream to " << DEBUG_TMP_FILE_PATH << endl;
    }
    else {
        ofs.put('0');
        ofs.close();
    }

    exit(1);
}
