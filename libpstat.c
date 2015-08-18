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

#include "libpstat.h"

#ifdef _LINUX
#include "os/linux.h"
#endif

#ifdef _UNKNOWN
#error "Undefined OS target"
#endif

// insert more OSs here

// allocate a new pstat 
struct pstat* pstat_new() {
   return (struct pstat*)calloc( sizeof(struct pstat), 1 );
}

// free a pstat 
void pstat_free( struct pstat* ps ) {
   free( ps );
}

// get the status of a running processes
// return 0 on success 
// return negative on error
int pstat( pid_t pid, struct pstat* ps, int flags ) {
   return pstat_os( pid, ps, flags );
}

pid_t pstat_get_pid( struct pstat* ps ) {
   return ps->pid;
}

bool pstat_is_running( struct pstat* ps ) {
   return ps->running;
}

bool pstat_is_deleted( struct pstat* ps ) {
   return ps->deleted;
}

int pstat_get_path( struct pstat* ps, char* pathbuf ) {
   
   if( pathbuf != NULL ) {
      strcpy( pathbuf, ps->path );
   }
   
   return (int)strlen( ps->path );
}
   
int pstat_get_stat( struct pstat* ps, struct stat* sb ) {
   
   if( !ps->running ) {
      return -ENODATA;
   }
   
   memcpy( sb, &ps->bin_stat, sizeof(struct stat) );
   
   return 0;
}

uint64_t pstat_get_starttime( struct pstat* ps ) {
   
   return ps->starttime;
}

   