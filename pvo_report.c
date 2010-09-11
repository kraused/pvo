
/// vim: tabstop=4:expandtab:hlsearch:

/* Copyright 2010 University of Lugano. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of the University of Lugano.
 */

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

