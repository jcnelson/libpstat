AC_PREREQ(2.61)
AC_INIT([libpstat], 1.0.2, [aaa@bbb.com])
AC_MSG_CHECKING([for supported operating system])
AC_CANONICAL_HOST
case "$host_os" in
*linux*)
	opsys=linux
	AC_DEFINE([LINUX],1,[Define for the Linux operating system.])
	;;
*)
	AC_MSG_RESULT([NO!])
	AC_MSG_ERROR([operating system $host_os is not supported by libpstat])
	;;
esac

#AC_MSG_RESULT($opsys)
AM_CONDITIONAL([LINUX], [test x$opsys = xlinux])
AC_CONFIG_SRCDIR([src/libpstat.c])
AM_INIT_AUTOMAKE([-Wall
		subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
AC_PROG_INSTALL
AC_CONFIG_HEADERS([config.h])
AM_PROG_AR([/usr/bin/ar])
LT_INIT()
#AM_CFLAGS="-Wall -g -std=c99"# -I\$(top_srcdir)/ -I\$(top_srcdir)/src/os"
#AC_CHECK_HEADER([stdio.h],
#                [AC_DEFINE([HAVE_STDIO_H])],
#                [AC_MSG_ERROR([Sorry, You do not have basic header files.])])
AC_CONFIG_FILES([Makefile
		src/Makefile
		src/os/Makefile
		src/tools/Makefile])

AC_OUTPUT
