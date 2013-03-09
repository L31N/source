/* simple CAN application example 
 * 
 * open CAN and test the read(2) call
 * An calling option  decides if the CAN device is opened for
 * blocking or nonblocking read.
 *
 * Option: -H
 * Explaining the 'horch' like Format
 * 7884.098543       10/0x0000000a : bD ( 8): 55 02 03 04 05 06 07 aa 
 *   |               |      |         |   |    |
 *   |               |      |         |   |    +- data bytes, hex coded
 *   |               |      |         |   +- length information of CAN frame
 *   |               |      |         +- Frame information
 *   |               |      |         	 bD  - Base Frame Format, Data Frame 
 *   |               |      |         	 eD  - Extended Frame Format, Data Frame
 *   |               |      |         	 bR  - Base Frame Format, Remote Frame 
 *   |               |      |         	 eR  - Extended Format, Remote Frame
 *   |               |      +- CAN Frame ID in Hex
 *   |               +- CAN Frame ID in decimal
 *   +--- Time stamp: s.µs
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <can4linux.h>

#define STDDEV "can0"
#define COMMANDNAME "receive"
#define VERSION "1.4"

#define RXBUFFERSIZE 100

#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

int sleeptime            = 1000;	/* standard sleep time */
int debug                = FALSE;
int baud		 = -1;		/* dont change baud rate */
int blocking		 = TRUE;	/* open() mode */
int buffers		 = RXBUFFERSIZE;/* open() mode */
int listenmode		 = FALSE;	/* listen only mode */
int horchmode		 = FALSE;	/* Horch-Format mode */

/* ----------------------------------------------------------------------- */

void usage(char *s)
{
static char *usage_text  = "\
 Open CAN device and display read messages\n\
 Default device is /dev/can0. \n\
Options:\n\
-d   - debug On\n\
       swich on additional debugging\n\
-b baudrate (Standard uses value of /proc/sys/Can/baud)\n\
-n   - non-blocking mode (default blocking)\n\
-B n - buffer size used in non-blocking mode (100)\n\
-s sleep sleep in ms between read() calls in non-blocking mode\n\
-H   - output displayed in \'horch \' format\n\
-l     Listen only mode\n\
-V   version\n\
\n\
";
    fprintf(stderr, "usage: %s [options] [device]\n", s);
    fprintf(stderr, usage_text);
}



/***********************************************************************
*
* set_bitrate - sets the CAN bit rate
*
*
* Changing these registers only possible in Reset mode.
*
* RETURN:
*
*/

int	set_bitrate(
	int fd,			/* device descriptor */
	int baud		/* bit rate */
	)
{
Config_par_t  cfg;
volatile Command_par_t cmd;
int ret;


    cmd.cmd = CMD_STOP;
    ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    cfg.target = CONF_TIMING; 
    cfg.val1   = baud;
    ret = ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

    cmd.cmd = CMD_START;
    ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    if (ret < 0) {
	perror("set_bitrate");
	exit(-1);
    } else {
	ret = 0;
    }
    return ret;
}

/***********************************************************************
*
* set_lomode - sets the CAN in Listen Only Mode
*
*
* Changing these registers only possible in Reset mode.
*
* RETURN:
*
*/
int set_lomode(int fd)
{
Config_par_t  cfg;
volatile Command_par_t cmd;
int ret;

    cmd.cmd = CMD_STOP;
    ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    cfg.target = CONF_LISTEN_ONLY_MODE; 
    cfg.val1   = 1;
    ret = ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

    cmd.cmd = CMD_START;
    ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    if (ret < 0) {
	perror("set_lomode");
	exit(-1);
    } else {
	ret = 0;
    }
    return ret;


}

/***********************************************************************
*
* main - 
*
*
*/

