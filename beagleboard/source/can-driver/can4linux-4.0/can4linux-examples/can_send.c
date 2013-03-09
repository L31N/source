/* can_send - can4linux can driver test program
*
*  send messages commissioned via command line
* 
* To do:
* - more sequences for -t 
* - -debug schaltet mit debug level auch Treiber in debug mode
*   long options --debug ?
* - ..
* - do configuration of test with a special file format (can_send.rc)
* - use nanosleep() see test3() ?
*
* - CAN Errors are shown only in the basic mode.
*   use it in all test modes
*   and make an automatic reset in case of bus-off configurable
*
* all activities has normally be finished before the program can exit
* and close the driver. Typically the transmit buffer is filled.
* The program has to wait for TX buffer empty before exiting.
* At the moment the program uses some sleep() only.
*
* July 2012
* running out of options - switched to long option names
* the most often used and traditional old options stay as they are (r,e,b,D,T,s,..)
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>

#ifdef USE_RT_SCHEDULING
# include <sched.h>
# include <sys/mman.h>
#endif

#include <errno.h>
#include "can4linux.h"


#if defined(EMBED)
# if defined(COLDFIRE)
/* IGW900 */
#  define STDDEV "/home/bin/can0"
# else
#  define STDDEV "/dev/can0"
# endif
#else
# define STDDEV "/dev/can1"
#endif
#define VERSION "SVN $Revision: 139 $"

#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

#define MAXDEVNAME 40		/* max characters for CAN device name */
#define DEFAULT_CANFD_LEN   16  /* Only: 8,12,16,20,24,32,48,64 are allowed */

extern int errno;
static canmsg_t message;
static int can_fd		= 0;    /* CAN device file descriptor, used global */ 
static int canfd		= 0;    /* use more than 8 bytes for CAN FD */ 
					/* if != 0 contains number of max bytes */
static unsigned int node        = 8;
static bool debug               = false;
static int extd                 = FALSE;
static int load			= 50;		/* default bus-load */
static int doload		= FALSE;		/* default bus-load */
static int stresstest           = FALSE;
static int testtype             = 1;  
static int sleeptime            = 1;		/* default: 1ms */
static int errwaittime		= 1000;	/* waittime after error */
static int cstdout              = FALSE;	/* use stdout for CAN message */
static unsigned long int test_count_soll = 1;	/* a single message */
static int rtr                  = FALSE;
static int baud			= -1;		/* dont change baud rate */
static int canreset		= FALSE;
static int endless		= 0;		/* endlees test */
static unsigned long errcnt	= 0;		/* global error counter */
#ifdef USE_RT_SCHEDULING
static int priority		= -1;		/* dont change priority rate */
#endif
/* different input formats for data frame */
static int isfloat		= FALSE;	/* if TRUE, arguments
						   are float arguments */
static int isshortinteger	= FALSE;	/* true if arguments are 2 byte
						   little endian short int */

/* functions */
/*@unused@*/ 
/* static void clean_process(void); */
static void test1(void);
static void test2(void);
static void test3(void);
static void test10(void);
static void test11(void);
static void test12(void);
static void test20(void);
static void test30(void);
static void test31(void);
static void test40(void);
static void test100(void);
static void test101(void);

/* show buffer in hex */
static void sbuf(unsigned const char *s, int n)
{
int i;

    for(i = 0; i < n; i++) {
	fprintf(stdout, "%02x ", (unsigned int)*s++);
    }
    putchar('\n');
}

/* convert byte number to a valid data length code 
   consider the global var canfd, containing the max length
 
    dlc	    bytes
    9	    12
    10	    16
    11	    20
    12	    24
    13	    32
    14	    48
    15	    64 */
unsigned int len2dlc(unsigned int cnt)
{   
unsigned  int i;
static const unsigned int  t[7][2] = {
	{  9, 12},
	{ 10, 16},
	{ 11, 20},
	{ 12, 24},
	{ 13, 32},
	{ 14, 48},
	{ 15, 64}
    };

    if( cnt <= 8) {
	return cnt;
    }
    if( cnt > canfd) {
	 cnt = canfd;
    }
    for(i = 0; i <= 7; i++) {
	if( t[i][1] >= cnt) {
	    break;
	}
    }
    return  t[i][0];
}

void printerrorcount(void)
{
    if(errcnt) {
	printf("got %ld CAN errors\n", errcnt);
    }
}

static void usage(char *s)
{
static const char *usage_text  = "\
 if optional id is not specified, 1365/0x555 is used\n\
 and 8 data bytes are used to construct a message.\n\
 Id and data can be given decimal 1365 or hex 0x555\n\
Options:\n\
-r --rtr      send message as rtr message.\n\
-e --ext      send message in extended message format.\n\
-l --load     load try to reach this bus load, given in %\n\
-s --sleep n  sleeptime between messages in ms, if not specified send single message\n\
-d --debug=   - debug On, with value set it in /proc/sys/dev/Can/dbgMask\n\
              switch debugging output on/off\n\
-b --baud baudrate (Standard wo option uses value of /proc/sys/Can/baud)\n\
-D dev        use /dev/dev/{can0,can1,can2,can3} (real nodes, std: can1)\n\
   --prio p   change scheduling priority of can_send\n\
   --canfd=   use CAN FD with given data lenth in frame\n\
\n\
standard data entry is in bytes. this can be changed to\n\
    --int16  two byte integer in little endian\n\
    --float  four byte float in little endian\n\
for all data following the CAN Id.\n\
\n\
-t type \n\
   1 stress test, transmit bursts of short RTR frames\n\
   2 (x) transmit bursts of 5 data frames, same ID\n\
   3 (x) transmit bursts of 5 data frames, different ID\n\
     frame 5 contains counter variable\n\
   4 (x) as without this option, but increments CAN-ID with each message\n\
   10 transmit bursts of 9 data frames, used for communication verification\n\
   11 transmit -T number of frames as fast as possible, write(fd, buf, 1)\n\
      if transmit buffer is full, sleep for -s ms time. \n\
      if == 0: don't sleep, poll\n\
      after every frame the message-id will be incremented\n\
   12 (x) same as 11\n\
      but the message-id is constant and the data bytes will be incremented\n\
   30 transmit consecutive messages with the same id\n\
      -T specifies the number of messages in this burst\n\
   31 same as 12 but 8 data bytes\n\
      -T specifies the number of messages in this burst\n\
   40 data for Java applet example (sinus data)\n\
   100 CANopen Tests\n\
   100 (x) fast NMT messages, consecutive OPERATIONAL/PREOPEARATIONAL\n\
       first parameter is CANopen Node-Id\n\
       can_send -Dcan1 -t100 -s 200\n\
   101 send simulated HB messages starting with id 2\n\
-R             reset CAN Controller only, exit() program after reset\n\
-T             number of bursts, time distance -s n (for -t<n>)\n\
-V --version   print program version\n\
\n\
Tests marked with (x) read back the receive queue\n\
";
    fprintf(stderr, "usage: %s [options] [id [ byte ..]]\n", s);
    fprintf(stderr, "%s", usage_text);
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

static int	set_bitrate(
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
    cfg.val1   = (unsigned int)baud;
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

#include "getstat.c"

static int can_reset(void) {
int ret;
volatile Command_par_t cmd;


    cmd.cmd = CMD_RESET;
    ret = ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);

    return ret;
}

static int can_start(void) {
int ret;
volatile Command_par_t cmd;


    cmd.cmd = CMD_CLEARBUFFERS;
    ret = ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);
    cmd.cmd = CMD_START;
    ret = ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);

    return ret;
}

