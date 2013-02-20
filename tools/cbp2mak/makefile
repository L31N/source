# -----------------------------------------------------------
ifeq "$(shell uname)" "Linux"
# -----------------------------------------------------------
e =
a = .so
d = .so
l = ./

COMPILE_c = gcc -c $< -o $@  $(CFLAGS)
COMPILE_cpp = g++ -c $< -o $@  $(CFLAGS)
LINK_exe = gcc -s -o $@ $^ -lstdc++ -Wl,-rpath $(CURDIR)
LINK_dll = gcc -s -o $@ $^ -lstdc++ -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
CFLAGS = -Wall -Os

# -----------------------------------------------------------
else
# -----------------------------------------------------------
# Windows

SHELL = cmd
e = .exe
a = .lib
d = .dll
l =

# MSC = 1
ifdef MSC
CFLAGS = -nologo -W3 -Zi -MD -O1
COMPILE_c = cl -c $< -Fo$@ $(CFLAGS)
COMPILE_cpp = $(COMPILE_c)
LINK_exe = link -nologo -out:$@ $^ -subsystem:console -debug -opt:nowin98,ref,icf
LINK_dll = $(LINK_exe) -dll
else
CFLAGS = -Wall -Os
COMPILE_c = gcc -c $< -o $@  $(CFLAGS)
COMPILE_cpp = g++ -c $< -o $@  $(CFLAGS)
LINK_exe = gcc -s -o $@ $^ -lstdc++
LINK_dll = gcc -s -o $@ $^ -lstdc++ -shared -Wl,--out-implib=$(basename $@).lib
LINK_lib = rm -f $@ && ar rcs $@ $^
endif
# -----------------------------------------------------------
endif
# -----------------------------------------------------------

TARGETS = txml$(d) xml2c$(e) cbp2mak$(e)
TXMLOBJ = tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o
VPATH = tinyxml

all : $(TARGETS)

%.o : %.c
	$(COMPILE_c)
%.o : %.cpp
	$(COMPILE_cpp)
%.c : %.xml xml2c$(e)
	$(l)xml2c$(e) $*.xml $*.h $*.c

xml2c$(e) : xml2c.o txml$(a)
	$(LINK_exe)
cbp2mak$(e) : cbp2mak.o txml$(a)
	$(LINK_exe)
txml$(d) : $(TXMLOBJ) txml.o
	$(LINK_dll)

cbp2mak.o : cbp.c cbw.c

txml.o : CFLAGS += -I tinyxml

clean :
	rm -rvf $(TARGETS) *.lib *.a *.o *.pdb *.exp *.def cbp.? cbw.? lmakestate

# -----------------------------------------------------------
