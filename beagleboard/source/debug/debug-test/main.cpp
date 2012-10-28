#include <iostream>

#include <bbdebug.h>

using namespace std;

int main()
{
    Debug debug("TACTIC_MODULE");

    while(true)
    {
        std::string input;
        cin >> input;
        //debug.send("%s", input.c_str());
        debug.send("test");
    }


    return 0;
}
