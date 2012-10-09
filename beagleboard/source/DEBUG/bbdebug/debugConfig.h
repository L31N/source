
#ifndef _DEBUG_CONFIG_H_
#define _DEBUG_CONFIG_H_

#include <string>

class debugConfig {
    public:
        debugConfig(std::string _DEBUG_CONFIG_FILE);
        ~debugConfig();

    private:
        std::string DEBUG_CONFIG_FILE;
}

#endif // _DEBUG_CONFIG_H_