int main(int argc,char **argv)
{
int fd;
int got;
int c;
char *pname;
extern char *optarg;
extern int optind;

canmsg_t rx[RXBUFFERSIZE];
char device[50];
int messages_to_read = 1;

    pname = *argv;

    /* parse command line */
    while ((c = getopt(argc, argv, "B:b:dD:hHls:nV")) != EOF) {
	switch (c) {
	    case 'b':
		baud = atoi(optarg);
		break;
	    case 'B':
	        buffers = atoi(optarg);
	    case 's':
		sleeptime = atoi(optarg);
		break;
	    case 'd':
		debug = TRUE;
		break;
	    case 'H':
		horchmode = TRUE;
		break;
	    case 'l':
		listenmode = TRUE;
		break;
	    case 'n':
		blocking = FALSE;
		messages_to_read = RXBUFFERSIZE;
		break;
		break;
	    case 'V':
		printf("%s %s\n", argv[0], " V " VERSION ", " __DATE__ );
		exit(0);
		break;

		/* not used, devicename is parameter */ 
	    case 'D':
		if (
		    /* path ist starting with '.' or '/', use it as it is */
			optarg[0] == '.'
			|| 
			optarg[0] == '/'
			) {
		    sprintf(device, "%s", optarg);

	        } else {
		    sprintf(device, "/dev/%s", optarg);
		}
		break;
	    case 'h':
	    default: usage(pname); exit(0);
	}
    }

    /* look for additional arguments given on the command line */
    if ( argc - optind > 0 ) {
        /* at least one additional argument, the device name is given */
        char *darg = argv[optind];

	if (
	    /* path ist starting with '.' or '/', use it as it is */
		    darg[0] == '.'
		    || 
		    darg[0] == '/'
		    ) {
		sprintf(device, "%s", darg);
	} else {
	sprintf(device, "/dev/%s", darg);
	}
    } else {
	sprintf(device, "/dev/%s", STDDEV);
    }

    if ( debug == TRUE ) {
	printf("%s %s\n", argv[0], " V " VERSION ", " __DATE__ );
	printf("(c) 1996-2006 port GmbH\n");
	printf(" using canmsg_t with %d bytes\n", (int)sizeof(canmsg_t));
	printf(" CAN device %s opened in %sblocking mode\n",
		device, blocking ? "" : "non-");
    }

    sleeptime *= 1000;
    
    if(blocking == TRUE) {
	/* fd = open(device, O_RDWR); */
	fd = open(device, O_RDONLY);
    } else {
	fd = open(device, O_RDONLY | O_NONBLOCK);
    }
    if( fd < 0 ) {
	fprintf(stderr,"Error opening CAN device %s\n", device);
	perror("open");
	exit(1);
    }
    if (baud > 0) {
	if ( debug == TRUE ) {
	    printf("change Bit-Rate to %d Kbit/s\n", baud);
	}
	set_bitrate(fd, baud);
    }
    if (listenmode == TRUE) {
	if ( debug == TRUE ) {
	    printf(" Change to Listen Only Mode\n");
	}
	set_lomode(fd);
    }

    /* printf("waiting for msg at %s\n", device); */

    while(1) {
      got=read(fd, &rx, messages_to_read);
      if( got > 0) {
        int i;
        int j;
        for(i = 0; i < got; i++) {
            if(horchmode) {
		printf("%12lu.%06lu %8ld/0x%08lx",
			rx[i].timestamp.tv_sec,
			rx[i].timestamp.tv_usec,
			rx[i].id, rx[i].id);
            } else {
		printf("Received with ret=%d: %12lu.%06lu id=%ld/0x%08lx\n",
			got, 
			rx[i].timestamp.tv_sec,
			rx[i].timestamp.tv_usec,
			rx[i].id, rx[i].id);

		printf("\tlen=%d", rx[i].length);
		printf(" flags=0x%02x", rx[i].flags );
            }
	    printf(" : %c%c (%2d):",
	    		(rx[i].flags & MSG_EXT) ? 'e' : 'b', /* extended/base */
	    		(rx[i].flags & MSG_RTR) ? 'R' : 'D', /* remote/data */
	    		rx[i].length );
	    if( !(rx[i].flags & MSG_RTR) ) {
		for(j = 0; j < rx[i].length; j++) {
		    printf(" %02x", rx[i].data[j]);
		}
	    }
	    if(horchmode && (rx[i].id == -1)) {
	    unsigned char f = ' '; 
		printf(" Error flags=0x%03x\n\tError: ", rx[i].flags);
		if(rx[i].flags & MSG_OVR) {
		    printf("%c CAN controller msg overflow", f);
		    f = ',';
		}
		if(rx[i].flags & MSG_PASSIVE) {
		    printf("%c CAN Error Passive", f);
		    f = ',';
		}
		if(rx[i].flags & MSG_BUSOFF) {
		    printf("%c CAN controller Bus off", f);
		    f = ',';
		}
		if(rx[i].flags & MSG_WARNING) {
		    printf("%c CAN controller Error Warnig Level reached", f);
		    f = ',';
		}
		if(rx[i].flags & MSG_BOVR) {
		    printf("%c can4linux rx/tx buffer overflow", f);
		    f = ',';
		}
	    }
	    printf("\n"); fflush(stdout);
	}
      } else {
	printf("Received with ret=%d\n", got);
	fflush(stdout);
      }
      if(blocking == FALSE) {
	  /* wait some time before doing the next read() */
	  usleep(sleeptime);
      }
    }

    close(fd);
    return 0;
}
