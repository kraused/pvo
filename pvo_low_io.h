
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_LOW_IO_H_INCLUDED
#define PVO_LOW_IO_H_INCLUDED 1

/// @file pvo_low_io.h
/// Low level I/O functionality

/// This file provides parallel I/O functionality
/// implemented on top of different transport mechanisms.
/// Each file has an associated unique shared
/// file pointer. All functions are collective.

#include "pvo_MPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Identifier for the transport layers
typedef enum {
    /// Serial (i.e., non-concurrent)
    /// access to a file. This is provided
    /// to allow for reusing the implemented
    /// methods also for writing files on a
    /// single process
    PVO_LOW_IO_SINGLE = 0,
    /// MPI-I/O based layer. Assumes
    /// that an MPI-2 compliant library
    /// is available
    PVO_LOW_IO_MPI    = 1,
    /// Implements concurrent file output
    /// based on the Posix interface. Should
    /// be used if either no MPI-2 library
    /// is available or if the latter doesn't
    /// work well (e.g., on some NFS installations)
    PVO_LOW_IO_POSIX  = 2
} pvo_low_transport_layer_t;


/// Internal handler for a file. The actual type
/// depends on the transport mechanism.
typedef void* pvo_low_io_internal_file_handle_t;

/// File handle
typedef struct pvo_low_io_file_handle
{
    /// The transport layer
    pvo_low_transport_layer_t           layer;

    /// The internal handle maintained by the 
    /// implementation
    pvo_low_io_internal_file_handle_t   handle;

    /// Open the file. This function allocates the
    /// internal handle instance
    /// @param[in]  self    the this pointer
    /// @param[in]  name    the filename
    /// @param[in]  comm    the MPI communicator. If MPI I/O is
    ///                     used this argument directly translates
    ///                     into the 
    /// @returns    0 if everything wents fine. -1 otherwise
    int (*open)( struct pvo_low_io_file_handle* self, 
                 const char* name, 
                 MPI_Comm    comm );

    /// Close the file. This function frees the internal
    /// handle instance
    /// @param[in]  self    teh this pointer
    /// @returns    0 if everything wents fine. -1 otherwise
    int (*close)( struct pvo_low_io_file_handle* self );

    /// Write data on a single process. The rank of the
    /// writer cannot be controlled, hence all processes should
    /// provide the same input buf. This function is intended
    /// to write metadata to a file while keeping the shared
    /// file pointer up to date.
    /// @param[in]  self    the this pointer
    /// @param[in]  buf     buffer to write
    /// @param[in]  count   number of elements in buffer
    /// @returns    0 if everything wents fine. -1 otherwise
    int (*write_single)( struct pvo_low_io_file_handle* self,
                         void* buf,
                         int   count );

    /// Write data with order implied by the rank of the processes
    /// in the communicator.
    /// @param[in]  self    the this pointer
    /// @param[in]  buf     buffer to write
    /// @param[in]  count   number of elements in buffer
    /// @returns    0 if everything wents fine. -1 otherwise
    int (*write_ordered)( struct pvo_low_io_file_handle* self,
                          void* buf,
                          int   count );

} * pvo_low_io_file_handle_t;


/// Create a file handle. This operation only creates an instance
/// of pvo_low_io_file_handle_t, it does not actually open a file.
/// This can be done using the open() function.
/// @param[in]  layer   the transport layer
/// @param[out] fh      the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_low_io_file_handle_create( pvo_low_transport_layer_t layer,
                                   pvo_low_io_file_handle_t* fh );

/// Destroy a file handle. The operation assumes that the file has
/// been closed previously using the close() function
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_low_io_file_handle_delete( pvo_low_io_file_handle_t fh );

#ifdef __cplusplus
}
#endif

#endif

