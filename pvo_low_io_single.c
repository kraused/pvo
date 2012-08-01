
/// vim: tabstop=4:expandtab:hlsearch

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

#include "pvo_low_io_single.h"
#include "pvo_report.h"
#include "pvo_memory.h"

/// We are using the standard I/O provided by stdio.h
/// This should be fine since this transport layer is
/// anyway designed for metadata output, i.e., for small
/// data volumes.
#include <stdio.h>

int pvo_low_io_single_open( pvo_low_io_file_handle_t self,    
                            const char* name,   
                            MPI_Comm    comm )
{
    if( NULL == ( self->handle = fopen( name, "w" ))) {
        PVO_WARN( "Opening file \"%s\" failed.", name );
        return -1;
    }

    return 0;
}


int pvo_low_io_single_close( pvo_low_io_file_handle_t self )
{
    if( EOF == fclose( (FILE* )self->handle ))
        return -1;

    return 0;
}


/// A thin wrapper around fwrite which performs checks and
/// makes sure we return the correct values
static int pvo_low_io_single_write( FILE* fh, void* buf, size_t count )
{
    if( NULL == fh ) {
        PVO_WARN( "Attempt to write to a file which is not open." );
        return -1;
    }

    if( count != fwrite( buf, 1, count, fh ))
        return -1;

    return 0;
}


int pvo_low_io_single_write_single( pvo_low_io_file_handle_t self,
                                    void*  buf,
                                    size_t count )
{
    if( NULL == self ) {
        PVO_WARN( "Invalid input: NULL == self." );
        return -1;
    }

    return pvo_low_io_single_write( (FILE* )self->handle, buf, count );
}


int pvo_low_io_single_write_ordered( pvo_low_io_file_handle_t self,
                                     void*  buf,
                                     size_t count )
{
    if( NULL == self ) {
        PVO_WARN( "Invalid input: NULL == self." );
        return -1;
    }

    /// An ordered write with a single process is the same as
    /// a normal write
    return pvo_low_io_single_write( (FILE* )self->handle, buf, count );
}

