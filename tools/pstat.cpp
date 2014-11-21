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

#include "pstat.h"

void usage( char const* prog_name ) {
   fprintf(stderr, "Usage: %s PID [PID...]", prog_name);
   exit(1);
}

char* sha256_printable( unsigned char const* sha256 ) {
   char* ret = (char*)calloc( sizeof(char) * (2 * SHA256_DIGEST_LENGTH + 1), 1 );
   char buf[3];
   for( int i = 0; i < SHA256_DIGEST_LENGTH; i++ ) {
      sprintf(buf, "%02x", sha256[i] );
      ret[2*i] = buf[0];
      ret[2*i + 1] = buf[1];
   }
   
   return ret;
}

void print_pstat( struct pstat* ps ) {
   
   printf("PID: %d\n", ps->pid );
   printf("  running: %s\n", ps->running ? "true" : "false" );
   
   if( !ps->running ) {
      return;
   }
   
   if( !ps->deleted ) {
      
      char* proc_sha256 = sha256_printable( ps->sha256 );
      if( proc_sha256 == NULL ) {
         // OOM
         exit(2);
      }
      
      printf("  binary:  %s\n", ps->path );
      printf("  deleted: %s\n", ps->deleted ? "true": "false" );
      printf("  inode:   %ld\n", ps->bin_stat.st_ino );
      printf("  size:    %jd\n", ps->bin_stat.st_size );
      printf("  modtime: %ld.%ld\n", ps->bin_stat.st_mtim.tv_sec, ps->bin_stat.st_mtim.tv_nsec );
      printf("  SHA256:  %s\n", proc_sha256 );
      
      free( proc_sha256 );
   }
   else {
      printf("  deleted: true\n");
   }
}

int main( int argc, char** argv ) {
   
   pid_t pid = 0;
   int rc = 0;
   struct pstat ps;
   
   if( argc <= 1 ) {
      usage( argv[0] );
   }
   
   for( int i = 1; i < argc; i++ ) {
      
      pid = 0;
      
      rc = sscanf( argv[i], "%d", &pid );
      if( rc != 1 ) {
         
         usage( argv[0] );
      }
      
      memset( &ps, 0, sizeof(struct pstat) );
      
      rc = pstat( pid, &ps, PSTAT_HASH );
      
      if( rc != 0 ) {
         perror("pstat");
         exit(1);
      }
      
      print_pstat( &ps );
   }
   
   return 0;
}
