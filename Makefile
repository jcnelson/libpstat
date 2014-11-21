CPP    := g++ -Wall -g -fPIC
LIB   := -lssl -lcrypto
INC   := -I/usr/include -I/usr/local/include -I. -Ios/
C_SRCS:= $(wildcard *.c) $(wildcard os/*.c)
CXSRCS:= $(wildcard *.cpp) $(wildcard os/*.cpp)
HEADERS := $(wildcard *.h) $(wildcard os/*.h)
OBJ   := $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CXSRCS))
DEFS  := -D_REENTRANT -D_THREAD_SAFE -D__STDC_FORMAT_MACROS

LIBPSTAT := libpstat.so
LIBPSTAT_SO := libpstat.so.1
LIBPSTAT_LIB := libpstat.so.1.0.1

DESTDIR := /usr/local/lib
INCDIR  := /usr/local/include/pstat

OS := UNKNOWN

all: pstat

pstat: $(OBJ)
	$(CPP) -shared -Wl,-soname,$(LIBPSTAT_SO) -o $(LIBPSTAT_LIB) $(OBJ) $(LIBINC) $(LIB)
	$(SHELL) -c "if ! test -L $(LIBPSTAT_SO); then /bin/ln -s $(LIBPSTAT_LIB) $(LIBPSTAT_SO); fi"
	$(SHELL) -c "if ! test -L $(LIBPSTAT); then /bin/ln -s $(LIBPSTAT_SO) $(LIBPSTAT); fi"

install: pstat
	/bin/cp -a $(LIBPSTAT) $(LIBPSTAT_SO) $(LIBPSTAT_LIB) $(DESTDIR)
	/bin/mkdir -p $(INCDIR)
	/bin/cp -a $(wildcard *.h) os/ $(INCDIR)

%.o : %.c
	$(CPP) -o $@ $(INC) -c $< $(DEFS) -D_$(OS)

%.o : %.cpp
	$(CPP) -o $@ $(INC) -c $< $(DEFS) -D_$(OS)

.PHONY: clean
clean:
	/bin/rm -f $(OBJ) $(LIBPSTAT_LIB) $(LIBPSTAT_SO) $(LIBPSTAT)
