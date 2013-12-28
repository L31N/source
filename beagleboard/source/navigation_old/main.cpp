
#include <iostream>
#include <cmath>

#include "bbvector.h"
#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"
#include "bbcan.h"
#include "bbdebug.h"
#include "navigationController.h"

#include "navigationController.h"

int main () {
<<<<<<< HEAD
    Navigation navi;
    navi.run();
=======
    NavigationController navi;

    navi.getPosition().print();
>>>>>>> 40599afceee25a4c17881d2e27b4d05ff1b47913

    return 0;
}
