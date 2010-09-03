
/// vim: tabstop=4:expandtab:hlsearch

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
                                 void* buf,
                                 int   count );

/// Write ordered. See the documentation in pvo_low_io.h
/// for more information.
int pvo_low_io_mpi_write_ordered( pvo_low_io_file_handle_t self,
                                  void* buf,
                                  int   count );

#ifdef __cplusplus
}
#endif

#endif

