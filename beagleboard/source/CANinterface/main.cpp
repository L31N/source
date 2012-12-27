
#include <iostream>
#include <boost/thread.hpp>

#include "BufferedAsyncSerial.h"

#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"
#include "ipc/buffer.h"
#include "ipc/ipc_config.h"

#include "bbdebug.h"

#include "can_config.h"

const std::string CAN_CONFIG_FILE_PATH = "../../../../etc/can.conf";

const std::string SERIAL_DEVICE_FILE = "/dev/ttyUSB0";
const unsigned int SERIAL_BAUD_RATE = 115200;

int main () {
     try {

        Debug debug ("CAN_INTERFACE_MODULE");
        debug.send("debug->initialisation");

        BufferedAsyncSerial serial(SERIAL_DEVICE_FILE.c_str(), SERIAL_BAUD_RATE);
        if (!serial.isOpen()) {
            std::cerr << "ERROR: could not open serial Stream to :" << SERIAL_DEVICE_FILE << std::endl;
            debug.send("ERROR: could not open serial stream to: %s", SERIAL_DEVICE_FILE.c_str());
        }

        ipcReceivingConnection ipcRCon ("CAN_INTERFACE_MODULE", 10);

        CANConfig cancfg(CAN_CONFIG_FILE_PATH.c_str());

        std::string serial_buffer;
        Data* ipc_buffer = NULL;
        while(true) {
            if (serial.readsome(serial_buffer, 11)) {      /// new serial message ...      /** asynchronous readsome function, must be called in loop ! **/
                debug.send("serial data received: %s", serial_buffer.c_str());
                std::cout << "data: " << serial_buffer << std::endl;

                //bool can_rtr = serial_buffer[0];
                unsigned short can_id = serial_buffer[1];
                unsigned short can_length = serial_buffer[2];
                char can_data[can_length];
                for (int i = 0; i < can_length; i++) can_data[i] = serial_buffer[3+i];

                std::string ipc_output = (char* )&can_id;
                ipc_output += can_data;

                std::string ipcSyn = cancfg.getIpcSynonym(can_id);
                ipcSendingConnection ipcSCon("CAN_INTERFACE_MODULE", ipcSyn, IPC_LOCAL);
                if (ipcSCon.is_open()) {
                    ipcSCon.sendData(ipc_output);
                }
                else {      /// could not open ipc_connection
                    std::cerr << "ERROR: could not open ipc_connection to: " << ipcSyn << std::endl;
                    debug.send("ERROR: could not open ipc_connection to: %s", ipcSyn.c_str());
                }
            }

            if (ipcRCon.checkForNewData()) {        /// new ipc data available ...
                ipc_buffer = ipcRCon.readDataFromBuffer();
                std::string data = ipc_buffer->getData();
                short senderID = ipc_buffer->getSenderID();

                debug.send("data: %s\tsenderID: %i", data.c_str(), senderID);
                std::cout << "data: " << data << "\tsenderID: " << senderID << std::endl;

                char command = data[0];

                data.erase(0,1);

                /*if (command == '') {       /// remote call
                    unsigned short can_id = cancfg.getCanID(data);
                    char serial_data[12];
                    serial_data[0] = 's';
                    serial_data[1] = 1;
                    serial_data[2] = can_id;
                    serial_data[3] = 0;
                    for (int i = 0; i < 8; i++) serial_data[4+i] = 0;   // sets the values of the CAN data to ZERO, only rtr is important !

                    serial.write(serial_data, 12);
                }*/
                if (command == 's') {  /// send data via CAN
                    unsigned short can_id = cancfg.getCanID(data);
                    char serial_data[12];
                    serial_data[0] = 's';
                    serial_data[1] = 0;
                    serial_data[2] = can_id;
                    serial_data[3] = 8;
                    for (int i = 0; i < 8; i++) serial_data[4+1] = 0;

                    serial.write(serial_data, 12);
                }
                else if (command == 'f') {  /// set filter
                    unsigned int num = data[0];
                    unsigned int mask = data[1];
                    unsigned int id = data[2];

                    char serial_data[12];
                    serial_data[0] = 'f';       /// 'f' for "set filter"
                    serial_data[1] = num;       /// num
                    serial_data[2] = id;        /// id
                    serial_data[3] = mask;      /// mask
                    serial_data[4] = 0;         /// rtr
                    for (int i = 0; i < 7; i++) serial_data[5+i] = 0;

                    serial.write(serial_data, 12);
                }
                else {
                    std::cerr << "ERROR: unknown command byte (IPC): " << command << std::endl;
                    debug.send("ERROR: unknown command byte (IPC): %c", command);
                }

            }
        }

        serial.close();
    }
    catch(boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}


