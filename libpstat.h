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

#ifdef _USE_SHA256

#include <openssl/sha.h>
#else

#define SHA256_DIGEST_LENGTH 32         // 256 / 8
#endif

// flags for pstat()
#define PSTAT_HASH      0x1

// process status structure
struct pstat {
   pid_t pid;
   bool running;                                        // whether or not the process is running.  If this is false, then the fields beneath will not be initialized
   
   char path[PATH_MAX+1];                               // path to the process.  set to NULL if not initialized 
   bool deleted;                                        // whether or not the process binary is deleted (in which case, proc_stat will be uninitialized)
   
   struct stat bin_stat;                                // stat(2) result from the binary path
   
   unsigned char sha256[SHA256_DIGEST_LENGTH];          // sha256 of the process binary.  Ignored if the PSTAT_HASH bit is not set in flags
};

extern "C" {

// stat a running process.
// return 0 on success, which is qualified to mean "We could fill in at least one pstat field, besides the PID"
// return -errno on error (specific to the OS)
int pstat( pid_t pid, struct pstat* ps, int flags );

// Get the SHA256 of a process, as identified by the filled-in pstat structure.
// ps should be populated by a prior call to pstat
// return 0 on success
// return -ENOENT if the process binary was deleted
// return -ECHILD if the process is not running
// return -errno if we fail to resolve or open the binary for reading.
int pstat_sha256( struct pstat* ps );

}

#endif 