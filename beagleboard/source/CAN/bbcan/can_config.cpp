
#include "can_config.h"

CANConfig::CANConfig(const std::string CAN_CONFIG_FILE_PATH) {
    CONFIG_FILE_PATH = CAN_CONFIG_FILE_PATH;
    ifs = new std::ifstream(CONFIG_FILE_PATH.c_str(), std::ios_base::in);

    if(!ifs->is_open()) {
        std::cerr << "error cannot open ifstream to: " << CONFIG_FILE_PATH << std::endl;
        return;
    }
}

CANConfig::~CANConfig() {
    ifs->close();
    delete ifs;
}

unsigned short CANConfig::getCanID(std::string can_member) {
    ifs->clear();
    ifs->seekg(0);
    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            size_t epos = line.find_first_of(' ');
            if (epos == std::string::npos) epos = line.find_first_of('#');

            if (epos != std::string::npos) {
                line = line.substr(0, epos);
            }
        }

        size_t firstDP = line.find_first_of(':');
        size_t lastDP = line.find_last_of(':');

        std::string can_mem = line.substr(0, firstDP);
        std::string can_id = line.substr(lastDP+1, std::string::npos);

        if (can_mem == can_member) {
            std::stringstream ss;
            ss << can_id;
            unsigned short id;
            ss >> id;
            return id;
        }
    }
    std::cerr << "ERROR: used unknown can_member: " << can_member << std::endl;
    return -1;
}

std::string CANConfig::getCanMember(unsigned short can_id) {
    ifs->clear();
    ifs->seekg(0);
    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            size_t epos = line.find_first_of(' ');
            if (epos == std::string::npos) epos = line.find_first_of('#');

            if (epos != std::string::npos) {
                line = line.substr(0, epos);
            }
        }


        size_t firstDP = line.find_first_of(':');
        size_t lastDP = line.find_last_of(':');

        std::string can_mem = line.substr(0, firstDP);
        std::string str_can_id = line.substr(lastDP+1, std::string::npos);

        std::stringstream ss;
        ss << str_can_id;
        unsigned short id;
        ss >> id;

        if (id == can_id) {
            return can_mem;
        }
    }
    std::cerr << "ERROR: used unknown can_id: " << can_id << std::endl;
    return "";
}

std::string CANConfig::getIpcSynonym(unsigned short can_id) {
    ifs->clear();
    ifs->seekg(0);

    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            size_t epos = line.find_first_of(' ');
            if (epos == std::string::npos) epos = line.find_first_of('#');

            if (epos != std::string::npos) {
                line = line.substr(0, epos);
            }
        }


        size_t firstDP = line.find_first_of(':');
        size_t lastDP = line.find_last_of(':');

        std::string ipc_syn = line.substr(firstDP+1, lastDP-firstDP-1);
        std::string str_can_id = line.substr(lastDP+1, std::string::npos);

        std::stringstream ss;
        ss << str_can_id;
        unsigned short id;
        ss >> id;

        if (id == can_id) {
            return ipc_syn;
        }
    }
    std::cerr << "ERROR: used unknown can_id: " << can_id << std::endl;
    return "";
}

std::string CANConfig::getIpcSynonym(std::string can_member) {
    ifs->clear();
    ifs->seekg(0);

    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            size_t epos = line.find_first_of(' ');
            if (epos == std::string::npos) epos = line.find_first_of('#');

            if (epos != std::string::npos) {
                line = line.substr(0, epos);
            }
        }


        size_t firstDP = line.find_first_of(':');
        size_t lastDP = line.find_last_of(':');

        std::string can_mem = line.substr(0, firstDP);
        std::string ipc_syn = line.substr(firstDP+1, lastDP-firstDP-1);

        if (can_mem == can_member) {
            return ipc_syn;
        }

    }
    std::cerr << "ERROR: used unknown can_member: " << can_member << std::endl;
    return "";
}
