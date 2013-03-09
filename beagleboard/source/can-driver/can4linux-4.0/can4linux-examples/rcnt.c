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

#define VERSION "$Id:$"

#define RXBUFFERSIZE 100

#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

int sleeptime            = 1000;	/* standard sleep time */
int debug                = FALSE;
int baud		 = -1;		/* dont change baud rate */
int blocking		 = TRUE;	/* open() mode */
long can_id              = 100;
long expected_msgs       = 1000;

/* ----------------------------------------------------------------------- */

void usage(char *s)
{
static char *usage_text  = "\
Open CAN device and count messages read from the CAN device \
with the given CAN-Id. The first 4 bytes are interpreted as \
sequence number just as the command: can_send -t12 -s10 -T0.\n\
Default CAN-Id is 100. \n\
Options:\n\
-d   - debug On\n\
       swich on additional debugging\n\
-D     CAN device default: /dev/can0\n\
-b     baudrate (Standard uses value of /proc/sys/Can/baud)\n\
-n     number of expected messages; default: 1000 \n\
-s     sleep sleep in ms between read() calls in non-blocking mode\n\
-V     version\n\
\n\
";
    fprintf(stderr, "usage: %s [options] <CAN-Id> \n", s);
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
long last_seq;
long diff;
long num_msgs;

    pname = *argv;
    can_id = 100;
    snprintf(device, 50, "/dev/can0");

    /* parse command line */
    while ((c = getopt(argc, argv, "b:dD:hs:n:V")) != EOF) {
	switch (c) {
	    case 'b':
		baud = atoi(optarg);
		break;
	    case 's':
		sleeptime = atoi(optarg);
		break;
	    case 'd':
		debug = TRUE;
		break;
	    case 'n':
		expected_msgs = atoi(optarg);
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
    	can_id = strtol(argv[optind], NULL, 0);
    }

    if ( debug == TRUE ) {
	printf("%s %s\n", argv[0], " V " VERSION ", " __DATE__ __TIME__);
	printf("(c) 1996-2007 port GmbH\n");
	printf(" using canmsg_t with %d bytes\n", sizeof(canmsg_t));
	printf(" CAN device %s opened in %sblocking mode\n",
		device, blocking ? "" : "non-");

    }

    sleeptime *= 100;
    
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

    printf("waiting for %ld messages at %s with can id %ld/0x%lx\n",
    		expected_msgs, device, can_id, can_id);

    last_seq = 0;
    num_msgs = 0;
    while(1) {
      got=read(fd, &rx, messages_to_read);
      if( got > 0) {
        long seq;
        int i;

        for(i = 0; i < got; i++) {
        /* printf("got id: %ld, %d\n", rx[i].id, num_msgs); */
      /* printf(" %0lx",rx[i].id ); fflush(stdout); */

	    if (can_id == rx[i].id) {
      /* printf("+"); fflush(stdout); */
	        /* got the frae I'm waiting for */
		num_msgs++;	/* increment received messages */
		if ((expected_msgs != 0) && (num_msgs == expected_msgs)) {
		    goto out;
		}

		seq = *(long *) &rx[i].data[0];
		diff = seq - last_seq;
		if ((diff != 0) && (diff != 1)) {
		    fprintf(stderr, "\n%4ld messages lost, "
		    	"current seq: %ld/0x%lx, last seq: %ld/0x%lx\n",
		    	diff, seq, seq, last_seq, last_seq);
		}
		last_seq = seq;
	    }
	    if ((rx[i].flags & MSG_ERR_MASK) != 0) {
		printf("   flags=0x%02x\n", rx[i].flags );
	    }
      }
      if(blocking == FALSE) {
	  /* wait some time before doing the next read() */
	  usleep(sleeptime);
      }
    } else {
    	printf("     receive error got=%d\n", got);
    }
  }
out:
    close(fd);
    return 0;
}