/*
* Show CAN errors as text, 
* in case of Bus-Off, try to recover
*/
static void displayerror(canmsg_t *rx)
{
/* static int buffcnt = 0; */

	printf("Flags 0x%02x,", rx->flags);
	if( 0 == (rx->flags & MSG_ERR_MASK)) {
		printf(" CAN Error Free");
	}
	if( rx->flags & MSG_WARNING) {
		printf(" CAN Warning Level,");
	}
	if( rx->flags & MSG_PASSIVE) {
		printf(" CAN Error Passive,");
	}
	if( rx->flags & MSG_OVR) {
		printf(" CAN controller rx overflow,");
	}
	if( rx->flags & MSG_BOVR) {
		printf(" CAN buffer overflow,");
	}
	if( rx->flags & MSG_BUSOFF) {
		printf(" CAN Bus Off,");
		can_reset();
		/* sleep 100ms */
		usleep (100000);
		can_start();
	}
	printf("\n");
}

/*
 * Read the content of the RX queue
 * but only evaluate so called error-messages from the driver
 * which can id is coded as -1.
 * Such messages are than interpreted by displayerror()
 */
void read_can(int can_fd)
{
canmsg_t rx;
int i;
	    
    /* now read the rx queue, it may contain
     * frames with id -1, which signals an error */
    do {
	i = (int)read(can_fd, &rx, 1);
	if(-1 == i) {
	    perror("read error");
	} else if((i > 0) && (CANDRIVERERROR == rx.id)) {
	    displayerror(&rx);
	} else if (i > 0) {
	    /* valid CAN frame. do some tests */
	    /* check at least for rx buffer overflow */
	    if(rx.flags & MSG_BOVR) {
		printf(">> Driver buffer overflow detected <<\n");
	    }
	}
    }
    while(i > 0);  /* do {} while(); */
}

/*
 * Read the content of the RX queue
 * Evaluate frames of a can_send -t2 process of correct counter 
 * in the 4 data byte frame.
 * And evaluate so called error-messages from the driver
 * which can id is coded as -1.
 * Such messages are than interpreted by displayerror()
 */
void read_t2(int can_fd)
{
canmsg_t rx;
int i;
static unsigned long fcnt = 0;
static unsigned long ecnt = 0;	 /**< expected counter in received frame */
	    
    /* now read the rx queue, it may contain
     * frames with id -1, which signals an error */
    do {
	i = (int)read(can_fd, &rx, 1);
	if(-1 == i) {
	    perror("read error");
	} else if((i > 0) && (CANDRIVERERROR == rx.id)) {
	    displayerror(&rx);
	    errcnt++;
	} else if (i > 0) {
	    /* valid CAN frame. do some tests */
	    /* or simply count it */
	    fcnt += i;
	    if(rx.flags & MSG_BOVR) {
		printf(">> Driver buffer overflow detected <<\n");
	    }
	    if((fcnt % 1000) == 0) {
		printf("received %ld frames\n", fcnt);
		showCANStat(can_fd);
	    }
	     /* for test with can_send -t2 at the other node */
	    if((rx.length == 4) & (rx.id == 0x100)) {
		/* frame contains counter  */
		unsigned long ccnt;		/**< current counter */
#if 0
printf("That is waht I'm looking for: id 0x%lX, l %d\n",
	rx.id, rx.length);
	sbuf(&rx.data[0], rx.length);
#endif
		ccnt =	  rx.data[0]
			+ (rx.data[1] * (1L <<  8))
			+ (rx.data[2] * (1L << 16))
			+ (rx.data[3] * (1L << 24));

/* printf("	counter  %ld\n", ccnt); */
		if( ccnt != ecnt) {
		    /* mismatch */
		    printf("counter mismatch, expected %ld, got %ld\n",
		    	ecnt, ccnt);
		    ecnt = ccnt + 1;	
		    sbuf(&rx.data[0], rx.length);
		} else {
		    ecnt++;
		}
	    }	/* expected frame */
	} /* valid CAN frame, i > 1 */
    }
    while(i > 0);  /* do {} while(); */
}


