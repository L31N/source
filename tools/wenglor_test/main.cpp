
#include <iostream>
#include <cstdlib>

#include <SerialStream.h>

using namespace std;
using namespace LibSerial;

int main()
{
	unsigned char sdata[32] = { 0x24, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x2E, 0x3B };

	SerialStream serial;
	serial.Open("/dev/ttyUSB0");
	serial.SetBaudRate( SerialStreamBuf::BAUD_38400 );
	//serial.SetBaudRate( SerialStreamBuf::BAUD_115200);


    while(true) {
        //Serial schreiben
        for(unsigned int i = 0; i< 32; i++)
        {
            serial.put(sdata[i]);
        }

        //Serial lesen
        unsigned char rdata[4];

        for (int i = 0; i < 68; i++) {
            unsigned char buf = serial.get();
            printf("%x", buf);
            if (i >= 36 && i <= 39) rdata[i-36] = buf;
        }
        printf("\n");

        unsigned int length = 0;
        for (int i = 0; i < 4; i++) length |= (rdata[i] << 8*i);
        std::cout << "length: " << length << std::endl;
        //sleep(1);
        usleep(36 * 1000);
        //usleep(100 * 1000);
    }

	serial.Close();

    return 0;
}
