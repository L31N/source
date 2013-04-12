
#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class CANConfig {
    public:
        CANConfig(const std::string CAN_CONFIG_FILE_PATH);
        ~CANConfig();

        unsigned short getCanID(std::string can_member);
        std::string getCanMember(unsigned short can_id);
        std::string getIpcSynonym(unsigned short can_id);
        std::string getIpcSynonym(std::string can_member);

    private:
        std::ifstream* ifs;
        std::string CONFIG_FILE_PATH;
};

#endif  // _CAN_CONFIG_H_
