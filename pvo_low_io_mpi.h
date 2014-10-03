
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

#ifndef PVO_LOW_IO_MPI_H_INCLUDED
#define PVO_LOW_IO_MPI_H_INCLUDED 1

/// @file pvo_low_io_mpi.h
/// Low level I/O functionality based on
/// the standard MPI I/O specifications.

#include "pvo_mpi.h"
#include "pvo_low_io.h"

#ifdef __cplusplus
extern "C" {
#endif


/// Open a file. See the documentation in pvo_low_io.h
/// for more information.
int pvo_low_io_mpi_open( pvo_low_io_file_handle_t self,
                         const char* name,
                         MPI_Comm    comm );

/// Close a file. See the documentation in pvo_low_io.h
/// for more information.
int pvo_low_io_mpi_close( pvo_low_io_file_handle_t self );

/// Write from a single process. See the documentation in pvo_low_io.h
/// for more information.
int pvo_low_io_mpi_write_single( pvo_low_io_file_handle_t self,
                                 void*  buf,
                                 size_t count );

/// Write ordered. See the documentation in pvo_low_io.h
/// for more information.
int pvo_low_io_mpi_write_ordered( pvo_low_io_file_handle_t self,
                                  void*  buf,
                                  size_t count );

#ifdef __cplusplus
}
#endif

#endif

