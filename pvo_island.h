
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

#ifndef PVO_ISLAND_H_INCLUDED
#define PVO_ISLAND_H_INCLUDED 1

/// @file pvo_island.h
/// Provides sub-sets functionality

#include "pvo_mpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/// pvo_island: subgroup/communicator for I/O
/// By definition, all processes in the same island
/// write to the same file. The PVO_COMM_WORLD is split
/// into disjoint groups to enhance scalability.
typedef struct pvo_island {

/// No need to store the "archipelago" communicator. We assume
/// this is PVO_COMM_WORLD    

    /// Each island should know how many other island 
    /// are in the archipelago. We store this directly
    /// in pvo_island since introducing a new structure
    /// just for this purpose seems like an overshoot.
    int             nislands;

    /// The island number
    int             no;

    /// The island communicator
    MPI_Comm        comm;

    /// The size of the island
    int             size;

    /// Rank of the process in the island. Rank zero has
    /// the "master" role and is taking care of coordination
    int             rank;

} pvo_island_t;

/// General remark:
/// In principle one could think of adding a further level of
/// hierarchy: We could define "sets" of processes and for these
/// processes select (for example the rank 0 process) as the "writer".
/// All other processes would then pass their data via message
/// passing to the writer who is the only one doing the real I/O.
/// This is different from the island concept which assumes that
/// all the processes in an island participate in collective
/// I/O operations

/// Create a new island by splitting PVO_COMM_WORLD into 
/// islands. All ranks providing the same value for color are
/// placed on the same island. Note that passing a negative valuee
/// as color (which might coincide with MPI_UNDEFINED by chance) 
/// is invalid since we assume that each process is part of exactly one 
/// island
/// @param[in]      color   the color used to decide how the islands
///                         are ranked
/// @param[inout]   island  on exit: initialized structure
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_create_island( int color, pvo_island_t* island );

/// Delete the island. Note that the handle itself is not
/// deleted
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_delete_island( pvo_island_t* );

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

