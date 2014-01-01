
#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cstdint>
#include <vector>
#include <map>

class CanDatabase;

class CANConfig {
    public:
        CANConfig(const std::string CAN_CONFIG_FILE_PATH);
        ~CANConfig();

        unsigned short getCanID(std::string can_member);
        std::string getCanMember(unsigned short can_id);
        std::string getIpcSynonym(unsigned short can_id);
        std::string getIpcSynonym(std::string can_member);

    private:
        //std::ifstream* ifs;
        //std::string CONFIG_FILE_PATH;

        CanDatabase* candb;
};

class CanDatabase {
public:

	struct CanMember
	{
		std::string canName;
		std::string ipcSynonym;
		unsigned short canID;
	};


	CanDatabase();
	~CanDatabase();

	bool init(const std::string filename = "test.txt");

	unsigned short getCanID(const std::string _canName);
	std::string getCanName(const unsigned short _canID);

	std::string getIpcSynonym(unsigned short canId);
	std::string getIpcSynonym(const std::string canName);

protected:

	std::map<unsigned short, CanMember> *canmap;
};



#endif  // _CAN_CONFIG_H_
