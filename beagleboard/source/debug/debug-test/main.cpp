
#include <iostream>

#include "debug/bbdebug.h"

using namespace std;

int main()
{
    Debug debug("IDLE_BOT");

    while(true)
    {
        string input;
        cout << "cin >> ";
        cin >> input;
        cout << "sent: " << input << endl;
        debug.send("%s", input.c_str());
    }

    return 0;
}
