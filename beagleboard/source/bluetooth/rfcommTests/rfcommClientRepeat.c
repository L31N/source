#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <unistd.h>

int main(int argc, char **argv)
{
    while(1) {

    //usleep(1000);

    struct sockaddr_rc addr = { 0 };
    int s, status;
    char* dest = *(argv+1);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
	if( status == 0 ) {
        	//while(1) {
			status = write(s, "hello!", 6);
		//}
    	}
	else if (status < 0) perror("send data failed !\n");

    //if( status < 0 ) perror("uh oh");

    close(s);

    } // end of while

    usleep(100000);
 //   } // end of while
    return 0;
}

