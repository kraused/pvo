
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

#include "pvo_config.h"
#include "pvo_low_io.h"
#include "pvo_memory.h"
#include "pvo_report.h"

#include "pvo_low_io_single.h"
#include "pvo_low_io_mpi.h"
#include "pvo_low_io_posix.h"

int pvo_low_io_file_handle_create( pvo_low_transport_layer_t layer,
                                   pvo_low_io_file_handle_t* fh )
{
    int err = 0;

    *fh = pvo_malloc( sizeof(struct pvo_low_io_file_handle) );
   
    (*fh)->layer  = layer;
    (*fh)->handle = NULL;

    switch( layer ) {
    case PVO_LOW_IO_SINGLE:
        (*fh)->open          = pvo_low_io_single_open;
        (*fh)->close         = pvo_low_io_single_close;
        (*fh)->write_single  = pvo_low_io_single_write_single;
        (*fh)->write_ordered = pvo_low_io_single_write_ordered;
        break;
#if 1 == PVO_HAVE_MPI
    case PVO_LOW_IO_MPI:
        (*fh)->open          = pvo_low_io_mpi_open;
        (*fh)->close         = pvo_low_io_mpi_close;
        (*fh)->write_single  = pvo_low_io_mpi_write_single;
        (*fh)->write_ordered = pvo_low_io_mpi_write_ordered;
        break;
#endif
    case PVO_LOW_IO_POSIX:
        (*fh)->open          = pvo_low_io_posix_open;
        (*fh)->close         = pvo_low_io_posix_close;
        (*fh)->write_single  = pvo_low_io_posix_write_single;
        (*fh)->write_ordered = pvo_low_io_posix_write_ordered;
        break;
    default:
        PVO_ERROR( "Invalid transport layer." );
        goto fn_fail;
    }


fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}


int pvo_low_io_file_handle_delete( pvo_low_io_file_handle_t fh )
{
    pvo_free( fh ); /// pvo->handle already freed by close

    return 0;
}

