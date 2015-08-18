/*
   libpstat: a library for getting information about running processes
   Copyright (C) 2014  Jude Nelson

   This program is dual-licensed: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License version 3 or later as 
   published by the Free Software Foundation. For the terms of this 
   license, see LICENSE.LGPLv3+ or <http://www.gnu.org/licenses/>.

   You are free to use this program under the terms of the GNU Lesser General
   Public License, but WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU Lesser General Public License for more details.

   Alternatively, you are free to use this program under the terms of the 
   Internet Software Consortium License, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   For the terms of this license, see LICENSE.ISC or 
   <http://www.isc.org/downloads/software-support-policy/isc-license/>.
*/


#ifndef _LIBPSTAT_H_
#define _LIBPSTAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// set of feature flags the implementation may support
#define LIBPSTAT_RUNNING        0x1     // implementation can tell that a process is running 
#define LIBPSTAT_BINARY         0x2     // implementation can tell the path and status a process's exec'ed binary image
#define LIBPSTAT_STARTTIME      0x4     // implementation can tell when the process started

// process status structure
struct pstat;

// features 
uint64_t pstat_supported_features();

// memory management
struct pstat* pstat_new();
void pstat_free( struct pstat* ps );

// stat a running process.
// return 0 on success, which is qualified to mean "We could fill in at least one pstat field, besides the PID"
// return -errno on error (specific to the OS)
int pstat( pid_t pid, struct pstat* ps, int flags );

// query a pstat structure...
pid_t pstat_get_pid( struct pstat* ps );

// if LIBPSTAT_RUNNING is supported
bool pstat_is_running( struct pstat* ps );

// if LIBPSTAT_BINARY is supported
bool pstat_is_deleted( struct pstat* ps );
int pstat_get_path( struct pstat* ps, char* pathbuf );
int pstat_get_stat( struct pstat* ps, struct stat* sb );

// if LIBPSTAT_STARTTIME is supported
uint64_t pstat_get_starttime( struct pstat* ps );

#endif 