
#include "libipc.h"

int get_pid_by_pname (std::string _pname, std::string _pidlist) {
    std::ifstream is (_pidlist.c_str(), std::ifstream::in);
    if (!is.is_open()) return -1;

    std::string buf;
    char cbuf[256] = {'\0'};
    std::string str_pid;
    int pid;

    while(!is.eof()) {
        is.getline(cbuf, 256);
        std::cout << cbuf << std::endl;
        if (*cbuf == '#') continue;
        buf = cbuf;
        unsigned int found_pos = buf.find(_pname);
        std::cout << "find_return: " << found_pos << std::endl;
        if (found_pos != std::string::npos) {     /// pname found at this line
            for(unsigned int i = 0; i < found_pos; i++) {
                if (buf[i] != ' ') {
                    str_pid += buf[i];
                }
            }
            if (str_pid != "") {
                std::istringstream iss(buf);
                iss >> pid;
                return pid;
            }
            else return 0;
        }
    }
    return -1;
}

std::string get_pname_by_pid (int _pid, std::string _pidlist) {
    std::ifstream is ((const char*)_pidlist.c_str(), std::ifstream::in);
    if (!is.is_open()); return std::string("");

    std::string buf;
    char* cbuf = '\0';

    std::string pname;

    std::ostringstream oss;
    oss << _pid;
    std::string pid = oss.str();

    while(!is.eof()) {
        is.getline(cbuf, 256);
        if (*cbuf == '#') continue;
        buf = std::string(cbuf);

        unsigned int found_pos = buf.find(pid);

        if (found_pos != std::string::npos) {
            pname = buf.substr(found_pos + 1);
            return pname;
        }
        else return 0;
    }
    return std::string("");
}



