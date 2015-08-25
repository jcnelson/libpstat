ROOT_DIR := $(shell dirname "$(realpath $(lastword $(MAKEFILE_LIST)))")
BUILD    ?= $(ROOT_DIR)/build

CC	    ?= cc
CFLAGS ?= -Wall -g -fPIC -std=c99
LIB   := 
INC   := -I. -I$(BUILD) -I$(BUILD)/include/pstat
C_SRCS:= $(wildcard *.c) $(wildcard os/*.c)
TOOL_C_SRCS := $(wildcard tools/*.c)
HEADERS := $(wildcard *.h) $(wildcard os/*.h)
OBJ   := $(patsubst %.c,$(BUILD)/%.o,$(C_SRCS))
TOOL_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(TOOL_C_SRCS))

DEFS  := -D_THREAD_SAFE -D__STDC_FORMAT_MACROS

VERSION_MAJOR := 1
VERSION_MINOR := 0
VERSION_PATCH := 2

LIBPSTAT := libpstat.so
LIBPSTAT_SO := libpstat.so.$(VERSION_MAJOR)
LIBPSTAT_LIB := libpstat.so.$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

PSTAT := tools/pstat

PREFIX ?= /usr/local
DESTDIR ?= /
INCLUDE_PREFIX ?= $(PREFIX)

BINDIR ?= $(DESTDIR)/$(PREFIX)/bin
LIBDIR ?= $(DESTDIR)/$(PREFIX)/lib
INCLUDE_DIR ?= $(DESTDIR)/$(INCLUDE_PREFIX)/include/pstat
PKGCONFIGDIR ?= $(DESTDIR)/$(PREFIX)/lib/pkgconfig

OS ?= LINUX

BUILD_LIBPSTAT_LIB := $(patsubst %,$(BUILD)/$(LIBDIR)/%,$(LIBPSTAT_LIB))
INSTALL_LIBPSTAT_LIB := $(patsubst $(BUILD)/$(LIBDIR)/%,$(LIBDIR)/%,$(BUILD_LIBPSTAT_LIB))

BUILD_LIBPSTAT_SO := $(patsubst %,$(BUILD)/$(LIBDIR)/%,$(LIBPSTAT_SO))
INSTALL_LIBPSTAT_SO := $(patsubst $(BUILD)/$(LIBDIR)/%,$(LIBDIR)/%,$(BUILD_LIBPSTAT_SO))

BUILD_LIBPSTAT := $(patsubst %,$(BUILD)/$(LIBDIR)/%,$(LIBPSTAT))
INSTALL_LIBPSTAT := $(patsubst $(BUILD)/$(LIBDIR)/%,$(LIBDIR)/%,$(BUILD_LIBPSTAT))

# NOTE: don't install os/ headers, since they're different for each OS.
BUILD_HEADERS := $(patsubst %,$(BUILD)/$(INCLUDE_DIR)/%,$(HEADERS))
INSTALL_HEADERS := $(patsubst $(BUILD)/$(INCLUDE_DIR)/%,$(INCLUDE_DIR)/%,$(BUILD_HEADERS))

BUILD_PC_FILE := $(BUILD)/libpstat.pc
INSTALL_PC_FILE := $(PKGCONFIGDIR)/libpstat.pc

BUILD_PSTAT := $(patsubst tools/%,$(BUILD)/$(BINDIR)/%,$(PSTAT))
INSTALL_PSTAT := $(patsubst $(BUILD)/$(BINDIR)/%,$(BINDIR)/%,$(BUILD_PSTAT))

all: tools libs

tools: $(BUILD_PSTAT)

libs: $(BUILD_HEADERS) $(BUILD_PC_FILE) $(BUILD_LIBPSTAT) $(BUILD_LIBPSTAT_SO) $(BUILD_LIBPSTAT_LIB)


$(BUILD_PC_FILE): libpstat.pc.in
	@mkdir -p "$(shell dirname "$@")"
	@cat $< | \
		sed -e 's~@PREFIX@~$(PREFIX)~g;' | \
		sed -e 's~@INCLUDEDIR@~$(INCLUDE_DIR)~g;' | \
		sed -e 's~@VERSION@~$(VERSION)~g; ' | \
		sed -e 's~@LIBS@~$(LIB)~g; ' | \
		sed -e 's~@LIBDIR@~$(LIBDIR)~g; ' | \
	   sed -e 's~@VERSION_MAJOR@~$(VERSION_MAJOR)~g; ' | \
	   sed -e 's~@VERSION_MINOR@~$(VERSION_MINOR)~g; ' | \
	   sed -e 's~@VERSION_PATCH@~$(VERSION_PATCH)~g; '	> $@

$(BUILD_LIBPSTAT_LIB): $(OBJ)
	@mkdir -p "$(shell dirname "$@")"
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(LIBPSTAT_SO) -o "$@" $(OBJ) $(LIBINC) $(LIB)

$(BUILD_LIBPSTAT_SO): $(BUILD_LIBPSTAT_LIB)
	@mkdir -p "$(shell dirname "$@")"
	@ln -sf "$(shell basename "$<")" "$@"

$(BUILD_LIBPSTAT): $(BUILD_LIBPSTAT_SO)
	@mkdir -p "$(shell dirname "$@")"
	@ln -sf "$(shell basename "$<")" "$@"

$(BUILD_PSTAT): $(TOOL_OBJ) $(BUILD_LIBPSTAT)
	@mkdir -p "$(shell dirname "$@")"
	$(CC) $(CFLAGS) -o "$@" $(TOOL_OBJ) $(LIBINC) -L$(BUILD)/$(LIBDIR) -lpstat

$(BUILD)/$(INCLUDE_DIR)/%.h: %.h
	@mkdir -p "$(shell dirname "$@")"
	@cp -a "$<" "$@"


$(INSTALL_LIBPSTAT): $(BUILD_LIBPSTAT)
	@mkdir -p "$(shell dirname "$@")"
	cp -a "$<" "$@"

$(INSTALL_LIBPSTAT_SO): $(BUILD_LIBPSTAT_SO)
	@mkdir -p "$(shell dirname "$@")"
	cp -a "$<" "$@"

$(INSTALL_LIBPSTAT_LIB): $(BUILD_LIBPSTAT_LIB)
	@mkdir -p "$(shell dirname "$@")"
	cp -a "$<" "$@"

$(INCLUDE_DIR)/%.h: $(BUILD)/$(INCLUDE_DIR)/%.h
	@mkdir -p "$(shell dirname "$@")"
	cp -a "$<" "$@"

$(INSTALL_PC_FILE): $(BUILD_PC_FILE)
	@mkdir -p "$(shell dirname "$@")"
	cp -a "$<" "$@"

$(INSTALL_PSTAT): $(BUILD_PSTAT)
	@mkdir -p "$(shell dirname "$@")"
	cp -a "$<" "$@"


libs-install: $(INSTALL_LIBPSTAT) $(INSTALL_LIBPSTAT_SO) $(INSTALL_LIBPSTAT_LIB)

tools-install: $(INSTALL_PSTAT)

headers-install: $(INSTALL_HEADERS)

install: libs-install tools-install headers-install

$(BUILD)/%.o : %.c
	@mkdir -p "$(shell dirname "$@")"
	$(CC) $(CFLAGS) -o "$@" $(INC) -c "$<" $(DEFS) -D_$(OS)

.PHONY: clean
clean:
	rm -f $(BUILD_HEADERS) $(BUILD_LIBPSTAT_LIB) $(BUILD_LIBPSTAT_SO) $(BUILD_LIBPSTAT) $(BUILD_PC_FILE) $(BUILD_HEADERS) $(BUILD_PSTAT) $(OBJ)

.PHONY: uninstall 
uninstall:
	rm -f $(INSTALL_HEADERS) $(INSTALL_LIBPSTAT_LIB) $(INSTALL_LIBPSTAT_SO) $(INSTALL_LIBPSTAT) $(INSTALL_PC_FILE) $(INSTALL_PSTAT)

print-%: ; @echo $*=$($*)

