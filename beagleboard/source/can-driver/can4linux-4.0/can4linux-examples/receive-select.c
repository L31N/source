/*
 *  $Id: receive-select.c $
 */

/*
 * receive-select.c
 *
 * Copyright (c) 2000-2012  Heinz-Jürgen Oertel
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <hj.oertel@t-online.de>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <can4linux.h>

/* took this from candump.c , which is a sister project */
#include "terminal.h"	/* for coloring output */


#define DEBUG 0

#define STDDEV "/dev/can0"		/* which is used if called without */
#define MAX_DEVNAME_LENGTH 64
#define MAX_DEVICES 10
#define RXBUFFERSIZE 40


/* got this from candump.c */
#define MAXCOL 6      /* number of different colors for colorized output */
#define BOLD    ATTBOLD
#define RED     ATTBOLD FGRED
#define GREEN   ATTBOLD FGGREEN
#define YELLOW  ATTBOLD FGYELLOW
#define BLUE    ATTBOLD FGBLUE
#define MAGENTA ATTBOLD FGMAGENTA
#define CYAN    ATTBOLD FGCYAN

const char col_on [MAXCOL][19] = {BLUE, RED, GREEN, BOLD, MAGENTA, CYAN};
const char col_off [] = ATTRESET;





/* --------------------------------------------------------------------------*/

char device[MAX_DEVNAME_LENGTH]; /* string to hold the CAN device Name */
fd_set rfds;		/* file descriptors for select() */
int fd[MAX_DEVICES];	/* open file descriptors */
char *devname[MAX_DEVICES]; /* used devices */
int current_open = 0;	/* current open devices */


int color = 1;		/* color is used or not */





/**************************************************************************
*
* clean - called at exit to switch console back in sane mode
*
*/
static void clean(void)
{
    system("stty sane");
}

/* get_device_names()

here is some space to improve:

if only one device name is given and its name is "any"
than scan for all available interfaces /dev/can*
  
*/
void get_device_names(int argc, char **argv)
{
int i;
int j;

    for(i = 1, j = 0; i < argc; i++) {
	/* printf("argc %d, argv[%d] %s\n", argc, i, argv[i]); */
	if (
	    /* path is starting with '.' or '/', use it as it is */
		*argv[i] == '.'
		|| 
		*argv[i] == '/'
		) {
	    snprintf(device, MAX_DEVNAME_LENGTH, "%s", argv[i]);
	} else {
	    snprintf(device, MAX_DEVNAME_LENGTH, "/dev/%s", argv[i]);
	}
	/* printf("try to open %s\n", device); */
	/* try to open this device */
	if(( fd[j] = open(device, O_RDWR )) < 0 ) {
	    fprintf(stderr,"Error opening CAN device %s\n", device);
	    continue;
	}
	devname[j] = argv[i];
	j++;	/* increment number of open devices */
    }
    current_open = j; 	/* set number of open devices */
}

static int can_reset(int can_fd) {
int ret;
volatile Command_par_t cmd;


    cmd.cmd = CMD_RESET;
    ret = ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);

    return ret;
}

