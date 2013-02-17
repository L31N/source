# -*- mode: Makefile; -*-
# -----------------------------------------
# project debug-user


export PATH := /opt/wx/2.8/bin:$(PATH)
export LD_LIBRARY_PATH := /opt/wx/2.8/lib:$(LD_LIBRARY_PATH)

_WX = /home/gr/projects/gui/codeblocks/wx
_WX.LIB = $(_WX)/lib
_WX.INCLUDE = $(_WX)/include

_CB = /home/gr/projects/gui/codeblocks/cb/src
_CB.INCLUDE = $(_CB)/include
_CB.LIB = $(_CB)/devel



CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = -Wall -fexceptions -m32 
INCLUDES = -I../../../include 
LDFLAGS = -lbbipc -lbbdebug -lpthread -m32 -L../../../lib  -s
RCFLAGS = 
LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_con = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_dll = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS) -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
COMPILE_c = gcc $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_cpp = g++ $(CFLAGS) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_rc = windres $(RCFLAGS) -J rc -O coff -i $< -o $@ -I$(dir $<)

%.o : %.c ; $(COMPILE_c)
%.o : %.cpp ; $(COMPILE_cpp)
%.o : %.cxx ; $(COMPILE_cpp)
%.o : %.rc ; $(COMPILE_rc)
.SUFFIXES: .o .d .c .cpp .cxx .rc

all: all.before all.targets all.after

all.before :
	-
all.after : $(FIRST_TARGET)
	
all.targets : DebugRelease

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean

# -----------------------------------------
# Debug

Debug.BIN = bin/Debug/debug-user
Debug.OBJ = debug-user.o 
DEP_FILES += debug-user.d 
clean.OBJ += $(Debug.BIN) $(Debug.OBJ)

Debug : Debug.before $(Debug.BIN) Debug.after_always
Debug : CFLAGS += -g  -Os
Debug : INCLUDES += 
Debug : RCFLAGS += 
Debug : LDFLAGS +=   
Debug : T_LDLIBS = 
ifdef LMAKE
Debug : CFLAGS -= -O1 -O2 -g -pipe
endif

Debug.before :
	
	
Debug.after_always : $(Debug.BIN)
	
$(Debug.BIN) : $(Debug.OBJ)
	$(LINK_con)
	

# -----------------------------------------
# Release

Release.BIN = bin/Release/debug-user
Release.OBJ = debug-user.o 
DEP_FILES += debug-user.d 
clean.OBJ += $(Release.BIN) $(Release.OBJ)

Release : Release.before $(Release.BIN) Release.after_always
Release : CFLAGS += -O2  -Os
Release : INCLUDES += 
Release : RCFLAGS += 
Release : LDFLAGS += -s   
Release : T_LDLIBS = 
ifdef LMAKE
Release : CFLAGS -= -O1 -O2 -g -pipe
endif

Release.before :
	
	
Release.after_always : $(Release.BIN)
	
$(Release.BIN) : $(Release.OBJ)
	$(LINK_con)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
