#include <iostream>
#include <ipc/bbipc.h>

using namespace std;


int main(int argc, char* argv[])
{
    //test ob argumente passen
    if(argc < 20)
    {
        cout << "To few parameters!" << endl;
    }



    ipcConfig ipcconfig("../../../../../etc/ipc.conf");

    ipcReceivingConnection receivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("DEBUG"), 10);

    while(true)
    {

    }
}