static int can_start(int can_fd) {
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
static void displayerror(int device, canmsg_t *rx)
{
/* static int buffcnt = 0; */

	fprintf(stderr, "Flags 0x%02x,", rx->flags);
	if( 0 == (rx->flags & MSG_ERR_MASK)) {
		fprintf(stderr, " CAN Error Free");
	}
	if( rx->flags & MSG_WARNING) {
		fprintf(stderr, " CAN Warning Level,");
	}
	if( rx->flags & MSG_PASSIVE) {
		fprintf(stderr, " CAN Error Passive,");
	}
	if( rx->flags & MSG_BUSOFF) {
		fprintf(stderr, " CAN Bus Off,");
		can_reset(fd[device]);
		/* sleep 100ms */
		usleep (100000);
		can_start(fd[device]);
	}
	/* printf("\n"); */
}

void format_message(int dev, canmsg_t *rx)
{
int j;
char *format;

    /* switch on line color */
    printf("%s", (color && (color < 3)) ? col_on[dev%MAXCOL] : "");

    if(rx->id == CANDRIVERERROR) {
	fprintf(stderr, "%10s: CAN Error, ", devname[dev]);
	displayerror(dev, rx);
    } else {
    /* no error */
	printf("%10s: %12lu.%06lu %8ld/0x%08lx",
			    devname[dev],
			    rx->timestamp.tv_sec,
			    rx->timestamp.tv_usec,
			    rx->id, rx->id);

	if(rx->flags & MSG_CANFD ) {
	    format = " : %c%c [%2d]:";
	} else {
	    format = " : %c%c (%2d):";
	}
	printf(format,
		    (rx->flags & MSG_EXT) ? 'e' : 'b',
		    (rx->flags & MSG_RTR) ? 'R' : 'D',
		    rx->length );
	if( !(rx->flags & MSG_RTR) ) {
	    /* For CAN FD we have to calculate the number of bytes to display
	       according the information of the data length code (dlc) */


	    if(!(rx->flags & MSG_CANFD) && (rx->length > 8)) {
		rx->length = 8;		/* restrict to 8 byte */
	    }
	    if(rx->flags & MSG_CANFD) {
		switch(rx->length) {
		    case  9:
			rx->length = 12;
			break;
		    case 10:
			rx->length = 16;
			break;
		    case 11:
			rx->length = 20;
			break;
		    case 12:
			rx->length = 24;
			break;
		    case 13:
			rx->length = 32;
			break;
		    case 14:
			rx->length = 48;
			break;
		    case 15:
			rx->length = 64;
			break;
		    default:
			fprintf(stderr, "unknown dlc %d\n", rx->length);
			break;
		}
	    }
	    for(j = 0; j < rx->length; j++) {
		printf(" %02x", rx->data[j]);
	    }
	}
    }
    /* switch back color */
    printf("%s\n", (color > 0) ? col_off :"");
    fflush(stdout);
}


/* ==========================================================================
* 
*/
int main(int argc,char **argv)
{
int got;
canmsg_t rx[RXBUFFERSIZE];
int i;

    printf("usage: %s <devices>]\n", argv[0]);
    printf("   e.g.:\n");
    printf("   %s /dev/can0 .... \n", argv[0]);
    printf("   wait for CAN messages using select()\n");
    printf("   DLC in () Standard CAN frame, in [] for CAN FD frames\n");
    printf("   bit rate used is bit rate set in /proc/sys/dev/Can/Baud\n");
    printf("   bit rates used: "); fflush(stdout);
    system("cat  /proc/sys/dev/Can/Baud"); 
    if (argc < 2) {
    	char *argv[] = { "", STDDEV};
	argc = 2;
    	get_device_names(argc, argv);
    } else {
    	get_device_names(argc, argv);
    }
#if DEBUG
    for(i = 0; i < current_open; i++) {
	printf("> device %s using fd %d\n", devname[i], fd[i]);
    }
#endif

    /* prepare clean up console mode at exit */
    atexit(clean);
    /* switch terminal in raw mode */
    i = system("stty cbreak -echo");
    if(i != 0) {
	fprintf(stderr, "  system(stty) returns %d\n", i);
	fflush(stderr);
    }

    int maxfd = fd[current_open - 1] + 1;
    int quit = 0;
    while(!quit) {
        FD_ZERO(&rfds);
	for (i = 0; i < current_open; i++) {
	    FD_SET(fd[i], &rfds); /* watch on every fd for CAN */
        }
        FD_SET(0, &rfds);		/* watch on fd for stdin */


	/* no wait for something to happen */
	if( select(maxfd, &rfds, NULL, NULL, NULL  ) > 0 )

	    for (i = 0; i < current_open; i++) {
		/* check all open file descriptors */

		if( FD_ISSET(fd[i], &rfds) ) {

		    got = read(fd[i], &rx, 1);

		    if( got > 0) {
			format_message(i, &rx[0]);
		    } else {
			printf("Received with ret=%d\n", got);
			fflush(stdout);
		    }
		}
	    }
	    if( FD_ISSET(0, &rfds) ) {
	    int c, i;
		/* it was the stdio terminal fd */
		i = read(0 , &c, 1);
		/* printf(" key = %x\n", c); */
		switch(c & 0xff) {  /* should work with utf8 and 8859 */
		    case 'l':
			system("clear");
			break;
		    case 'c':
			printf(
			"--------------------------------------------------\n");
			break;
		    case 'q':
		    	/* quit */
		    	quit = 1;
		    	break; 	/* while loop */
		    case '?':
		    case 'h':
		        printf(" -c  draw a \"cut\" line\n");
		        printf(" -l  erase screen\n");
			break;
		}

	    } /* stdio fd */
    }
    for (i = 0; i < current_open; i++) {
	close(fd[i]);
    }
    return 0;
}
