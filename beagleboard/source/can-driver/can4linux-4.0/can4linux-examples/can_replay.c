
/* can_replay
*  
*   replay a CAN session recorded with 'horch'
* 
* To do:
*   ALL
* because this is just the idea
*
* How it can be done
* - replay all CAN frames as fast as possible   (without options)
* - use a fixed delay between each frame (in ms or µs, use -s
* - use the time stamp information caontained in the logfile, use  -t)
*/
#define VERSION "SVN $Revision: 89 $"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
#include "libcan.h"


# define STDDEV "/dev/can1"
#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

/* global variables */
int can_fd;
int debug                = FALSE;
int baud		 = -1;		/* dont change baud rate */
int priority		 = -1;		/* dont change priority rate */



void usage(char *s)
{
static char *usage_text  = "\
replay a CAN session recorded with 'horch'\n\
Options:\n\
-d   - debug On\n\
-b baudrate (Standard uses value of /proc/sys/dev/Can/baud)\n\
-D dev use /dev/dev/{can0,can1,can2,can3} (real nodes, std: can1)\n\
";
    fprintf(stderr, "usage: %s [options] logfile ..\n", s);
    fprintf(stderr, usage_text);
}

void processfile(FILE *fp)
{

}

/**
*
* The main program
*
*/
int main(int argc, char **argv)
{
int ret;
int c;
char *pname;
extern char *optarg;
extern int optind;
char device[64] = STDDEV;
int max_priority;

    pname = *argv;

#ifdef USE_RT_SCHEDULING
    max_priority = sched_get_priority_max(SCHED_RR) - 1;
#else 
    max_priority = 1;
#endif


    /* parse command line */
    while ((c = getopt(argc, argv, "b:dhp:D:VR")) != EOF) {
	switch (c) {
	    case 'b':
		baud = atoi(optarg);
		break;
	    case 'p':
	        {
#ifdef USE_RT_SCHEDULING
	        struct sched_param mysched;
		    priority = atoi(optarg);
		    if (priority < 0 ) {
		      fprintf(stderr, "Priority < 0 not allowed\n");
		    }
		    if (priority > max_priority) {
		      fprintf(stderr, "Priority > %d not allowed\n",
		      					max_priority);
		    }
		    mysched.sched_priority =
		    		sched_get_priority_max(SCHED_RR) - 1;
		    ret = sched_setscheduler(0,SCHED_FIFO,&mysched);
		    if ( debug == TRUE ) {
			printf("sched_setscheduler() = %d\n", ret);
		    }
		    /* lock all currently and in future
			allocated memory blocks in physical ram */
		    ret = mlockall(MCL_CURRENT | MCL_FUTURE);
		    if ( debug == TRUE ) {
			printf("mlockall() = %d\n", ret);
		    }
#endif
		}
		break;
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
	    case 'd':
		debug = TRUE;
		break;
	    case 'V':
		printf("can_replay " VERSION ", " __DATE__ "\n");
		exit(0);
		break;
	    case 'R':
		can_reset(); exit(0);
		break;
	    case 'h':
	    default: usage(pname); exit(0);
	}
    }


    /* look for additional arguments given on the command line */
    if ( argc - optind > 0 ) {
        /* at least one additional argument, the message log is given */
	while(optind != argc) {
	FILE *fp;
	    char *logfile =  argv[optind++];
	    printf("replay %s\n", logfile);
	    fp = fopen(logfile, "r");
	    if(fp == NULL) {
	        fprintf(stderr, "file '%s' ", logfile);
		perror("open error");
		exit(1);
	    }
	    processfile(fp);
	}
    }
    exit(0);
}


