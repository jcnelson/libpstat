/*
   libpstat: a library for getting information about running processes
   Copyright (C) 2014  Jude Nelson

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libpstat.h"

#ifdef _LINUX
#include "os/linux.h"
#endif

#ifdef _UNKNOWN
#error "Undefined OS target"
#endif

// insert more OSs here

// get the status of a running processes
// return 0 on success 
// return negative on error
int pstat( pid_t pid, struct pstat* ps, int flags ) {
   return pstat_os( pid, ps, flags );
}
