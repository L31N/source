
#ifndef _BB_DEBUG_HPP_
#define _BB_DEBUG_HPP_

#include <string>
#include <fstream>

#include "ipc/bbipc.h"
#include "ipc/buffer.h"
#include "ipc/config.h"
#include "ipc/connection.h"

#include <vector>

class DebugController {
    public:
        DebugController();
        ~DebugController();

        void send();

    private:
        std::vector<std::string> vector;
        bool checkObjectForDebug(short objectID);

    friend class DebugObject;
        private:
            void pushBack(std::string dbg_string);
};

class DebugObject {
    public:
        DebugObject(std::string _objectIdentifier, DebugController);
        ~DebugObject();

        void push();

    private:
        std::string objectIdentifier;
};


#endif // _BB_DEBUG_HPP_
