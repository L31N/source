

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
    //test ob argumente passen
    if(argc < 2)
    {
        std::cout << "Usage: " << *argv << "[option] [ipc_syn0 ipc_syn1 ...]" << std::endl;
        std::cout << "options:\n\t-a\tshow all debug output\t" << std::endl;
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


    ipcReceivingConnection receivingConnection("DEBUG", 500, 64);

    while(true)
    {

        Data* data = receivingConnection.readDataFromBuffer();
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

    return 0;
}
