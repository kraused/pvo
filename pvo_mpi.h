
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

#ifndef PVO_MPI_H_INCLUDED
#define PVO_MPI_H_INCLUDED 1

/// @file pvo_mpi.h
/// PVO interface to MPI, the message passing standard

#include "pvo_config.h"

#ifdef PVO_HAVE_MPI_H
#include <mpi.h>
#else
#include "pvo_mpi_stubs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// PVO World communicator
extern MPI_Comm     PVO_COMM_WORLD;

/// Return the size of the communicator
/// PVO_COMM_WORLD
static inline int pvo_world_size() {
    int size;
    MPI_Comm_size( PVO_COMM_WORLD, &size );
    return size; 
}

/// Return the rank of the calling process in the
/// communicator PVO_COMM_WORLD
static inline int pvo_world_rank() {
    int rank;
    MPI_Comm_rank( PVO_COMM_WORLD, &rank );
    return rank;
}

#ifdef __cplusplus
}
#endif

#endif

