# -*- mode: Makefile; -*-
[[projects
# -----------------------------------------
# project [P]
[custom_make]

export PATH := /opt/wx/2.8/bin:$(PATH)
export LD_LIBRARY_PATH := /opt/wx/2.8/lib:$(LD_LIBRARY_PATH)

_WX = /home/gr/projects/gui/codeblocks/wx
_WX.LIB = $(_WX)/lib
_WX.INCLUDE = $(_WX)/include

_CB = /home/gr/projects/gui/codeblocks/cb/src
_CB.INCLUDE = $(_CB)/include
_CB.LIB = $(_CB)/devel

[environment]

CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = [cflags]
INCLUDES = [includes]
LDFLAGS = [ldflags] -s
RCFLAGS = [rcflags]
LDLIBS = $(T_LDLIBS) [ldlibs] -lstdc++

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

[vtargets]
all.before :
	-[before]
all.after : $(FIRST_TARGET)
	[after]
all.targets : [targets]

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean

[[targets
# -----------------------------------------
# [T]

[T].BIN = [output]
[T].OBJ = [ofiles]
DEP_FILES += [dfiles]
clean.OBJ += $([T].BIN) $([T].OBJ)

[T] : [T].before $([T].BIN) [T].after_always
[T] : CFLAGS += [cflags] -Os
[T] : INCLUDES += [includes]
[T] : RCFLAGS += [rcflags]
[T] : LDFLAGS += [ldflags] [create_lib] [create_def]
[T] : T_LDLIBS = [ldlibs]
ifdef LMAKE
[T] : CFLAGS -= -O1 -O2 -g -pipe
endif

[T].before :
	[before]
	[before_always]
[T].after_always : $([T].BIN)
	[after_always]
$([T].BIN) : $([T].OBJ)
	$(LINK_[link])[command]
	[after]

]]
# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
]]
[[workspace
# -----------------------------------------
# workspace [W]

all :

% :
[[projects
	$(MAKE) -C [cbp_path] -f [cbp_mak] $@
]]
]]
