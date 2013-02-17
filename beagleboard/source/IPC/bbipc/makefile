# -*- mode: Makefile; -*-
# -----------------------------------------
# project bbipc


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

CFLAGS = -fpermissive -m32 
INCLUDES = -I/home/l31n/robotics.git/source/beagleboard/include 
LDFLAGS = -m32 -lpthread -L/home/l31n/robotics.git/source/beagleboard/lib  -s
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
	
all.targets : Debug

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean

# -----------------------------------------
# Debug

Debug.BIN = libbbipc.a
Debug.OBJ = bbipc.o buffer.o ipc_config.o ipc_connection.o 
DEP_FILES += bbipc.d buffer.d ipc_config.d ipc_connection.d 
clean.OBJ += $(Debug.BIN) $(Debug.OBJ)

Debug : Debug.before $(Debug.BIN) Debug.after_always
Debug : CFLAGS += -Wall -g -DDEBUG  -Os
Debug : INCLUDES += 
Debug : RCFLAGS += 
Debug : LDFLAGS +=   $(CREATE_DEF)
Debug : T_LDLIBS = 
ifdef LMAKE
Debug : CFLAGS -= -O1 -O2 -g -pipe
endif

Debug.before :
	
	
Debug.after_always : $(Debug.BIN)
	
$(Debug.BIN) : $(Debug.OBJ)
	$(LINK_lib)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
