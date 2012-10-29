#include <iostream>
#include <ipc/bbipc.h>

using namespace std;


int main(int argc, char* argv[])
{
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

    ipcConfig ipcconfig("../../../../../etc/ipc.conf");

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


    ipcReceivingConnection receivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("DEBUG"), 10);

    while(true)
    {
        Data *data;// = receivingConnection.readDataFromBuffer();
        if(data != NULL)
        {
            int senderId = data->getSenderID();
            string message = data->getData();
            bool transfer = false;

            /*for(int i = 0; i < showTableCount; i++)
            {
                cout << "test weather " << showTable[i] << " is equal to " << senderId;
                if(showTable[i]==senderId)
                {
                    transfer = true;
                    break;
                }
            }*/

        }
    }
}
