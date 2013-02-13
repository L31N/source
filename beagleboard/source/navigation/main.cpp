
#include <iostream>
#include <cmath>

#include "bbvector.h"
#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"
#include "bbcan.h"
#include "bbdebug.h"

#include "navigationController.h"

int main () {
    NavigationController navi;

    navi.getPosition().print();

    return 0;
}
