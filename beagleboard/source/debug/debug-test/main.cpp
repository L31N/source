
#include <iostream>

#include "bbdebug.h"

using namespace std;

int main()
{
    //Debug debug("TACTIC_MODULE");

    while(true)
    {
        string input;
        cin >> input;
        cout << "sent: " << input << endl;
        debug.send("%s", input.c_str());
    }

    return 0;
}
