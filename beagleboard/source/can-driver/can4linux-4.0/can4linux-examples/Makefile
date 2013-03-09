# Make some example applications using can4linux device driver
# (c) 2003-2006 port GmbH oe@port.de
#
# This can be used building stand alone applications on a PC Linux
# for Crosscompiling set CC when calling make
# e.g. for MPLs VCMA9 say:
# make CC=/opt/toolchains/bin/arm-linux-gcc-3.3.2
#

TARGET=LINUX_X86

# ============================================================================
# Target indepentant definitions
CTAGS =	ctags --c-types=dtvf
CTAGS =	elvtags -tsevl
CTAGS =	etags  

# ============================================================================
ifeq "$(TARGET)" "LINUX_X86"
#ARCH  = -m32

#CFLAGS = -Wall -I/usr/src/can4linux -DUSE_RT_SCHEDULING
CFLAGS = -Wall -I../can4linux 
CFLAGS = -g -Wall -I../can4linux -DUSE_RT_SCHEDULING -DCANFD
endif

# ============================================================================
ifeq "$(TARGET)" "LINUX_COLDFIRE"

CROSS = m68k-elf-
ARCH  = -m5307

# IGW900
UCLIBBASE = /home/oertel/pakete/uClinux-dist-20030909-SSV20040610/lib
LIBS  = \
	-L$(UCLIBBASE)/uClibc/. \
	-L$(UCLIBBASE)/uClibc/lib \
	-L$(UCLIBBASE)/libm \
	-L$(UCLIBBASE)/libnet \
	-L$(UCLIBBASE)/libdes \
	-L$(UCLIBBASE)/libaes \
	-L$(UCLIBBASE)/libpcap \
	-L$(UCLIBBASE)/libssl \
	-L$(UCLIBBASE)/libcrypt_old \
	-L$(UCLIBBASE)/libsnapgear++ \
	-L$(UCLIBBASE)/libsnapgear \
	-L$(UCLIBBASE)/zlib \

LDFLAGS = $(CFLAGS) -Wl,-elf2flt -Wl,-move-rodata -nostartfiles  \
	$(UCLIBBASE)/uClibc/lib/crt0.o -Wa,-m5307
DEFS    = -DCONFIG_COLDFIRE -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -DCOLDFIRE 
          
CFLAGSX = -Os -fomit-frame-pointer -fno-builtin -msep-data 

endif

# ============================================================================
ifeq "$(TARGET)" "LINUX_POWERPC"
CROSS=powerpc-linux-
ARCH=missing

CFLAGS = -Wall -I../can4linux -DUSE_RT_SCHEDULING 
endif

# ============================================================================
ifeq "$(TARGET)" "LINUX_BLACKFIN"
PATH += :/opt/uClinux/bfin-uclinux/bin
CROSS=/opt/uClinux/bfin-uclinux/bin/bfin-uclinux-
ARCH=



CC= $(CROSS)gcc
AS=
AR=
LD=

CFLAGS  =  $(ARCH) $(CFLAGSX) -I. -D$(TARGET) $(DEFS)
ASFLAGS	=
INCDIR	=


LDFLAGS = $(CFLAGS) -Wl,-elf2flt 
	
DEFS    = -DCONFIG_BFIN -Dlinux -D__linux__ -Dunix -D__uClinux__ \
	-DEMBED -DBFIN  -Wall
DEFS    = -Wall
CFLAGSX = -Os -fomit-frame-pointer -fno-builtin -msep-data
CFLAGSX =

endif
# ============================================================================



all: ctest baud can_send acceptance noiser receive transmit \
	can_verify receive-select transmit-select \
	transmitblock dump-struct rcnt





# simple applications, using the simple build-in make rule
ctest:			ctest.o
can_verify:		can_verify.o
can_replay:		can_replay.o libcan.o
baud:			baud.o
acceptance:		acceptance.o
noiser:			noiser.o
receive:		receive.o
listen-select:		listen-select.o
receive-select:		receive-select.o
transmit-select:	transmit-select.o
transmit:		transmit.o
transmitblock:		transmitblock.o
transmit2:		transmit2.o
transmit3:		transmit3.o
dump-struct:		dump-struct.o
rcnt:			rcnt.o
canled:			canled.c
timertest:		timertest.c

can_send:		can_send.o
	$(CC) $(CFLAGS) $(LDFLAGS)  can_send.o -lc -o can_send

ctags:
	$(CTAGS) can_send.c ../src/can4linux.h

# create a shared object file for TCL
# (tested with SWIG1.1-883, must be installed)
# Test with canLtwo.tcl
canLtwo.so:	canLtwo.c canLtwo.i ../can4linux/can4linux.h
	swig -tcl canLtwo.i
	$(CC) $(CFLAGS) -c -fpic -I/usr/local/include \
	    canLtwo.c canLtwo_wrap.c
	$(CC) -shared canLtwo.o canLtwo_wrap.o -o $@

clean:
	-rm *.o *.gdb gmon.out \
		ctest acceptance baud can_verify \
		canled \
		transmit transmit2 transmit3 \
		transmit-select transmitblock \
		dump-struct \
		can_replay can_send \
		receive \
		receive-select listen-select \
		noiser \
		rcnt \
		canLtwo.so

# run Flexelint on can_send.c
lint:
	flint  gcc-include-path.lnt size-options.lnt co-gnu3.lnt \
		-format='%f%(:%l%)%(:%c:%)%t\s%n:%m' \
		can_send.c



