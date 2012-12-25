
#include <SerialStream.h>
#include <string>
#include <iostream>

#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"
#include "ipc/ipc_config.h"
#include "ipc/buffer.h"

#include "bbdebug.h"

const unsigned int SERIAL_BAUD_RATE = 57600;
const std::string SERIAL_INTERFACE = "/dev/ttyUSB0";

int main () {
    LibSerial::SerialStream* serial = new LibSerial::SerialStream(SERIAL_INTERFACE.c_str(), std::ios::in | std::ios::out);
    if (!serial->IsOpen()) {
        std::cerr << "\nerror: could not open serialStream: " << SERIAL_INTERFACE << std::endl;
        return -1;
    }
    #ifdef DEBUG
    else cout << "\nopened serialStream successfully\n" << endl;
    #endif


    switch (SERIAL_BAUD_RATE) {
        case 50:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_50);
            break;
        case 75:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_75);
            break;
        case 110:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_110);
            break;
        case 150:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_150);
            break;
        case 200:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_200);
            break;
        case 300:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_300);
            break;
        case 600:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_600);
            break;
        case 1200:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_1200);
            break;
        case 1800:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_1800);
            break;
        case 2400:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_2400);
            break;
        case 4800:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_4800);
            break;
        case 9600:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
            break;
        case 19200:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_19200);
            break;
        case 38400:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_38400);
            break;
        case 57600:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_57600);
            break;
        case 115200:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_115200);
            break;

        default:
            serial->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_57600);
    }

    if (!serial->good()) {
        std::cerr << "ERROR: could not set BAUD_RATE: " << SERIAL_BAUD_RATE << std::endl;
        return -1;
    }

    serial->SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
    if (!serial->good()) {
        std::cerr << "ERROR: could not set CHAR_SIZE" << std::endl;
        return -1;
    }

    /** **/

    /** **/

    serial->Close();
    delete serial;

    return 0;
}
