
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_MPI_H_INCLUDED
#define PVO_MPI_H_INCLUDED 1

/// @file pvo_MPI.h
/// PVO interface to MPI, the message passing standard

#ifdef HAVE_MPI_H
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

