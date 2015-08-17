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

#include "pstat.h"

void usage( char const* prog_name ) {
   fprintf(stderr, "Usage: %s PID [PID...]\n", prog_name);
   exit(1);
}

void print_pstat( struct pstat* ps ) {
   
   struct stat bin_stat;
   char path[PATH_MAX+1];
   
   printf("PID: %d\n", pstat_get_pid( ps ) );
   printf("  running: %s\n", pstat_is_running( ps ) ? "true" : "false" );
   
   if( !pstat_is_running( ps ) ) {
      return;
   }
   
   if( !pstat_is_deleted( ps ) ) {
      
      pstat_get_stat( ps, &bin_stat );
      pstat_get_path( ps, path );
      
      printf("  binary:  %s\n", path );
      printf("  deleted: %s\n", pstat_is_deleted( ps ) ? "true": "false" );
      printf("  inode:   %ld\n", bin_stat.st_ino );
      printf("  size:    %jd\n", bin_stat.st_size );
      printf("  modtime: %ld\n", bin_stat.st_mtime );
      
   }
   else {
      printf("  deleted: true\n");
   }
}

int main( int argc, char** argv ) {
   
   pid_t pid = 0;
   int rc = 0;
   struct pstat* ps = pstat_new();
   
   if( ps == NULL ) {
      exit(1);
   }
   
   if( argc <= 1 ) {
      usage( argv[0] );
   }
   
   for( int i = 1; i < argc; i++ ) {
      
      pid = 0;
      
      rc = sscanf( argv[i], "%d", &pid );
      if( rc != 1 ) {
         
         usage( argv[0] );
      }
      
      rc = pstat( pid, ps, 0 );
      
      if( rc != 0 ) {
         fprintf( stderr, "pstat(%d) rc = %d\n", pid, rc );
         exit(1);
      }
      
      print_pstat( ps );
   }
   
   pstat_free( ps );
   
   return 0;
}
