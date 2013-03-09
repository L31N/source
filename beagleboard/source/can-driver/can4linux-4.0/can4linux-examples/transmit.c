/*   Transmit sends a bunch of telegrams by filling them with		*/
/*   different CANids and data bytes. You may send more than one	*/
/*   telegram with write just by sending out an array of telegrams.	*/
/*   call it: 								*/
/*    transmit								*/
/*    transmit can0							*/
/*    transmit can0 0x200						*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <can4linux.h>

#define STDDEV		"can1"  /* used without program arguments    */
#define STARTID		0x100   /* start all frames sent with this Id */




/*
* Show CAN errors as text, 
* // in case of Bus-Off, try to recover
*/
static void displayerror(canmsg_t *rx)
{
/* static int buffcnt = 0; */

	printf("Flags 0x%02x,", rx->flags);
	if( rx->flags & MSG_OVR) {
		printf(" CAN Controller Buffer Overflow,");
	}
	if( rx->flags & MSG_BOVR) {
		printf(" Driver Buffer Overflow,");
	}
	if( 0 == (rx->flags & MSG_ERR_MASK)) {
		printf(" CAN Error Free");
	}
	if( rx->flags & MSG_WARNING) {
		printf(" CAN Warning Level,");
	}
	if( rx->flags & MSG_PASSIVE) {
		printf(" CAN Error Passive,");
	}
	if( rx->flags & MSG_BUSOFF) {
		printf(" CAN Bus Off,");
		/* can_reset(); */
		/* sleep 100ms */
		/* usleep (100000); */
		/* can_start(); */
	}
	printf("\n");
}


/*
 * Read the content of the RX queuee
 * but only evaluate so called error-messages from the driver
 * which can id is coded as -1.
 * Such messages are than interpretet by displayerror()
 */
void read_can(int can_fd)
{
canmsg_t rx;
int i;
	    
	/* now read the rx queue, it may contain
	 * frames with id -1, which signals an error
	 * read() does block */
	do {
	    i = (int)read(can_fd, &rx, 1);
	    if(-1 == i) {
		perror("read error");
	    } else if((i > 0) && (CANDRIVERERROR == rx.id)) {
		displayerror(&rx);
	    }
	}
	while(i > 0);  /* do {} while(); */
}


/**
*
* The main program
*
*/
int main(int argc,char **argv)
{
unsigned int fd;
int i, sent;
canmsg_t tx[256];
char device[40];
int id = STARTID;

    if(argc > 1) {
	sprintf(device, "/dev/%s", argv[1]);
    }
    else {
	sprintf(device, "/dev/%s", STDDEV);
    }
    if(argc == 3) {
    	id = strtoul(argv[2], NULL, 0);
    }

    printf("using CAN device %s, id = 0x%X\n", device, id);
    
    /* Open the CAN device for blocking read/write operations */ 
    if(( fd = open(device, O_RDWR )) < 0 ) {
	fprintf(stderr,"Error opening CAN device %s\n", device);
        exit(1);
    }

    for(i = 0; i < 16; i++) {
      sprintf( (char *)tx[i].data, "msg%d", i);
      /* printf("sending '%s'\n", (char *)tx[i].data ); */
      tx[i].flags = 0;  
      tx[i].length = strlen((char *)tx[i].data);  
      tx[i].id= id + i;

    }

    if (0 == fork()) {
    /* child does check the rx queue for errors */ 
	printf("checking RX for errors\n");


	    /* now read the rx queue, it may contain
	     * frames with id -1, which signals an error */
	     while(1) read_can(fd);

    } else {
    /* Parent process is transmitting CAN frames */
	while (1) {
	    /* printf("fast xmit message\n"); */

	    /* to prevent RX overflow and to see CAN errors
	     * the fd hat to be read after the write()
	     */
	    sent = write(fd, tx, 16 );
	    if(sent <= 0) {
		perror("sent");
		close(fd);
		exit(1);
	    }
	    usleep(1000);
	}
    }
    return 0;
}

