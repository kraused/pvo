
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

#ifndef PVO_DECLS_H_INCLUDED
#define PVO_DECLS_H_INCLUDED 1

/// @file pvo_decls.h
/// Collection of function declarations

#include "pvo_mpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Initialize the library.
/// @param[in]  comm    world communicator
///                     for the pvo library.
///                     The library will
///                     duplicate this 
///                     communicator
/// @returns    0 if everything wents
///             fine. Otherwise -1 is returned. 
int pvo_init( MPI_Comm comm );

/// Finalize the library
int pvo_quit();

/// Helper routine for creating islands in such a way that processes
/// on the same physical node are in the same group. The returned
/// value identifies the physical node uniquely. 
/// Unfortunately it is hard to make restrictions on the size of
/// the returned value. In general, 16 bit might not be sufficient
/// for uniquely identifying the nodes (e.g., Jugene with 73,728 nodes
/// as of April 16 2010). 
int pvo_physical_node_uid();

#ifdef __cplusplus
}
#endif

#endif

