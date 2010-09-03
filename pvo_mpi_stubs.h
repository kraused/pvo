
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_MPI_STUBS_H_INCLUDED
#define PVO_MPI_STUBS_H_INCLUDED 1

/// @file pvo_mpi_stubs.h
/// Stub versions of MPI routines

#include <string.h>
#include <stdlib.h>

#include "pvo_report.h"

#ifdef __cplusplus
extern "C" {
#endif

/// MPI Communicator type
typedef int MPI_Comm;

/// MPI Datatype
typedef int MPI_Datatype;

/// MPI Operation
typedef int MPI_Op;

/// The types
#define MPI_INT         (0)
#define MPI_LONG        (1)
#define MPI_FLOAT       (2)
#define MPI_DOUBLE      (3)
#define MPI_BYTE        (4)
#define MPI_CHAR        (5)

/// The operations
#define MPI_SUM         (0)

/// Predefined communicators
#define MPI_COMM_WORLD  (0)
#define MPI_COMM_NULL   (1)

/// In place operations
#define MPI_IN_PLACE    NULL

/// Query the size of a communicator
static inline int MPI_Comm_size( MPI_Comm comm, int* size )
{
    *size = 1;
    return 0;
}

/// Query the rank of the process in a communicator
static inline int MPI_Comm_rank( MPI_Comm comm, int* rank )
{
    *rank = 0;
    return 0;
}

/// Initialize MPI
static inline int MPI_Init( int* argc, char*** argv )
{
    return 0;
}

/// Finalize MPI
static inline int MPI_Finalize()
{
    return 0;
}

/// Nothing to do
static inline int MPI_Bcast( void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
{
    return 0;
}

/// Nothing to do: The received value on this process is undefined (as the standard
/// allows it)
static inline int MPI_Exscan( void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
    return 0;
}

/// Get size of type
static inline int MPI_Type_size( MPI_Datatype datatype, int* size )
{
    switch( datatype )
    {
    case MPI_INT:
        *size = sizeof(int);
        break;
    case MPI_LONG:
        *size = sizeof(long);
        break;
    case MPI_FLOAT:
        *size = sizeof(float);
        break;
    case MPI_DOUBLE:
        *size = sizeof(double);
        break;
    case MPI_BYTE:
    case MPI_CHAR:
        *size = sizeof(char);
        break;
    default:
        PVO_ERROR( "Invalid datatype." );
    }

    return 0;
}

/// Allreduce operation
static inline int MPI_Allreduce( void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
    int size;

    if( MPI_IN_PLACE != sendbuf )
    {
        MPI_Type_size( datatype, &size );
        memcpy( recvbuf, sendbuf, count*size );
    }

    return 0;
}

/// Abort routine
static inline int MPI_Abort( MPI_Comm comm, int errorcode )
{
    exit( errorcode );
    return 0;
}

/// Duplicate the communicator
static inline int MPI_Comm_dup( MPI_Comm comm, MPI_Comm *comm_out )
{
    *comm_out = comm;
    return 0;
}

/// Free a communicator
static inline int MPI_Comm_free( MPI_Comm* comm )
{
    *comm = MPI_COMM_NULL;
    return 0;
}

/// Nothing to split
static inline int MPI_Comm_split( MPI_Comm comm, int color, int key, MPI_Comm* comm_out )
{
    *comm_out = comm;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif

