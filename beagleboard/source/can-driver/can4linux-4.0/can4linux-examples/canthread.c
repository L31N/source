/*
 * canthred - demonstration of using can4linux with threads
 *
 * Copyright (c) 2008 port GmbH Halle/Saale
 *------------------------------------------------------------------
 * $Header: /z2/cvsroot/products/0530/software/can4linux-examples/canthread.c,v 1.1 2008/06/23 06:14:30 hae Exp $
 *
 *--------------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log: canthread.c,v $
 * Revision 1.1  2008/06/23 06:14:30  hae
 * initial checkin
 *
 *
 *--------------------------------------------------------------------------
 */


/**
* \file canthread.c
* \author Name, port GmbH
* $Revision: 1.1 $
* $Date: 2008/06/23 06:14:30 $
*
*
*/


/* header of standard C - libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

/* header of common types */
#include <pthread.h>
#include <can4linux.h>

/* shared common header */

/* header of project specific types */

/* project headers */

/* local header */

/* constant definitions
---------------------------------------------------------------------------*/
#define VERSION "1.0"

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


#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

#ifndef CO_CONST
# define CO_CONST const
#endif

#define MAX_DEVNAME 32

#define MAX_THREADS 2
#define RX_THREAD 0
#define TX_THREAD 1

/* local defined data types
---------------------------------------------------------------------------*/
typedef struct {
    int tno;

    int can_fd;
    int loop_cnt;
} thread_data_t;

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/
static void *can_rx_thread( void *ptr );
static void *can_tx_thread( void *ptr );

static int set_bitrate( int fd, int baud );
static int can_start( int fd );
static int can_stop( int fd );
static int can_reset( int fd );

static void usage (char *s);

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static CO_CONST char _rcsid[] = "$Id: canthread.c,v 1.1 2008/06/23 06:14:30 hae Exp $";
#endif

static int o_debug;
static int o_bitrate = 125;
static char o_device[MAX_DEVNAME];

static pthread_t threads[MAX_THREADS];
static thread_data_t td[MAX_THREADS];


/***************************************************************************/
/**
*
* \brief one line short description 
*
* Description with
* \code
* examples
* \endcode
*
* \returns
* some text
* \retval THIS means this
* \retval THAT means anothe thing
*
* \internal
*/
int main (
	int argc,		/**< */
	char **argv			/**< */
	)
{
int c;
int can_fd;

    while ((c = getopt(argc, argv, "b:dD:hV")) != EOF) {
	switch (c) {
	    case 'b':
		o_bitrate = atoi(optarg);
		break;
	    case 'D':
		if (
		    /* path ist starting with '.' or '/', use it as it is */
			optarg[0] == '.'
			|| 
			optarg[0] == '/'
			) {
		    snprintf(o_device, MAX_DEVNAME, "%s", optarg);

	        } else {
		    snprintf(o_device, MAX_DEVNAME, "/dev/%s", optarg);
		}
		break;
	    case 'd':
		o_debug = TRUE;
		break;

	    case 'V':
		printf("%s V " VERSION ", " __DATE__ "\n", *argv);
		exit(0);
		break;

	    case 'h': /* fall through */
	    default:
	    	usage(*argv);
	    	exit(0);
	}
    }

    if (o_debug == 1) {
	fprintf(stderr, "%s V " VERSION ", " __DATE__ "\n", *argv);
	fprintf(stderr, "Opening %s with %d KBit/s\n", o_device, o_bitrate);
    }

    can_fd = open(o_device, O_RDWR | O_NONBLOCK);
    if (can_fd == -1) {
	fprintf(stderr, "Open error %d;", errno);
	perror(o_device);
	exit(1);
    }
    set_bitrate(can_fd, o_bitrate);

    td[RX_THREAD].tno = 1;
    td[RX_THREAD].can_fd = can_fd;
    td[RX_THREAD].loop_cnt = 10000;
    pthread_create(&threads[RX_THREAD], NULL,
		can_rx_thread, (void *) &td[RX_THREAD]);


    /* need to protect resource can_fd with a mutex */
    td[TX_THREAD].tno = 2;
    td[TX_THREAD].can_fd = can_fd;
    td[TX_THREAD].loop_cnt = 100;
    pthread_create(&threads[TX_THREAD], NULL,
		can_tx_thread, (void *) &td[TX_THREAD]);

    pthread_join(threads[RX_THREAD], NULL);
    pthread_join(threads[TX_THREAD], NULL);

    return 0;
}

