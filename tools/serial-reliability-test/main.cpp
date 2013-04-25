
#include <SerialStream.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>

int main (int argc, char** argv) {
    unsigned long count = 0;
    LibSerial::SerialStream serial;

    if (argc != 4) {
        std::cout << "usage: " << argv[0] << " <device> <baudrate> <count>" << std::endl;
        std::cout << "available baudrates: " << std::endl;
        std::cout << "\t9600" << std::endl;
        std::cout << "\t38400" << std::endl;
        std::cout << "\t115200" << std::endl;

        return -1;
    }
    else {
        serial.Open(argv[1]);
        if (serial.IsOpen()) std::cout << "serial stream is open ..." << std::endl;
        else {
            std::cerr << "could not open serial stream ..." << std::endl;
            return -1;
        }

        if (std::string(argv[2]) == std::string("9600")) serial.SetBaudRate( LibSerial::SerialStreamBuf::BAUD_9600 );
        else if (std::string(argv[2]) == std::string("38400")) serial.SetBaudRate (LibSerial::SerialStreamBuf::BAUD_38400 );
        else if (std::string(argv[2]) == std::string("115200")) serial.SetBaudRate (LibSerial::SerialStreamBuf::BAUD_115200);
        else {
            std::cerr << "invalid baudrate: " << argv[2] << std::endl;
            return -1;
        }

        std::string count_str = argv[3];
        std::stringstream ss;
        ss << argv[3];
        ss >> count;
    }

    unsigned long errors = 0;

    if (count < 1) {
        std::cerr << "count must not be NULL !" << std::endl;
        return -1;
    }

    for (unsigned long tries = 0; tries < count; tries ++) {
        time_t dtime = time(NULL);
        tm* tmtime = localtime(&dtime);
        unsigned char databyte = ((unsigned char)tmtime->tm_sec) % 255;
        serial.put(databyte);
        if (!serial.get() == databyte) {    // transmission error !!!
            errors++;
        }
        if ((tries % (tries/10)) == 0) std::cout << ".";
    }

    std::cout << "test complete ...\n error rate: " << (float(errors) / float(count)) * 100.0 << "%" << std::endl;

    return 0;
}
