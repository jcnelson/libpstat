CPP    := g++ -Wall -g -fPIC
LIB   := -lssl -lcrypto
INC   := -I/usr/include -I/usr/local/include -I. -Ios/
C_SRCS:= $(wildcard *.c) $(wildcard os/*.c)
CXSRCS:= $(wildcard *.cpp) $(wildcard os/*.cpp)
TOOL_C_SRCS := $(wildcard tools/*.c)
TOOL_CXSRCS := $(wildcard tools/*.cpp)

HEADERS := $(wildcard *.h) $(wildcard os/*.h)
OBJ   := $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CXSRCS))
TOOL_OBJ := $(patsubst %.c,%.o,$(TOOL_SRCS)) $(patsubst %.cpp,%.o,$(TOOL_CXSRCS))

DEFS  := -D_REENTRANT -D_THREAD_SAFE -D__STDC_FORMAT_MACROS

LIBPSTAT := libpstat.so
LIBPSTAT_SO := libpstat.so.1
LIBPSTAT_LIB := libpstat.so.1.0.1

PSTAT := tools/pstat

DESTDIR := /usr/local/bin
LIBDIR := /usr/local/lib
INCDIR  := /usr/local/include/pstat

OS := UNKNOWN

all: libpstat pstat

libpstat: $(OBJ)
	$(CPP) -shared -Wl,-soname,$(LIBPSTAT_SO) -o $(LIBPSTAT_LIB) $(OBJ) $(LIBINC) $(LIB)
	$(SHELL) -c "if ! test -L $(LIBPSTAT_SO); then /bin/ln -s $(LIBPSTAT_LIB) $(LIBPSTAT_SO); fi"
	$(SHELL) -c "if ! test -L $(LIBPSTAT); then /bin/ln -s $(LIBPSTAT_SO) $(LIBPSTAT); fi"

pstat: $(TOOL_OBJ)
	$(CPP) -o $(PSTAT) $(TOOL_OBJ) $(LIBINC) -L. -lpstat

install: libpstat pstat
	/bin/cp -a $(PSTAT) $(DESTDIR)
	/bin/cp -a $(LIBPSTAT) $(LIBPSTAT_SO) $(LIBPSTAT_LIB) $(LIBDIR)
	/bin/mkdir -p $(INCDIR) $(INCDIR)/os
	/bin/cp -a $(wildcard os/*.h) $(INCDIR)/os
	/bin/cp -a $(wildcard *.h) $(INCDIR)

%.o : %.c
	$(CPP) -o $@ $(INC) -c $< $(DEFS) -D_$(OS)

%.o : %.cpp
	$(CPP) -o $@ $(INC) -c $< $(DEFS) -D_$(OS)

.PHONY: clean
clean:
	/bin/rm -f $(OBJ) $(TOOL_OBJ) $(LIBPSTAT_LIB) $(LIBPSTAT_SO) $(LIBPSTAT) $(PSTAT)
