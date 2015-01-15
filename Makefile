CPP    := gcc -Wall -g -fPIC
LIB   := 
LIB_USE_SSL := -lssl -lcrypto
INC   := -I. -Ios/ -I/usr/include -I/usr/local/include 
C_SRCS:= $(wildcard *.c) $(wildcard os/*.c)
CXSRCS:= $(wildcard *.cpp) $(wildcard os/*.cpp)
TOOL_C_SRCS := $(wildcard tools/*.c)
TOOL_CXSRCS := $(wildcard tools/*.cpp)

HEADERS := $(wildcard *.h) $(wildcard os/*.h)
OBJ   := $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CXSRCS))
TOOL_OBJ := $(patsubst %.c,%.o,$(TOOL_SRCS)) $(patsubst %.cpp,%.o,$(TOOL_CXSRCS))

DEFS  := -D_THREAD_SAFE -D__STDC_FORMAT_MACROS

VERSION_MAJOR := 1
VERSION_MINOR := 0
VERSION_PATCH := 1

LIBPSTAT := libpstat.so
LIBPSTAT_SO := libpstat.so.$(VERSION_MAJOR)
LIBPSTAT_LIB := libpstat.so.$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

PSTAT := tools/pstat

PREFIX ?= /
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include/pstat
PKGCONFIGDIR ?= $(PREFIX)/lib/pkgconfig

OS ?= LINUX

# process USE 
ifdef USE_SSL
	# compile with SSL
	DEFS += -D_USE_SSL
	LIB += $(LIB_USE_SSL)
endif

PC_FILE := libpstat.pc

all: libpstat pstat

$(PC_FILE):	$(PC_FILE).in
	@cat $< | \
		sed -e 's~@PREFIX@~$(PREFIX)~g;' | \
		sed -e 's~@INCLUDEDIR@~$(INCLUDEDIR)~g;' | \
		sed -e 's~@VERSION@~$(VERSION)~g; ' | \
		sed -e 's~@LIBS@~$(LIB)~g; ' | \
		sed -e 's~@LIBDIR@~$(LIBDIR)~g; ' | \
	   sed -e 's~@VERSION_MAJOR@~$(VERSION_MAJOR)~g; ' | \
	   sed -e 's~@VERSION_MINOR@~$(VERSION_MINOR)~g; ' | \
	   sed -e 's~@VERSION_PATCH@~$(VERSION_PATCH)~g; '	> $@

libpstat: $(OBJ)
	$(CPP) -shared -Wl,-soname,$(LIBPSTAT_SO) -o $(LIBPSTAT_LIB) $(OBJ) $(LIBINC) $(LIB)
	$(SHELL) -c "if ! test -L $(LIBPSTAT_SO); then /bin/ln -s $(LIBPSTAT_LIB) $(LIBPSTAT_SO); fi"
	$(SHELL) -c "if ! test -L $(LIBPSTAT); then /bin/ln -s $(LIBPSTAT_SO) $(LIBPSTAT); fi"

pstat: $(TOOL_OBJ)
	$(CPP) -o $(PSTAT) $(TOOL_OBJ) $(LIBINC) -L. -lpstat

libpstat-install: libpstat $(PC_FILE)
	mkdir -p $(LIBDIR) $(PKGCONFIGDIR)
	cp -a $(LIBPSTAT) $(LIBPSTAT_SO) $(LIBPSTAT_LIB) $(LIBDIR)
	cp -a $(PC_FILE) $(PKGCONFIGDIR)

pstat-install: pstat
	mkdir -p $(BINDIR)
	cp -a $(PSTAT) $(BINDIR)

libpstat-dev-install: libpstat
	mkdir -p $(INCLUDEDIR) $(INCLUDEDIR)/os
	cp -a $(wildcard os/*.h) $(INCLUDEDIR)/os
	cp -a $(wildcard *.h) $(INCLUDEDIR)

install: libpstat-install pstat-install libpstat-dev-install

%.o : %.c
	$(CPP) -o $@ $(INC) -c $< $(DEFS) -D_$(OS)

%.o : %.cpp
	$(CPP) -o $@ $(INC) -c $< $(DEFS) -D_$(OS)

.PHONY: clean
clean:
	/bin/rm -f $(OBJ) $(TOOL_OBJ) $(LIBPSTAT_LIB) $(LIBPSTAT_SO) $(LIBPSTAT) $(PSTAT) $(PC_FILE)
