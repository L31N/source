
#include "can_config.h"

/*CANConfig::CANConfig(const std::string CAN_CONFIG_FILE_PATH) {
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

    std::cout << "getCanID(std::string can_member)" << std::endl;

    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            line = line.substr(0, line.find_first_of('#'));
            while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t') {
                line.erase(line.size()-1);
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

    std::cout << "getCanMember(unsigned short can_id)" << std::endl;

    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            line = line.substr(0, line.find_first_of('#'));
            while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t') {
                line.erase(line.size()-1);
            }
        }

        size_t firstDP = line.find_first_of(':');
        size_t lastDP = line.find_last_of(':');

        std::string can_mem = line.substr(0, firstDP);
        std::string str_can_id = line.substr(lastDP+1, std::string::npos);

        std::cout << "str_can_id:" << str_can_id << ":" << std::endl;

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
    ifs->seekg(0, ifs->beg);

    std::cout << "getIpcSynonym(" << can_id << ")" << std::endl;

    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            line = line.substr(0, line.find_first_of('#'));
            while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t') {
                line.erase(line.size()-1);
            }
        }


        size_t firstDP = line.find_first_of(':');
        size_t lastDP = line.find_last_of(':');

        std::string ipc_syn = line.substr(firstDP+1, lastDP-firstDP-1);
        std::string str_can_id = line.substr(lastDP+1, std::string::npos);

        std::cout << "str_can_id:" << str_can_id << ":";

        std::stringstream ss;
        ss << str_can_id;
        unsigned short id;
        ss >> id;

        std::cout << id << std::endl;

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

    std::cout << "getIpcSynonym(std::string can_member)" << std::endl;

    while(!ifs->eof()) {
        char cline[256];
        ifs->getline(cline, 256);
        std::string line(cline);
        if (line[0] == '#' || line.length() <= 0) continue;       /// its a comment line or a blank line
        else {                                                     /// cut off the blanks at the end of line
            line = line.substr(0, line.find_first_of('#'));
            while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t') {
                line.erase(line.size()-1);
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
}*/

CANConfig::CANConfig(const std::string CAN_CONFIG_FILE_PATH) {
    candb = new CanDatabase();
    candb->init(CAN_CONFIG_FILE_PATH);
}

CANConfig::~CANConfig() {
    delete candb;
}

unsigned short CANConfig::getCanID(std::string can_member) { return candb->getCanID(can_member); }
std::string CANConfig::getCanMember(unsigned short can_id) { return candb->getCanName(can_id); }
std::string CANConfig::getIpcSynonym(unsigned short can_id) { return candb->getIpcSynonym(can_id); }
std::string CANConfig::getIpcSynonym(std::string can_member) { return candb->getIpcSynonym(can_member); }


CanDatabase::CanDatabase()
{
	canmap = 0;
}

CanDatabase::~CanDatabase()
{
	delete canmap;
}

bool CanDatabase::init(const std::string filename)
{
	std::ifstream input(filename.c_str());
	if (!input.is_open())
		return 1;

	std::string line;

	std::map<unsigned short, CanMember> *temp = new std::map<unsigned short, CanMember>();

	while (!input.eof())
	{
		std::getline(input, line);

		line = line.substr(0, line.find_first_of('#'));
		while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t') line.erase(line.size()-1);

		size_t pos0 = line.find_first_of(':');
		size_t pos1 = line.find_last_of(':');

		if (pos0 == std::string::npos || pos1 == std::string::npos)
		{
			//std::cerr << "invalid line\n";
			continue; //invalid line
		}

		std::string part0 = line.substr(0, pos0);
		std::string part1 = line.substr(pos0+1, pos1 - pos0 - 1);
		std::string part2 = line.substr(pos1+1, std::string::npos);

		std::stringstream ss;
		ss << part2;

		unsigned short num;
		ss >> num;

		CanMember member;
		member.canName = part0;
		member.ipcSynonym = part1;
		member.canID = num;

		//std::cout << "canName: " << member.canName << "\tipcSynonym: " << member.ipcSynonym << "\tcanID: " << num << std::endl;

		(*temp)[num] = member;
	}


	delete canmap;
	canmap = temp;

	input.close();

	return 0;
}

unsigned short CanDatabase::getCanID(const std::string _canName) {
    for (unsigned int i = 0; i < canmap->size(); i++) {
        if (canmap->operator[](i).canName == _canName) {
            return (*canmap)[i].canID;
        }
    }
    return 0;
}

std::string CanDatabase::getCanName(unsigned short _canID) {
    if (canmap->find(_canID) == canmap->end())
        return std::string();
    return (*canmap)[_canID].canName;
}

std::string CanDatabase::getIpcSynonym(unsigned short _canID)
{
	if (canmap->find(_canID) == canmap->end())
		return std::string();
	return (*canmap)[_canID].ipcSynonym;
}

std::string CanDatabase::getIpcSynonym(const std::string _canName)
{
    for (unsigned int i = 0; i < canmap->size(); i++) {
        if (canmap->operator[](i).canName == _canName) {
            return (*canmap)[i].ipcSynonym;
        }
    }
    return std::string();
}
