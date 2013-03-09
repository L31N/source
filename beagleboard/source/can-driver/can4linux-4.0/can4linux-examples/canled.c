/* simple driver test: control on-board LEDs with ioctl()
* 
* The firat board supporting this is CC_PCICAN (Contemporary Controls)
*
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <can4linux.h>

#define STDDEV "can0"


void usage(char *pname)
{
    printf("usage: %s dev ledcolor on|off\n", pname);
    printf("   e.g.:\n");
    printf("   %s /dev/can0 green on\n", pname);
    printf("\nTypical LED colors found on boards are: red,green,yellow\n");
}


/*
*
*
*/
int main(int argc,char **argv)
{
int can_fd;
char device[40];
enum can_led_color color;
enum can_led_state ledstate;
Command_par_t cmd;

    if(argc < 4) { 
	usage(argv[0]);
	exit(1);
    }

    if(0 == strncmp("green", argv[2], 5)) {
       printf("=> green\n");
       color = green;
    }
    else 
    if(0 == strncmp("yellow", argv[2], 6)) {
       printf("=> yellow\n");
       color = yellow;
    }
    else 
    if(0 == strncmp("red", argv[2], 3)) {
       printf("=> red\n");
       color = red;
    ;
    }
    else {
	usage(argv[0]);
	exit(1);
    }

    /* Now checking the state for on/off */
    if(0 == strncmp("on", argv[3], 2)) {
	ledstate = on;
    }
    else 
    if(0 == strncmp("off", argv[3], 3)) {
	ledstate = off;
    }
    else {
	usage(argv[0]);
	exit(1);
    }


    /* all parameters are checked */
    /* open the driver */
    sprintf(device, "%s", argv[1]);
    printf("using CAN device %s\n", device);
    if(( can_fd = open(device, O_RDWR )) < 0 ) {
	fprintf(stderr,"Error opening CAN device %s\n", device);
        exit(1);
    }

    cmd.cmd  = CMD_CTRL_LED;
    cmd.val1 = color;
    cmd.val2 = ledstate;
    ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);


    close(can_fd);
    return 0;
}