/**
*
* The main program
*
*/
int main(int argc, char **argv)
{
int ret;
int cnt;
int c;					/* getopt() */
char *pname;
extern char *optarg;
extern int optind;
char device[MAXDEVNAME] = STDDEV;
int max_rr_priority, min_rr_priority;
int max_ff_priority, min_ff_priority;
int increment = 0;
unsigned long int test_count = 0;
struct timespec timereq;		/* used with nanosleep */

    pname = *argv;

#ifdef USE_RT_SCHEDULING
    max_rr_priority = sched_get_priority_max(SCHED_RR);
    min_rr_priority = sched_get_priority_min(SCHED_RR);
    max_ff_priority = sched_get_priority_max(SCHED_FIFO);
    min_ff_priority = sched_get_priority_min(SCHED_FIFO);
#else 
    max_priority = 1;
#endif

    atexit(printerrorcount);

    /* our default 8 byte message */
    /* it uses a pattern like 0x555555... at the beginning
       to simplyfy watching the frame on a scope an measure one
       bit time */
    message.id      = 0x555;
    message.cob     = 0;
    message.flags   = 0;
    message.length  = 8;
    message.data[0] = (unsigned char)0x55;
    message.data[1] = (unsigned char)0x55;
    message.data[2] = (unsigned char)0x55;
    message.data[3] = (unsigned char)4;
    message.data[4] = (unsigned char)5;
    message.data[5] = (unsigned char)6;
    message.data[6] = (unsigned char)7;
    message.data[7] = (unsigned char)0xaa;

    /* parse command line */
    /* new: -i arguments are 2 byte Integer 
            -I arguments are 4 byte Integer 
	    -f arguments are 4 byte float
	    all in little endian */
    while (1) {
	static const struct option long_options[] =
	{
	    /* These options set a flag. */
	    {"ext",     no_argument,       NULL,    'e'},
	    {"float",   no_argument,       NULL,    'f'},
	    {"int16",   no_argument,       NULL,    'i'},
	    {"rtr",     no_argument,       NULL,    'r'},
	    {"version", no_argument,       NULL,    'V'},
	    /* These options don't set a flag.
	      We distinguish them by their indices. */
	    {"baud",    required_argument, NULL,    'b'},
	    {"canfd",   optional_argument, NULL,   200 },
	    {"debug",   optional_argument, NULL,    'd'},
	    {"load",    required_argument, NULL,    'l'},
	    {"sleep",   required_argument, NULL,    's'},
	    {"prio",    required_argument, NULL,    'p'},
	    {"type",    required_argument, NULL,    't'},
	    {0, 0, 0, 0}
	};
	/* getopt_long stores the option index here. */
	int option_index = 0;

	c = getopt_long(argc, argv, "b:dehl:rs:n:D:t:T:VR",
		    long_options, &option_index);
	if (c == -1) {
             break;
	}
	switch (c) {
	    case 'r':
		rtr = TRUE;
		break;
	    case 'e':
		extd = TRUE;
		break;
	    case 'f':
		isfloat = TRUE;
		break;
	    case 'i':
		isshortinteger = TRUE; /* little Endian */
		break;
	    case 'b':
		baud = atoi(optarg);
		break;
	    case 'l':
		load = atoi(optarg);
		doload = TRUE;
		break;
	    case 'p':
	        {
#ifdef USE_RT_SCHEDULING
	        struct sched_param mysched;
	        /* use Realtime round-robin ( or Realtime first-in first-out */
		    priority = atoi(optarg);
		    if (priority < min_rr_priority ) {
		      fprintf(stderr, "Priority < %d not allowed\n",
		      					min_rr_priority);
		    }
		    if (priority > max_rr_priority) {
		      fprintf(stderr, "Priority > %d not allowed\n",
		      					max_rr_priority);
		    }
		    mysched.sched_priority = priority;	
		    		/* sched_get_priority_max(SCHED_RR) - 1; */

		    ret = sched_setscheduler(0, SCHED_RR, &mysched);
		    if ( debug == true ) {
			printf("sched_setscheduler() = %d\n", ret);
		    }
		    if(ret == -1) {
			printf("No permission to change process priorities\n");
		    }
		    /* lock all currently and in future
			allocated memory blocks in physical ram */
		    ret = mlockall(MCL_CURRENT | MCL_FUTURE);
		    if ( debug == true ) {
			printf("mlockall() = %d\n", ret);
		    }

#endif
		}
		break;
	    case 's':
		sleeptime = atoi(optarg);
		endless |= 0x01;
		break;
	    case 'n':
		node = (unsigned int)strtoul(optarg, NULL, 0);
		snprintf(device, MAXDEVNAME, "/dev/canp%d", (int)node);
		break;
	    case 'D':
	        if (0 == strcmp(optarg, "stdout")) {
	            cstdout = TRUE;
		} else if (
		    /* path ist starting with '.' or '/', use it as it is */
			optarg[0] == '.'
			|| 
			optarg[0] == '/'
			) {
		    snprintf(device, MAXDEVNAME, "%s", optarg);

	        } else {
		    snprintf(device, MAXDEVNAME, "/dev/%s", optarg);
		}
		break;
	    case 'd':
		debug = TRUE;
		if( 0 == strcmp("debug", long_options[option_index].name)) {
		    /* long option,
		       used to switch can4linux into debug as well */

		    char command[200];
		    if( optarg != NULL) {
			debug = atoi(optarg);
			sprintf(command,
			    "sudo sh -c '/bin/echo %d > /proc/sys/dev/Can/dbgMask'", debug);
			(void)system(command);
			printf("Do: %s\n", command);
		    } else {
			debug = TRUE;
		    }
		}
		break;
	    case 't':
		stresstest = TRUE;
		testtype   = atoi(optarg);
		break;
	    case 'T':
		test_count_soll = strtoul(optarg, NULL, 0);
		endless |= 0x02;
		break;
	    case 'V':
		printf("can_send V " VERSION ", " __DATE__ "\n");
		exit(0);
		break;
	    case 'R':
		canreset = TRUE;
		break;
	    case 200:		/* --canfd= */
		printf(" used CAN-FD option, index %d, arg: %s\n", option_index, optarg);
		/* check for allowed values
		    dlc	    bytes
		    9	    12
		    10	    16
		    11	    20
		    12	    24
		    13	    32
		    14	    48
		    15	    64
		 */
		if(optarg != NULL) {
		    canfd = atoi(optarg);
		    switch(canfd) {
			case  8:
			case 12:
			case 16:
			case 20:
			case 24:
			case 32:
			case 48:
			case 64:
			    break;
			default:
			    printf(
			    "CAN FD data length %d not allowed. Only: 8,12,16,20,24,32,48,64\n",
			    canfd);
			    exit(1);
		    }
		} else {
		    canfd = DEFAULT_CANFD_LEN;
		}

		printf("Using default CAN FD len of %d\n", canfd);
		printf(" I'm just beginning to implement it. If you like, please help.\n");
		break;
	    case 'h':
	    default: usage(pname);
#ifdef USE_RT_SCHEDULING
		printf("\nRealtime   RR prio min max %3d/%d\n", 
			    min_rr_priority, max_rr_priority);
		printf("Realtime FIFO prio min max %3d/%d\n",
			    min_ff_priority, max_ff_priority);
#endif
		exit(0);
	}   /* end switch() */
    }	    /* end while() */

    timereq.tv_sec  = sleeptime / 1000;
    timereq.tv_nsec = (sleeptime % 1000) * 1000 * 1000;

    /* look for additional arguments given on the command line */
    if ( argc - optind > 0 ) {
        /* at least one additional argument, the message id is given */
	message.id =  strtoul(argv[optind++], NULL, 0);
    	memset(message.data, 0, 8);
	message.length = 0;
    }
    if ( argc - optind > 0 ) {
    	/* also data are given with the command */
        int index = 0;	    /* index into the message.data[] array */
	cnt = 0;
	if(isfloat == TRUE) {
	    /* programm arguments are float values */
	    float *fp = (float *)&message.data[0];
	    while(optind != argc) {
		if(debug)printf(" float: %s\n", argv[optind]);
		*fp = (float)atof(argv[optind++]);
		fp++;
		cnt += 4;
	    }

	} else if (isshortinteger == TRUE) {
	    /* programm arguments are float values */
	    short int *sip = (short int *)&message.data[0];
            while(optind != argc) {
                *sip = (short int)strtoul(argv[optind++], NULL, 0);
		sip++;
		cnt += 2;
	    }

	} else {
	    /* programm arguments are byte values */
            while(optind != argc) {
		message.data[index] =
		    (unsigned char)strtoul(argv[optind++], NULL, 0);
		if(canfd > 0) {
		    if(cnt++ < canfd) index++; 
		} else {
		    if(cnt++ < 8) index++; 
		}
	    } /* while() */
	}
        if(canfd) {
            /* depending on count, find the correct coding for .length */
            message.length = len2dlc(cnt); 
        } else {
            if(cnt < 16 ) {
                message.length = cnt;
            } else {
                message.length = 15;
            }
        }
    }

    /* ----------------------------------------------- */
    if (rtr) {
	message.flags |= MSG_RTR;
    }
    if (extd) {
	message.flags |= MSG_EXT;
    }
    if(canfd > 0) {
	message.flags |= MSG_CANFD;
    }

    if (endless != 0x01) {
        /* only the value 1 means endless, all other values means,
         * additional options used -> no endless
         */
        endless = 0;
    }

    if ( debug == true ) {

	printf("can_send V " VERSION ", " __DATE__ "\n");
	printf("(c) 1996-2009 port GmbH\n");
	printf(" using canmsg_t with %d bytes\n", (int)sizeof(canmsg_t));
	printf("  data at offset %d\n", (int)offsetof(canmsg_t, data));
#ifdef USE_RT_SCHEDULING
	printf(" max process RR priority is \"-p %d\"\n", max_rr_priority);
	/* display scheduler parameters */
#endif

#ifdef USE_RT_SCHEDULING
	{
	int pol;
	struct sched_param sp;

	    pol = sched_getscheduler(0);
	    sched_getparam(0, &sp);
	    printf("using realtime %s policy and priority %d\n",
	    	(pol == SCHED_OTHER) ? "OTHER" :
	    	(pol == SCHED_RR)    ? "RR" :
	    	(pol == SCHED_FIFO)  ? "FIFO" :
#ifdef SCHED_BATCH
	    	(pol == SCHED_BATCH) ? "BATCH" :
#endif
#ifdef SCHED_IDLE
	    	(pol == SCHED_IDLE)  ? "IDLE" :
#endif
		"???", sp.sched_priority);
	}
#endif /* USE_RT_SCHEDULING */
	if (stresstest) {
	    printf("should send one of the test sequences\n");
	} else {
	    printf("should send mess %ld with: %s", (long int)message.id,
		message.length > 0 ? ": " : "out data");
	    /* FIXME: bytes, not dlc */
	    sbuf(message.data, message.length);
	}
    }

    sleeptime *= 1000;
    if ( debug == true ) {
	printf("Sleeptime between transmit messages= %d us\n", sleeptime);

#if _POSIX_C_SOURCE >= 199309L
	printf("POSIX nanosleep()\n");
#endif
	printf(" Sec %d; ns  %ld\n", (int)timereq.tv_sec, timereq.tv_nsec);
    }

    srand(node * 100);

    if(sleeptime > 0) {
    	errwaittime = sleeptime;
    } else {
    	errwaittime = 10000;
    }

    if (cstdout == FALSE) {
        /* really use CAN, open the device driver */
	if ( debug == true ) {
	    printf("Open device %s\n", device);
	}
	/* can_fd = open(device, O_WRONLY | O_NONBLOCK); */
	can_fd = open(device, O_RDWR | O_NONBLOCK);
	if (can_fd == -1) {
	    fprintf(stderr, "open error %d;", errno);
	    perror(device);
	    exit(1);
	}

	if ( canreset == TRUE ) {
	    ret = can_reset();
	    if ( ret == -1 ) {
		perror("CAN Reset");
		exit(EXIT_FAILURE);
	    }
	    if ( debug == true) {
		printf("Reset successfull\n");
	    }
	    exit(EXIT_SUCCESS);
	}
	
	if (baud > 0) {
	    if ( debug == true ) {
		printf("change Bit-Rate to %d Kbit/s\n", baud);
	    }
	    set_bitrate(can_fd, baud);
	}
    } else {
	can_fd = 1;		/* use stdout */
    }
    if ( debug == true ) {
	printf("opened %s successful, got can_fd = %d\n", device, can_fd);
    }

    if (doload == TRUE) {
	test20();
	exit(0);
    }
    if (stresstest) {
	switch(testtype) {
	    case 1:  test1(); exit(0); break;
	    case 2:  test2(); exit(0); break;
	    case 3:  test3(); exit(0); break;
	    case 4:  increment = 1; break;
	    case 10: test10(); exit(0); break;
	    case 11: test11();
	    	sleep(2); printf("finito\n");
	    		exit(0); break;
	    case 12: test12(); exit(0); break;
	    case 30: test30(); exit(0); break;
	    case 31: test31(); exit(0); break;
	    case 40: test40(); exit(0); break;
	/* specific CANopen related tests */
	    case 100: test100(); exit(0); break;
	    case 101: test101(); exit(0); break;

	    default:
	    fprintf(stderr, "test type %d is not defined\n", testtype);
	    exit(0); break;
	}
    }

    /* else */
    /* the default can_send, simply send a message */
    /* no special test, send the normal message, (old behaviour) */
    do {

        if(debug == true) {
	     printf(" transmit message id: %ld, \n", (long int)message.id ); 
	}
	/* printf("dlc %d\n", message.length); */
	ret = (int)write(can_fd, &message, 1);

	if(ret == -1) {
	    /* int e = errno; */
	    perror("write error");
	    /* if ( e == ENOSPC) { */
		usleep((useconds_t)errwaittime); 
		continue;
	    /* } */
	} else if(ret == 0) {
	    printf("transmit timed out\n");
		usleep((useconds_t)errwaittime); 
		continue;
	} else {
	    ;
	}
	/* now read the rx queue, it may contain
	 * frames with id -1, which signals an error */
	 read_can(can_fd);

	if( debug == true ) {
	     showCANStat(can_fd);
 	}
	/* if(sleeptime > 0) usleep(sleeptime); */
	if(sleeptime > 0) nanosleep(&timereq, NULL);
	message.id += increment;
	
	test_count++;
	
        if(endless != 1) {
	    if(test_count == test_count_soll) {
		break;
	    }
	}
    }
    while(true);    /* do {} while(); */


    if (sleeptime == 0) {
        /* do not close while the controller is sending a message
         * sleep() then close()
         */
        (void)usleep((useconds_t)100000);
        /* sleep(1); */
    }
    ret = close(can_fd);
    if (ret == -1) {
	fprintf(stderr, "close error %d;", errno);
	perror("");
	exit(1);
    }
    if ( debug == true ) {
	printf("closed fd = %d successful\n", can_fd);
    }
    return 0;
}


