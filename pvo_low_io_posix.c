
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

#include "pvo_low_io_posix.h"
#include "pvo_report.h"
#include "pvo_memory.h"

#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#if 1   /// FIXME Mac Os X
#define O_LARGEFILE 0
#endif

/// pvo_low_io_posix_internal_file_handle_t: The internal
/// file handle used by the low level implementation
typedef struct {
    MPI_Comm    comm;   /// The communicator used
                        /// for opening the file.
    int         fd;     /// The file descriptor
    long        fptr;   /// The shared file pointer
                        /// maintained seperatly from
                        /// the individual file pointers
} pvo_low_io_posix_internal_file_handle_t;


int pvo_low_io_posix_open( pvo_low_io_file_handle_t self,    
                            const char* name,   
                            MPI_Comm    comm )
{
    pvo_low_io_posix_internal_file_handle_t* h;

    self->handle = h = pvo_malloc( sizeof(pvo_low_io_posix_internal_file_handle_t) );

    if( MPI_Comm_dup ( comm, &h->comm )) {
        PVO_WARN( "Opening file \"%s\" failed.", name );
        return -1;
    }
    
    h->fptr = 0;
    h->fd   = open( name, O_CREAT | O_WRONLY | O_FSYNC,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
    /// Do not use O_TRUNC to open the file as this messes up 
    /// parallel writes to the file.

    if( -1 == h->fd ) {
        PVO_WARN( "Opening file \"%s\" failed (%s).", name, strerror(errno) );
        return -1;
    }

    return 0;
}


int pvo_low_io_posix_close( pvo_low_io_file_handle_t self )
{
    pvo_low_io_posix_internal_file_handle_t* h = self->handle;

    if( -1 == close( h->fd )) {
        PVO_WARN( "close() failed." );
        return -1;
    }
    if( MPI_Comm_free( &h->comm )) {
        PVO_WARN( "MPI_Comm_free failed." );
        return -1;
    }

    pvo_free( self->handle );

    return 0;
}


/// Write count bytes at position pos to the file fd.
static int pvo_low_io_posix_write( int fd, long pos, void* buf, int count )
{
    if( (off_t )-1 == lseek( fd, pos, SEEK_SET ))
        return -1;

    if( -1 == write( fd, buf, count ))
        return -1;
    
    return 0;
}


int pvo_low_io_posix_write_single( pvo_low_io_file_handle_t self,
                                    void* buf,
                                    int   count )
{
    pvo_low_io_posix_internal_file_handle_t* h = self->handle;
    int myrank, err, t[2];

    MPI_Comm_rank( h->comm, &myrank );

    if( 0 == myrank )
        err = pvo_low_io_posix_write( h->fd, h->fptr, buf, count );

    t[0] = err;
    t[1] = count;   // Make sure the shared file pointer is consistent
                    // even if the user gives a wrong input
    MPI_Bcast( t, 2, MPI_INT, 0, h->comm );
    err   = t[0];
    count = t[1];

    h->fptr += count;    // Update shared file pointer

    return err;
}


int pvo_low_io_posix_write_ordered( pvo_low_io_file_handle_t self,
                                     void* buf,
                                     int   count )
{
    pvo_low_io_posix_internal_file_handle_t* h = self->handle;
    int  myrank, err, offset, t[2];

    MPI_Comm_rank( h->comm, &myrank );

    /// Compute the offset of the process
    MPI_Exscan( &count, &offset, 1, MPI_INT, MPI_SUM, h->comm );
    if( 0 == myrank )
        offset = 0;

    err = pvo_low_io_posix_write( h->fd, h->fptr+offset, buf, count );    

    t[0] = err;
    t[1] = count;
    MPI_Allreduce( MPI_IN_PLACE, t, 2, MPI_INT, MPI_SUM, h->comm );
    err   = t[0];
    count = t[1];

    h->fptr += count;    // Update shared file pointer

    return err;
}

