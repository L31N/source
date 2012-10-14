
#include "cncconv.h"

int main () {
    /*Config cfg("cnc.conf");

    string text, command, argument;
    int line_number;

    cout << "\nFCOMS #################" << endl;
    while(cfg.getNextFCommand(text, command, argument, line_number)) {
        cout << "text: " << text << endl;
        cout << "command: " << command << endl;
        cout << "argument: " << argument << endl;
        cout << "line_number: " << line_number << endl;
        cout << endl;
    }

    cout << "\nXCOMS #################" << endl;
    while(cfg.getNextXCommand(text, command, argument, line_number)) {
        cout << "text: " << text << endl;
        cout << "command: " << command << endl;
        cout << "argument: " << argument << endl;
        cout << "line_number: " << line_number << endl;
        cout << endl;
    }

    cout << "\nYCOMS #################" << endl;
    while(cfg.getNextYCommand(text, command, argument, line_number)) {
        cout << "text: " << text << endl;
        cout << "command: " << command << endl;
        cout << "argument: " << argument << endl;
        cout << "line_number: " << line_number << endl;
        cout << endl;
    }

    cout << "\nZCOMS #################" << endl;
    while(cfg.getNextZCommand(text, command, argument, line_number)) {
        cout << "text: " << text << endl;
        cout << "command: " << command << endl;
        cout << "argument: " << argument << endl;
        cout << "line_number: " << line_number << endl;
        cout << endl;
    }

    cout << "\n#####################################################" << endl;
    cout << cfg.getNumOfWarnings() << " WARNINGS" << endl;*/

    Converter conv("cnc.conf", "input.txt", "output.txt");

    return 0;
}
