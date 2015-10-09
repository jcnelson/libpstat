ACLOCAL_AMFLAGS = -I m4 $(ACLOCAL_FLAGS)
LIBPSTAT_MAJOR=1
LIBPSTAT_MINOR=0

lib_LTLIBRARIES = libpstat.la
instdir=/usr/local
libpstat_la_SOURCES = os/linux.c os/common.c libpstat.c
AM_CFLAGS=-Wall -g -std=c99 -I$(top_srcdir)/src/os/
libpstat_la_CFLAGS=-shared -fPIC $(AM_CFLAGS)

if LINUX
AM_CFLAGS+=-D_LINUX
libpstat_la_CFLAGS+=-D_LINUX
endif
libpstat_la_LDFLAGS = -no-undefined -version-info $(LIBPSTAT_MAJOR):$(LIBPSTAT_MINOR)
EXTRA_DIST=libpstat.pc.in
nobase_include_HEADERS = libpstat.h