/***************************************************************************/
/**
*
* CAN thread that reads from the CAN controller
*
*
* \retval 0
*
*/
static void *can_rx_thread (
	void *ptr
    )
{
int i;
int ret;
int tno;
int can_fd;
int loop_cnt;
char type;

pthread_t tid;
canmsg_t rx;

    tid = pthread_self();
    tno = ((thread_data_t *) ptr)->tno;
    fprintf(stderr, "Thread(%d) started: Id = %x\n",  tno, (int)tid);

    can_fd      = ((thread_data_t *) ptr)->can_fd;
    loop_cnt    = ((thread_data_t *) ptr)->loop_cnt;

    if (o_debug) {
	fprintf(stderr, "Thread(%d) can_fd: %d, loop_cnt: %d\n",
				tno, can_fd, loop_cnt);
    }

    do {
	ret = read(can_fd, &rx, 1);
	if (ret <= 0 ) {
	    /* perror("read error"); */
	    usleep(200);
	} else {

	    /* if (-1ul == rx.id)  */
	    if (CANDRIVERERROR == rx.id) {
		perror("driver error");
		usleep(200);
	    } else {

	        type = (rx.flags & MSG_EXT) ? 'x' : 's';

	        if ((rx.flags & MSG_RTR) != 0) {
		    printf("%4ld/0x%03lx : %cR : (length = %d)\n",
		    	rx.id, rx.id, type, rx.length );
		} else {
		    printf("%4ld/0x%03lx : %cD : ",
		    	rx.id, rx.id, type );
		    for (i=0; i<rx.length; i++) {
			printf("%02x ", rx.data[i]);
		    }
		    printf("\n");
		}
	    }
	}
        loop_cnt--;
    } while (loop_cnt > 0);

    fprintf(stderr, "Thread(%d) exit\n", tno);
    pthread_exit(NULL);
}

/***************************************************************************/
/**
*
* CAN thread that wirte to the CAN controller
*
* \retval 0
*
*/
static void *can_tx_thread (
	void *ptr
    )
{
int i;
int ret;
int tno;
int can_fd;
int loop_cnt;

pthread_t tid;
canmsg_t tx;

    tid = pthread_self();
    tno = ((thread_data_t *) ptr)->tno;
    fprintf(stderr, "Thread(%d) startet: Id = %x\n",  tno, (int)tid);

    can_fd      = ((thread_data_t *) ptr)->can_fd;
    loop_cnt    = ((thread_data_t *) ptr)->loop_cnt;

    if (o_debug) {
	fprintf(stderr, "Thread(%d) can_fd: %d, loop_cnt: %d\n",
				tno, can_fd, loop_cnt);
    }

    tx.id = 100;
    tx.length = 8;
    for (i=0; i< tx.length; i++) {
	tx.data[i] = i;
    }

    do {
        ret = write(can_fd, &tx, 1);
        if (ret < 0) {
	    perror("write error");
        } else {
            /* fprintf(stderr, "ret %d\n", ret); */
            /* fprintf(stderr, "loop_cnt %d\n", loop_cnt); */
        }
	usleep(200);

        loop_cnt--;
    } while (loop_cnt > 0);

    fprintf(stderr, "Thread(%d) exit\n", tno);

    pthread_exit(NULL);
}

/***************************************************************************/
/**
*
* Set CAN bitrate.
*
* Stops the program when setting the bitrate fails.
*
* \retval 0
*
*/
static void usage (char *s) {
static char *usage_text  = "\
Options:\n\
-b baudrate (Standard uses value of /proc/sys/Can/baud)\n\
-d   - debug On\n\
       schaltet zusaetzlich Debugging im Treiber an/aus\n\
-D dev use /dev/dev/{can0,can1,can2,can3} (real nodes, std: can1)\n\
-h show this help \n\
-V   print program version\n\
\n\
";

     fprintf(stderr, "usage: %s [options] [id [ byte ..]]\n", s);
     fprintf(stderr, usage_text);
}

/***************************************************************************/
/**
*
* Set CAN bitrate.
*
* Stops the program when setting the bitrate fails.
*
* \retval 0
*
*/
static int set_bitrate(
	int fd,			/**< device descriptor */
	int baud		/**< bit rate */
    )
{
int ret;
volatile Config_par_t  cfg;

    ret = can_stop(fd);
    
    cfg.target = CONF_TIMING; 
    cfg.val1   = baud;
    ret = ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

    ret = can_start(fd);

    if (ret < 0) {
	perror("set_bitrate");
	exit(-1);
    } else {
	ret = 0;
    }
    return ret;
}

/***************************************************************************/
/**
*
* Reset the CAN-Controller
*
* \retval 0 success
* \retval !=0 failure
*
*/
static int can_reset(
	int fd			/**< device descriptor */
    )
{
int ret;
volatile Command_par_t cmd;


    cmd.cmd = CMD_RESET;
    ret = ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    return ret;
}

/***************************************************************************/
/**
*
* Start the CAN-Controller
*
* \retval 0 success
* \retval !=0 failure
*
*/
static int can_start(
	int fd			/**< device descriptor */
    )
{
int ret;
volatile Command_par_t cmd;


    cmd.cmd = CMD_CLEARBUFFERS;
    ret = ioctl(fd, CAN_IOCTL_COMMAND, &cmd);
    cmd.cmd = CMD_START;
    ret = ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    return ret;
}


/***************************************************************************/
/**
*
* Stop the CAN-Controller
*
* \retval 0 success
* \retval !=0 failure
*
*/
static int can_stop (
	int fd			/**< device descriptor */
    )
{
int ret;
volatile Command_par_t cmd;


    cmd.cmd = CMD_STOP;
    ret = ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

    return ret;
}

/*______________________________________________________________________EOF_*/
