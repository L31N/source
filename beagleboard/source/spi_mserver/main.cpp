
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "spimcp2515.h"
#include "spi_mserver.h"

int main (int argc, char** argv) {

    SpiMServer ms;
    ms.run();

    return 0;
}
