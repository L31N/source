
#ifndef LIB_IPC_MAIN_H
#define LIB_IPC_MAIN_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

int get_pid_by_pname (std::string _pname, std::string _pidlist);
std::string get_pname_by_pid (int _pid, std::string _pidlist);

#endif
