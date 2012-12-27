
#include <iostream>
#include <boost/thread.hpp>

#include "BufferedAsyncSerial.h"

#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"
#include "ipc/buffer.h"
#include "ipc/ipc_config.h"

#include "bbdebug.h"

const std::string SERIAL_DEVICE_FILE = "/dev/ttyUSB0";
const unsigned int SERIAL_BAUD_RATE = 115200;

int main () {
     try {
        BufferedAsyncSerial serial(SERIAL_DEVICE_FILE.c_str(), SERIAL_BAUD_RATE);

        ipcReceivingConnection ipcRCon ("CAN_INTERFACE_MODULE", 10);
        ipcSendingConnection ipcSensors ("CAN_INTERFACE_MODULE", "SENSOR_DATA_MODULE", IPC_LOCAL);
        ipcSendingConnection ipcMotors ("CAN_INTERFACE_MODULE", "MOTOR_CONTROL_MODULE", IPC_LOCAL);

        Debug debug ("CAN_INTERFACE_MODULE");


        std::string buffer;
        while(true) {
            if (serial.readsome(buffer, 12)) {      /// new serial message ...      /** asynchronous readsome function, must be called in loop ! **/
                std::cout << "data: " << buffer << std::endl;
                buffer = "";
            }

            if (ipcRCon.checkForNewData()) {        /// new ipc data available ...

            }
        }

        serial.close();
    }
    catch(boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}


