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
#ifndef _PSTAT_OS_LINUX_H_
#define _PSTAT_OS_LINUX_H_

#include "common.h"
#include <unistd.h>
#include <fcntl.h>

// this is private
struct pstat {
   pid_t pid;
   bool running;                                        // whether or not the process is running.  If this is false, then the fields beneath will not be initialized
   
   char path[PATH_MAX+1];                               // path to the process.  set to NULL if not initialized 
   bool deleted;                                        // whether or not the process binary is deleted (in which case, proc_stat will be uninitialized)
   
   struct stat bin_stat;                                // stat(2) result from the binary path
   
   uint64_t starttime;                                  // time (in jiffies) when this process started
};

uint64_t pstat_os_supported_features();
int pstat_os( pid_t pid, struct pstat* ps, int flags );

#endif 