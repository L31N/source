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
    bool showAll = false;

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
                //showTable[i]=ipcconfig.get
            }

            cout << tmp << endl;
    }




    ipcReceivingConnection receivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("DEBUG"), 10);

    /*while(true)
    {

    }*/
}