/* test1:
   - Data message 8 byte
   - RTR  Message 0 byte
   - RTR  Message 1 byte
   - RTR  Message 2 byte
*/
void test1(void)
{
unsigned long int test_count = 0;
canmsg_t tm[4];
int ret;

    tm[0].id = 100;
    tm[0].cob = 0;
    tm[0].length = 8;
    tm[0].flags = 0;
    if (extd) {
	tm[0].flags |= MSG_EXT;
    }
    tm[0].data[0] = 0x55;
    tm[0].data[1] = 2;
    tm[0].data[2] = 3;
    tm[0].data[3] = 4;
    tm[0].data[4] = 5;
    tm[0].data[5] = 6;
    tm[0].data[6] = 7;
    tm[0].data[7] = 0xaa;

    tm[1].id = message.id;
    tm[1].cob = 0;
    tm[1].length = 0;
    tm[1].flags = MSG_RTR;
    if (extd) {
	tm[1].flags |= MSG_EXT;
    }

    tm[2].id = message.id;
    tm[2].cob = 0;
    tm[2].length = 1;
    tm[2].flags = MSG_RTR;
    if (extd) {
	tm[2].flags |= MSG_EXT;
    }

    tm[3].id = message.id;
    tm[3].cob = 0;
    tm[3].length = 2;
    tm[3].flags = MSG_RTR;
    if (extd) {
	tm[3].flags |= MSG_EXT;
    }


    do {
	ret = (int)write(can_fd, &tm[0], 4);
	if (ret == -1) {
	    perror("write error");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else {
	    if ( debug == true ) {
		printf("transmitted %d\n", ret);
	    }
	}
	
	test_count++;
	
	if(endless != 1) {
	    if (test_count == test_count_soll) {
		break;
	    }
	}

	if (sleeptime > 0 )  {
	    usleep((useconds_t)sleeptime);
	}
    }
    while (true);
    sleep(1);
}

/* test2:
   - Data message 8 byte id= default or command line
   - Data message 8 byte  "
   - Data message 0 byte  "
   - Data message 8 byte  "
   - Data message 4 byte  "
*/
void test2(void)
{
unsigned long int test_count = 0;
unsigned long int err_count = 0;
canmsg_t tm[5];
int ret;
unsigned int cnt = 0;

    tm[0].id = message.id;
    tm[0].cob = 0;
    tm[0].length = 8;
    tm[0].flags = 0;
    if (extd) {
	tm[0].flags |= MSG_EXT;
    }
    tm[0].data[0] = 0x55;
    tm[0].data[1] = 2;
    tm[0].data[2] = 3;
    tm[0].data[3] = 4;
    tm[0].data[4] = 5;
    tm[0].data[5] = 6;
    tm[0].data[6] = 7;
    tm[0].data[7] = 0xaa;

    tm[1].id = message.id;
    tm[1].cob = 0;
    tm[1].length = 8;
    tm[1].flags = 0;
    if (extd) {
	tm[1].flags |= MSG_EXT;
    }
    tm[1].data[0] = 0xaa;
    tm[1].data[1] = 7;
    tm[1].data[2] = 6;
    tm[1].data[3] = 5;
    tm[1].data[4] = 4;
    tm[1].data[5] = 3;
    tm[1].data[6] = 2;
    tm[1].data[7] = 0x55;

    tm[2].id = message.id;
    tm[2].cob = 0;
    tm[2].length = 0;
    tm[2].flags = 0;
    if (extd) {
	tm[2].flags |= MSG_EXT;
    }

    tm[3].id = message.id;
    tm[3].cob = 0;
    tm[3].length = 8;
    tm[3].flags = 0;
    if (extd) {
	tm[3].flags |= MSG_EXT;
    }
    tm[3].data[0] = 0x55;
    tm[3].data[1] = 2;
    tm[3].data[2] = 3;
    tm[3].data[3] = 4;
    tm[3].data[4] = 5;
    tm[3].data[5] = 6;
    tm[3].data[6] = 7;
    tm[3].data[7] = 0xaa;

    tm[4].id = message.id;
    tm[4].cob = 0;
    tm[4].length = 4;
    tm[4].flags = 0;

    /* currently data[] starts at offset 22 within canmsg_t
     * Therfore a misalignment bus error is generated on some
     * targets where a 4 byte value like cnt must be 4 byte aligned.
     * The following code does not work in this machines
     *
     * *(unsigned int *)&tm[4].data[0] = cnt++
     */
     
    *(unsigned short *)&tm[4].data[0] = (unsigned short)(cnt & 0xffff);
    *(unsigned short *)&tm[4].data[2] = (unsigned short)((cnt & 0xfffff) >> 16);
    cnt++;

    if (extd) {
	tm[4].flags |= MSG_EXT;
    }

    do {
	ret = (int)write(can_fd, &tm[0], 5);
	if (ret == -1) {
	    perror("write error");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    usleep((useconds_t)errwaittime + (1000 * err_count)); 
	    err_count++;
	    showCANStat(can_fd);
	    /* Try something here to solve the problem */
	    if(err_count % 10)  {
		can_reset();
		/* sleep 100ms */
		usleep (100000);
		can_start();

	    }
	    continue;
	} else {
	    if ( debug == true ) {
		printf("transmitted %d\n", ret);
	    }
	}
	/* now read the rx queue, it may contain
	 * frames with id -1, which signals an error */
	read_can(can_fd);

	/* *(unsigned int *)&tm[4].data[0] = cnt++; */
	*(unsigned short *)&tm[4].data[0] = (unsigned short)(cnt & 0xffff);
	*(unsigned short *)&tm[4].data[2] = (unsigned short)((cnt & 0xfffff) >> 16);
	cnt++;
	if((cnt % 10000) == 0) {
	    showCANStat(can_fd);
	}
	
	test_count++;

	if(endless != 1) {
	    if (test_count == test_count_soll) {
		break;
	    }
	}

	if ( sleeptime > 0 ) {
	    usleep((useconds_t)sleeptime);
	}
    } while(true);
    /* give the driver ISR a chance to empty the transmit queue */
    sleep(1);
}

/* test3:
   - Data message 8 byte id= default or command line
   - Data message 8 byte  "
   - Data message 0 byte  "
   - Data message 8 byte  "
   - Data message 4 byte  "   - data with 4 byte counter
*/
void test3(void)
{
unsigned long int test_count = 0;
canmsg_t tm[5];
int ret;
unsigned int cnt = 0;

struct timespec req;
/* struct timespec rem; */

#define ID_OFFSET 0x10 		/* used for the following messages */

    tm[0].id = message.id;
    tm[0].cob = 0;
    tm[0].length = 8;
    tm[0].flags = 0;
    if (extd) {
	tm[0].flags |= MSG_EXT;
    }
    tm[0].data[0] = 0x55;
    tm[0].data[1] = 2;
    tm[0].data[2] = 3;
    tm[0].data[3] = 4;
    tm[0].data[4] = 5;
    tm[0].data[5] = 6;
    tm[0].data[6] = 7;
    tm[0].data[7] = 0xaa;

    tm[1].id = tm[0].id + ID_OFFSET;
    tm[1].cob = 0;
    tm[1].length = 8;
    tm[1].flags = 0;
    if (extd) {
	tm[1].flags |= MSG_EXT;
    }
    tm[1].data[0] = 0xaa;
    tm[1].data[1] = 7;
    tm[1].data[2] = 6;
    tm[1].data[3] = 5;
    tm[1].data[4] = 4;
    tm[1].data[5] = 3;
    tm[1].data[6] = 2;
    tm[1].data[7] = 0x55;

    tm[2].id = tm[1].id + ID_OFFSET;
    tm[2].cob = 0;
    tm[2].length = 0;
    tm[2].flags = 0;
    if (extd) {
	tm[2].flags |= MSG_EXT;
    }

    tm[3].id = tm[2].id + ID_OFFSET;
    tm[3].cob = 0;
    tm[3].length = 8;
    tm[3].flags = 0;
    if (extd) {
	tm[3].flags |= MSG_EXT;
    }
    tm[3].data[0] = 0x55;
    tm[3].data[1] = 2;
    tm[3].data[2] = 3;
    tm[3].data[3] = 4;
    tm[3].data[4] = 5;
    tm[3].data[5] = 6;
    tm[3].data[6] = 7;
    tm[3].data[7] = 0xaa;

    tm[4].id = tm[3].id + ID_OFFSET;
    tm[4].cob = 0;
    tm[4].length = 4;
    tm[4].flags = 0;
    /* *(unsigned int *)&tm[4].data[0] = cnt++; */
    *(unsigned short *)&tm[4].data[0] = (unsigned short)(cnt & 0xffff);
    *(unsigned short *)&tm[4].data[2] = (unsigned short)((cnt & 0xfffff) >> 16);
    cnt++;

    if (extd) {
	tm[4].flags |= MSG_EXT;
    }

#if 1
	/*
	If  the process is scheduled under a real-time policy like
	SCHED_FIFO or SCHED_RR, then pauses of up to 2 ms will  be
	performed as busy waits with microsecond precision.
	*/
	req.tv_sec = sleeptime / (1000*1000);
	req.tv_nsec = (sleeptime % (1000*1000)) * 1000;
	if ( debug ) {
	    printf("Sleep %ld.%09ld\n", req.tv_sec, req.tv_nsec);
	}
#endif


    do {
	ret = (int)write(can_fd, &tm[0], 5);
	if (ret == -1) {
	    perror("write error");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else {
	    if ( debug ) {
		printf("transmitted %d\n", ret);
	    }
	}

	*(unsigned short *)&tm[4].data[0] = (unsigned short)(cnt & 0xffff);
	*(unsigned short *)&tm[4].data[2] = (unsigned short)((cnt & 0xfffff) >> 16);
	cnt++;
	
	test_count++;
	
	if(endless != 1) {
	    if (test_count == test_count_soll) {
		break;
	    }
	}

	if ( debug ) {
	    showCANStat(can_fd);
	}
	/* now read the rx queue, it may contain
	 * frames with id -1, which signals an error */
	read_can(can_fd);

	if (sleeptime > 0) {

	    (void)usleep((useconds_t)sleeptime);
    /* Für die Verwendung von nanosleep muss noch etwas getan werden.
       Nanosleep benutzt bei Zeiten < 2 ms eine echte busy loop im kernel,
       d.h. mit "-s 1" geht bei Endlosschleifen dann gar nichts mehr
       außer dem großen Roten Knopf am Rechner.
       Falls also implementieren, dann auf kurze Schleifen, max 10 oder
       so begrenzen, damit kann es aber möglich sein mal schnell 
       meherer telegramme hintereinender zu senden, dann wieder usleep()
       nehmen ...
     */
	    /* nanosleep(&req, &rem); */
	}
    }
    while(true);
    sleep(1);
}


/* Test 10


You can see the output in hexformat by calling
             +- select test type
             |      +-- send messages to
             |      |        +- number of sequences a 9 messages ( 2 * 9)
             |      |        |     +- time in ms between sequences
             |      |        |     |
$ can_send -t10 -D stdout -T 2 -s 100 | od -t x1 -w32
                                         |   |     |
                                         |   |     +-- 32 bytes per line == 
                                         |   |         one message
                                         |   +-- type hex, one byte
                                         +- use "object dump" for display


*/
#define SEQN	9 /* number of messages for one sequence, one write call */
static void update_seq(canmsg_t *m)
{
unsigned long int i;

    /* calculate next sequence */
    /* first: new message id */
				/* ((2^11)/9)*9 - 1   
				 * ((2*29)/9)*9 - 1
				 */
    if (    ( extd && (m->id > (536870906 - SEQN))) 
	 || (!extd && (m->id > (     2042 - SEQN)))  ) {
	/* reset message id to 0 */
	for (i = 0; i < SEQN; i++) {
	    (m + i)->id = i;
	}
    } else {
	if ( debug ) {
	    printf(" new id %lu\n", (m->id + SEQN));
	}
	/* not wrapped, increment message id */
	for (i = 0; i < SEQN; i++) {
		(m + i)->id += SEQN;
	}
    }

    /* now update data bytes with counter value */
    for (i = 0; i < SEQN; i++) {
        unsigned char *p;
        p = &((m + i)->data[0]);
#if defined(EMBED)
	*(unsigned short *)p += 1;
#else
	*(unsigned long long *)p += 1;
#endif
    }
}

void test10(void)
{
unsigned long int test_count = 0;
int ret, i;
/* unsigned int cnt = 0; */
int fac = 1;

/* why not
    {  0 ,   0,   0, {0, 0}, 0, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
  in the following initailization table ?
  time is a struct with two entries ? */

canmsg_t tm[SEQN] =  {
    /*  f, cob,  id, time,   l,   data[8]                     */
    {  0 ,   0,   0, {0, 0},  0, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   1, {0, 0},  1, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   2, {0, 0},  2, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   3, {0, 0},  3, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   4, {0, 0},  4, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   5, {0, 0},  5, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   6, {0, 0},  6, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   7, {0, 0},  7, { 0, 0, 0, 0, 0, 0, 0, 0} }, 
    {  0 ,   0,   8, {0, 0},  8, { 0, 0, 0, 0, 0, 0, 0, 0} }};

    if (cstdout == TRUE) {
	/* use stdout */
	fac = (int)sizeof(canmsg_t);
    }
    if (extd) {
	/* set the extd flag in all messages */
	for (i = 0; i < SEQN; i++) {
		tm[i].flags |= MSG_EXT;
	}
    }
    if ( debug ) {
	printf("using test10 with extd = %s\n", extd ? "TRUE" : "FALSE");
    }

    /* loop forever if sleeptime > 0 */
    do {
        int seq   = SEQN * fac;
        int start = 0;

	while(seq) {
	    if ( debug ) {
		 printf("send %d/%d, ", start, seq);
	    }
	    ret = (int)write(can_fd, &tm[start], (size_t)seq);
	    if (ret == -1) {
		perror("write error");
		exit(2);
	    } else if (ret == 0) {
		printf("transmit timed out\n");
		/* in case of polling (sleeptime==0), wait 0 or 1000us */
		/* usleep(sleeptime);  */
		usleep((useconds_t)errwaittime); 
		continue;
	    } else {
		if ( debug ) {
		    printf("transmitted %d\n", ret);
		}
	    }
	    if ( debug ) {
		showCANStat(can_fd);
	    }
	    seq -= ret;
	    start += ret;
	    if ( seq >= 0 && sleeptime) {
	    	(void)usleep((useconds_t)10000); 
	    }
	}
	
	test_count++;

	if (endless != 1) {
	    /* is it enough ? than leave loop */
	    if (test_count == test_count_soll) {
		break;
	    }
	}

        update_seq(&tm[0]);

	/* if neccessary sleep */
	if (sleeptime > 0) {
	    usleep((useconds_t)sleeptime);
	}
    }
    while(true);
    /* before closing the driver, give it a chance to transmit
    messages in tx buffer */
    usleep((useconds_t)1000000);
}

/* using the global defined message, that can be changed via command line,
the function tries to reach a specified bus-load
- use 20 ms sleep time or larger for low bus loads
*/
void test20(void)
{
int n, i;
unsigned long int test_count;
int run;
int ret;

    /* first assume we have only 11 bit and 8 data bytes */
    /* 1 Message = 120 bits */
    /* number of messages for 100 % */
    /* time   / (bits/m   * time_one_bit) */
    n = 20000 / (120 * (1000 / baud));  
    n = n * load / 100;

    test_count = 0;
    printf("send %lu messages, %d messages  every 20 ms cycle \n",
		    test_count_soll, n);

    /* printf("soll %ld \n", test_count_soll); */
    sleeptime = 19000;
    run = 0;
    run = TRUE;

    while(run) {
	for(i = 0; i < n; i++) {
	    ret = (int)write(can_fd, &message, 1);
	    if (ret == -1) {
		perror("write error; ");
	    } else if (ret == 0) {
		printf("Node %u: transmit time out\n", node);
	    } else {
		if ( debug ) {
		     printf("Node %u: transmit message %lu\n",
		     		node, message.id );
		 }
	    }

	    /* endless??? 
	     *  2007_09_28 - ro
	     */
	    
	    if (test_count_soll && (++test_count == test_count_soll)) {
		run = FALSE;
		break;
	    }
	    /* printf("  count %d \n", test_count); */
	}
	usleep((useconds_t)sleeptime);
    }

}

void test11(void)
{
canmsg_t message;
unsigned long int test_count = 0;
int ret;

    /* our default 8 byte message */
    message.id      = 0;
    message.cob     = 0;
    message.flags   = 0;
    message.length  = 8;
    message.data[0] = 0x55;
    message.data[1] = 2;
    message.data[2] = 3;
    message.data[3] = 4;
    message.data[4] = 5;
    message.data[5] = 6;
    message.data[6] = 7;
    message.data[7] = 0xaa;
    if (extd) {
	message.flags |= MSG_EXT;
    }
    /* else */
    /* the default can_send, simply send a message */
    /* no special test, send the normal message, (old behaviouur) */
    do {
    again:
        if ( debug ) {
	     printf(" transmit message %lu\n", message.id );
	 }
	ret = (int)write(can_fd, &message, 1);
	if (ret == -1) {
	    /* int e = errno; */
	    perror("write error");
	    /* if ( e == ENOSPC) { */
		if (sleeptime != 0) { usleep((useconds_t)sleeptime);  }
		/* continue; */
		goto again;
	    /* } */
	} else if (ret != 1) {
	    /* write buffer full? try it again */
	    /* fprintf(stderr, "transmitted %d from 1\n", ret); */
	    /* Only if buffer full, sleep a while if specified */
		if (sleeptime != 0) { usleep((useconds_t)sleeptime);  }
		goto again;
	} else {
	}
	if ( debug ) {
	     showCANStat(can_fd);
	}
	message.id++ ;
	
	test_count++;

	if (endless != 1) {
	    /* is it enough ? than leave loop */
	    if (test_count == test_count_soll) {
		break;
	    }
	}
    }
    while(true);
    usleep(1000000);
}

/* used for some tests
because it sends on a fixed id,
it can cooperate with other CAN transmitters on the bus,
sending frames with higher or  lower priority, e.g. -t2.
a tcl script check_idata exists which than can be used
to check a logfile recorded with horch
*/
void test12(void)
{
unsigned long int test_count = 0;
int ret;

    /* message.id      = 0; */ /* default message id */
    message.cob     = 0;
    message.flags   = 0;
    if (extd != 0) {
	message.flags |= MSG_EXT;
    }
    message.length  = 4;
    message.data[0] = (unsigned char)0;
    message.data[1] = (unsigned char)0;
    message.data[2] = (unsigned char)0;
    message.data[3] = (unsigned char)0;
    message.data[4] = (unsigned char)0;
    message.data[5] = (unsigned char)0;
    message.data[6] = (unsigned char)0;
    message.data[7] = (unsigned char)0;
    /* else */
    /* the default can_send, simply send a message */
    /* no special test, send the normal message, (old behaviouur) */
    do {
    	message.data[0] = (unsigned char)test_count % 0x100;
    	message.data[1] = (unsigned char)(test_count >> 8) % 0x100;
    	message.data[2] = (unsigned char)(test_count >> 16) % 0x100;
    	message.data[3] = (unsigned char)(test_count >> 24) % 0x100;
    again:
        if ( debug ) {
	     printf(" %6lu: transmit message %lu\n",
	     	test_count, message.id );
	 }
#if 0
	gettimeofday(&tstamp, NULL);
	printf("%12lu.%06lu >", tstamp.tv_sec, tstamp.tv_usec);
	showCANStat(can_fd);
#endif
	/* only one message a time */
	ret = (int)write(can_fd, &message, 1);
#if 0
	gettimeofday(&tstamp, NULL);
	printf("%12lu.%06lu <", tstamp.tv_sec, tstamp.tv_usec);
	showCANStat(can_fd);
	fputs("\n", stdout);
#endif

	if (ret == -1) {
	    /* int e = errno; */
	    perror("write error");
	    /* if ( e == ENOSPC) { */
		if (sleeptime != 0) { usleep((useconds_t)sleeptime);  }
		/* continue; */
		goto again;
	    /* } */
	} else if (ret != 1) {
	    /* fprintf(stdout, "transmitted %d from 1\n", ret); */
	    /* sleep only if the transmit buffer is full ! */
		if (sleeptime != 0) { usleep((useconds_t)sleeptime);  }
		goto again;
	} else {
	}

	if ( debug )
	     showCANStat(can_fd);

        read_t2(can_fd);

	/* and DON't sleep here after each frame, like it is in test11 */
	/* if (sleeptime != 0) { usleep((useconds_t)sleeptime);  } */
	
	test_count++;

	if (endless != 1) {
	    /* is it enough ? than leave loop */
	    if (test_count == test_count_soll) {
		break;
	    }
	}
    }
    while(true);
    usleep(1000000);
}

void test30(void)
{
/* long int test_count = 0; */
/* unsigned int cnt = 0; */

#define MAXTESTMSG 5
canmsg_t tm[MAXTESTMSG];
int ret;

 /* should be malloced  here according to the variable 
	    test_count_soll
    and all messages initialized in a loop
    */

    tm[0].id = message.id;
    tm[0].cob = 0;
    tm[0].length = 8;
    tm[0].flags = 0;
    if (extd != 0) {
	tm[0].flags |= MSG_EXT;
    }
    tm[0].data[0] = (unsigned char)0x55;
    tm[0].data[1] = (unsigned char)2;
    tm[0].data[2] = (unsigned char)3;
    tm[0].data[3] = (unsigned char)4;
    tm[0].data[4] = (unsigned char)5;
    tm[0].data[5] = (unsigned char)6;
    tm[0].data[6] = (unsigned char)7;
    tm[0].data[7] = (unsigned char)0xaa;

    tm[1].id = message.id;
    tm[1].cob = 0;
    tm[1].length = 8;
    tm[1].flags = 0;
    if (extd != 0) {
	tm[1].flags |= MSG_EXT;
    }
    tm[1].data[0] = (unsigned char)0xaa;
    tm[1].data[1] = (unsigned char)7;
    tm[1].data[2] = (unsigned char)6;
    tm[1].data[3] = (unsigned char)5;
    tm[1].data[4] = (unsigned char)4;
    tm[1].data[5] = (unsigned char)3;
    tm[1].data[6] = (unsigned char)2;
    tm[1].data[7] = (unsigned char)0x55;


    tm[2].id = message.id;
    tm[2].cob = 0;
    tm[2].length = 8;
    tm[2].flags = 0;
    if (extd != 0) {
	tm[2].flags |= MSG_EXT;
    }
    tm[2].data[0] = (unsigned char)0xaa;
    tm[2].data[1] = (unsigned char)7;
    tm[2].data[2] = (unsigned char)6;
    tm[2].data[3] = (unsigned char)5;
    tm[2].data[4] = (unsigned char)4;
    tm[2].data[5] = (unsigned char)3;
    tm[2].data[6] = (unsigned char)2;
    tm[2].data[7] = (unsigned char)0x55;


    tm[3].id = message.id;
    tm[3].cob = 0;
    tm[3].length = 8;
    tm[3].flags = 0;
    if (extd != 0) {
	tm[3].flags |= MSG_EXT;
    }
    tm[3].data[0] = (unsigned char)0xaa;
    tm[3].data[1] = (unsigned char)7;
    tm[3].data[2] = (unsigned char)6;
    tm[3].data[3] = (unsigned char)5;
    tm[3].data[4] = (unsigned char)4;
    tm[3].data[5] = (unsigned char)3;
    tm[3].data[6] = (unsigned char)2;
    tm[3].data[7] = (unsigned char)0x55;

    tm[4].id = message.id;
    tm[4].cob = 0;
    tm[4].length = 8;
    tm[4].flags = 0;
    if (extd != 0) {
	tm[4].flags |= MSG_EXT;
    }
    tm[4].data[0] = (unsigned char)0xaa;
    tm[4].data[1] = (unsigned char)7;
    tm[4].data[2] = (unsigned char)6;
    tm[4].data[3] = (unsigned char)5;
    tm[4].data[4] = (unsigned char)4;
    tm[4].data[5] = (unsigned char)3;
    tm[4].data[6] = (unsigned char)2;
    tm[4].data[7] = (unsigned char)0x55;

    if(test_count_soll > MAXTESTMSG)
	    test_count_soll = MAXTESTMSG;

    do {
	ret = (int)write(can_fd, &tm[0], (size_t)test_count_soll);
	if (ret == -1) {
	    perror("write error");
	    (void)usleep((useconds_t)errwaittime); 
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    (void)usleep((useconds_t)errwaittime); 
	    continue;
	} else {
	    if ( debug ) {
		printf("transmitted %d\n", ret);
	    }
	}

	if ( sleeptime > 0 ) {
	    (void)usleep((useconds_t)sleeptime);
	}
    }
    while(true);
    sleep(1);
}

static void test31(void)
{
unsigned long int test_count = 0;
int ret;

    /* message.id      = 0; */ /* default message id */
    message.cob     = 0;
    message.flags   = 0;
    if (extd != 0) {
	message.flags |= MSG_EXT;
    }
    message.length  = 8;
    message.data[0] = (unsigned char)1;
    message.data[1] = (unsigned char)2;
    message.data[2] = (unsigned char)3;
    message.data[3] = (unsigned char)4;
    message.data[4] = (unsigned char)5;
    message.data[5] = (unsigned char)6;
    message.data[6] = (unsigned char)7;
    message.data[7] = (unsigned char)8;
    /* else */
    /* the default can_send, simply send a message */
    /* no special test, send the normal message, (old behaviouur) */
    do {
    	message.data[0] = (unsigned char)test_count % 0x100;
    	message.data[1] = (unsigned char)(test_count >> 8) % 0x100;
    	message.data[2] = (unsigned char)(test_count >> 16) % 0x100;
    	message.data[3] = (unsigned char)(test_count >> 24) % 0x100;
    again:
        if ( debug ) {
	     printf(" %6lu: transmit message %lu\n",
	     	test_count, message.id );
	 }
#if 0
	gettimeofday(&tstamp, NULL);
	printf("%12lu.%06lu >", tstamp.tv_sec, tstamp.tv_usec);
	showCANStat(can_fd);
#endif
	ret = (int)write(can_fd, &message, (size_t)1);
#if 0
	gettimeofday(&tstamp, NULL);
	printf("%12lu.%06lu <", tstamp.tv_sec, tstamp.tv_usec);
	showCANStat(can_fd);
	fputs("\n", stdout);
#endif

	if (ret == -1) {
	    /* int e = errno; */
	    perror("write error");
	    /* if ( e == ENOSPC) { */
		if (sleeptime != 0) {
		    (void)usleep((useconds_t)sleeptime);
		}
		/* continue; */
		goto again;
	    /* } */
	} else if (ret != 1) {
	    /* fprintf(stdout, "transmitted %d from 1\n", ret); */
		if (sleeptime != 0) {
		    (void)usleep((useconds_t)sleeptime);
		}
		goto again;
	} else {
	}

	if ( debug )
	     showCANStat(can_fd);

	if (sleeptime != 0) {
	    (void)usleep((useconds_t)sleeptime);
	}
	
	test_count++;

	if (endless != 1) {
	    /* is it enough ? than leave loop */
	    if (test_count == test_count_soll) {
		break;
	    }
	}
    }
    while(true);
    (void)usleep((useconds_t)1000000);
}

void test40(void)
{
/* long int test_count = 0; */
/* unsigned int cnt = 0; */

#if defined(X)
canmsg_t tm;
int ret;
float  inc = 0;


    tm.id = message.id;
    tm.cob = 0;
    tm.length = 8;
    tm.flags = 0;
    if (extd) {
	tm.flags |= MSG_EXT;
    }
    tm.data[0] = 0;
    tm.data[1] = 0;
    tm.data[2] = 0;
    tm.data[3] = 0;
    tm.data[4] = 0;
    tm.data[5] = 0;
    tm.data[6] = 0;
    tm.data[7] = 0;


    do {
	/* calc data  */

	tm.data[0]  = 128 * (sin(inc + 0 * (M_2_PI/2)) + 1); 
	tm.data[1]  = 128 * (sin(inc + 1 * (M_2_PI/2)) + 1); 
	tm.data[2]  = 128 * (sin(inc + 2 * (M_2_PI/2)) + 1); 
	tm.data[3]  = 128 * (sin(inc + 3 * (M_2_PI/2)) + 1); 
	tm.data[4]  = 128 * (sin(inc + 4 * (M_2_PI/2)) + 1); 
	tm.data[5]  = 128 * (sin(inc + 5 * (M_2_PI/2)) + 1); 
	tm.data[6]  = 128 * (sin(inc + 6 * (M_2_PI/2)) + 1); 
	tm.data[7]  = 128 * (sin(inc + 7 * (M_2_PI/2)) + 1); 

	ret = (int)write(can_fd, &tm, 1);
	if (ret == -1) {
	    perror("write error");
	    usleep((useconds_t)errwaittime);
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    usleep((useconds_t)errwaittime);
	    continue;
	} else {
	    if ( debug ) {
		printf("transmitted %d\n", ret);
	    }
	}

	inc += 0.1;

	if ( sleeptime > 0 ) {
	    (void)usleep((useconds_t)sleeptime);
	}
    }
    while(true);
    sleep(1);
#endif
}

/* test100:
   - CANopen NMT Message OPERATIONAL
   - CANopen NMT Message PRE-OPERATIONAL
*/
void test100(void)
{
unsigned long int test_count = 0;
canmsg_t tm[5];
int ret;

    tm[0].id = 0;
    tm[0].cob = 0;
    tm[0].length = 2;
    tm[0].flags = 0;
    tm[0].data[0] = 0x01;
    tm[0].data[1] = message.id;    /* !! is CANopen node id */
    tm[0].data[2] = 3;
    tm[0].data[3] = 4;
    tm[0].data[4] = 5;
    tm[0].data[5] = 6;
    tm[0].data[6] = 7;
    tm[0].data[7] = 0xaa;

    tm[1].id = 0;
    tm[1].cob = 0;
    tm[1].length = 2;
    tm[1].flags = 0;
    tm[1].data[0] = 0x80;
    tm[1].data[1] = message.id;
    tm[1].data[2] = 6;
    tm[1].data[3] = 5;
    tm[1].data[4] = 4;
    tm[1].data[5] = 3;
    tm[1].data[6] = 2;
    tm[1].data[7] = 0x55;
 /* 2,3,4 not used yet */
    tm[2].id = message.id;
    tm[2].cob = 0;
    tm[2].length = 0;
    tm[2].flags = 0;
    if (extd) {
	tm[2].flags |= MSG_EXT;
    }

    tm[3].id = message.id;
    tm[3].cob = 0;
    tm[3].length = 8;
    tm[3].flags = 0;
    if (extd) {
	tm[3].flags |= MSG_EXT;
    }
    tm[3].data[0] = 0x55;
    tm[3].data[1] = 2;
    tm[3].data[2] = 3;
    tm[3].data[3] = 4;
    tm[3].data[4] = 5;
    tm[3].data[5] = 6;
    tm[3].data[6] = 7;
    tm[3].data[7] = 0xaa;

    tm[4].id = message.id;
    tm[4].cob = 0;
    tm[4].length = 4;
    tm[4].flags = 0;


    do {
	ret = (int)write(can_fd, &tm[0], 2);
	if (ret == -1) {
	    perror("write error");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else {
	    if ( debug == true ) {
		printf("transmitted %d\n", ret);
	    }
	}
	/* now read the rx queue, it may contain
	 * frames with id -1, which signals an error */
	read_can(can_fd);

	test_count++;

	if(endless != 1) {
	    if (test_count == test_count_soll) {
		break;
	    }
	}

	if ( sleeptime > 0 ) {
	    usleep((useconds_t)sleeptime);
	}
    }
    while(true);
    sleep(1);
}

/* test101:
   -  Send simulated HB messages 

   starting with node id 2
   40 messages

   later use option to set
   HB cycle time and status and lot more.
*/
void test101(void)
{
unsigned long int test_count = 0;
#define NUMBER_OF_HB	40
canmsg_t tm[NUMBER_OF_HB];		/* so much HB messages are produced */
int ret;
int i;


    for(i = 0; i < NUMBER_OF_HB; i++) {
	tm[i].id =  0x702 + i;
	tm[i].cob = 0;
	tm[i].length = 1;
	tm[i].flags = 0;
	tm[i].data[0] = 0x7f;		/* Preop */
    }

    i = 0;
    do {
	ret = (int)write(can_fd, &tm[i], 1);
	if (ret == -1) {
	    perror("write error");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else if (ret == 0) {
	    printf("transmit timed out\n");
	    usleep((useconds_t)errwaittime); 
	    continue;
	} else {
	    if ( debug == true ) {
		printf("transmitted %d\n", ret);
	    }
	}
	/* now read the rx queue, it may contain
	 * frames with id -1, which signals an error */
	read_can(can_fd);

	test_count++;

	if(endless != 1) {
	    if (test_count == test_count_soll) {
		break;
	    }
	}

	i++;
	if(i < NUMBER_OF_HB) {
	    usleep((useconds_t)2000);	/* time between two HBs */
	} else {
	    i = 0;
	    if ( sleeptime > 0 ) {
		usleep((useconds_t)sleeptime);
	    }
	}
    }
    while(true);
    sleep(1);
}
