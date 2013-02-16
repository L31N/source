[[projects
# -----------------------------------------
# project [P]

all : [targets]

P_CFLAGS = [cflags]
P_INCLUDES = [includes]
P_LDFLAGS = [ldflags]
P_RCFLAGS = [rcflags]
P_LDLIBS = [ldlibs]

SYSLIBS = kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib \
    comdlg32.lib shlwapi.lib advapi32.lib version.lib ole32.lib uuid.lib

LDLIBS = $(P_LDLIBS) $(T_LDLIBS) $(SYSLIBS)
CFLAGS = -nologo -W3 -Zi -MD -O1 -DWIN32
LDFLAGS = -nologo -debug -opt:nowin98,ref,icf $(P_LDFLAGS) $(T_LDFLAGS)
INCLUDES = $(P_INCLUDES) $(T_INCLUDES)

LINK_con = link -nologo -out:$@ $** -subsystem:console $(LDFLAGS) $(LDLIBS)
LINK_exe = link -nologo -out:$@ $** -subsystem:windows $(LDFLAGS) $(LDLIBS)
LINK_dll = $(LINK_exe) -dll
LINK_lib = del $@ && lib -out:$@ $**

COMPILE_c = cl -Fo$@ $(CFLAGS) $(INCLUDES) -c
COMPILE_cpp = cl -Fo$@ $(CFLAGS) -EHsc $(INCLUDES) -c
COMPILE_rc = rc $(INCLUDES) /fo$@

.c.obj : ; $(COMPILE_c) $<
.cpp.obj : ; $(COMPILE_cpp) $<
.rc.obj : ; $(COMPILE_rc) $<

.SUFFIXES: .o .d .c .cpp .cxx .rc

[[targets
# -----------------------------------------
# [T]

[T]_BIN = [output]
[T]_OBJ = [ofiles]

T_CFLAGS = [cflags]
T_INCLUDES = [includes]
T_RCFLAGS = [rcflags]
T_LDFLAGS = [ldflags]
T_LDLIBS = [ldlibs]

[T] : $([T]_BIN)
$([T]_BIN) : $([T]_OBJ)
	$(LINK_[link])[command]

[[units
[obj] : [src]
	$(COMPILE_[comp]) [src]
]]

clean_OBJ = $(clean_OBJ) $([T]_BIN) $([T]_OBJ)
]]

clean : ; rm -fv $(clean_OBJ) *.pdb *.lib *.exp
]]

[[workspace
# -----------------------------------------
# workspace [W]

all :
all clean :
[[projects
	cd [cbp_path]
	$(MAKE) -f [cbp_mak] $@
	cd $(MAKEDIR)
]]
]]
