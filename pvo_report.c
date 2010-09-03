
/// vim: tabstop=4:expandtab:hlsearch:

#include "pvo_report.h"
#include "pvo_mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/// Implementation inspired by git 1.6.6

static void report( const char* prefix, const char* filename, 
                    const char* funct, long line,
                    const char* err, va_list vl ) {
    char msg[4096];
    
    vsnprintf( msg, sizeof(msg), err, vl );
    fprintf  ( stderr, " [%s in %s:%ld] %s%s\n", filename, 
               funct, line, prefix, msg );
}


void pvo_error( const char* filename, const char* funct, long line,
                const char* err, ... ) {
    va_list vl;

    va_start( vl, err );
    report( "error: ", filename, funct, line, err, vl );
    va_end  ( vl );    
}

void pvo_die  ( const char* filename, const char* funct, long line,
                const char* err, ... ) {
    va_list vl;

    va_start( vl, err );
    report( "error: ", filename, funct, line, err, vl );
    va_end  ( vl );    

    MPI_Abort( MPI_COMM_WORLD, 128 );
}

void pvo_warn ( const char* filename, const char* funct, long line,
                const char* err, ... ) {
    va_list vl;

    va_start( vl, err );
    report( "warn: ", filename, funct, line, err, vl );
    va_end  ( vl );    
}

void pvo_info ( const char* filename, const char* funct, long line,
                const char* err, ... ) {
    va_list vl;

    va_start( vl, err );
    report( "info: ", filename, funct, line, err, vl );
    va_end  ( vl );    
}

