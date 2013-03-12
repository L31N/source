
// cpimcp2515.cpp

#include "spimcp2515.h"

SpiMcp2515::SpiMcp2515(const std::string spidev) {
    fd = open(spidev.c_str(), O_RDWR);
}

SpiMcp2515::~
