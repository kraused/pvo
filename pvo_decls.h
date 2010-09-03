
/// vim: tabstop=4:expandtab:hlsearch

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

