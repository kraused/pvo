
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_low_io_MPI.h"
#include "pvo_report.h"
#include "pvo_memory.h"

/// pvo_low_io_MPI_internal_file_handle_t: The internal
/// file handle used by the low level implementation
typedef struct {
    MPI_Comm    comm;   /// The communicator used
                        /// for opening the file. This
                        /// is necessary since MPI-2.2
                        /// doesn't provide
                        /// MPI_File_get_comm()
    MPI_File    file;   /// The MPI file handle
} pvo_low_io_MPI_internal_file_handle_t;


int pvo_low_io_MPI_open( pvo_low_io_file_handle_t self,    
                         const char* name,   
                         MPI_Comm    comm )
{
    pvo_low_io_MPI_internal_file_handle_t* h;

    self->handle = h = pvo_malloc( sizeof(pvo_low_io_MPI_internal_file_handle_t) );

    if( MPI_Comm_dup ( comm, &h->comm )) {
        PVO_WARN( "Opening file \"%s\" failed.", name );
        return -1;
    }
    if( MPI_File_open( comm, (char* )name, MPI_MODE_WRONLY | MPI_MODE_CREATE, 
                       MPI_INFO_NULL, &h->file )) {
        PVO_WARN( "Opening file \"%s\" failed.", name );
        return -1;
    }

    return 0;
}


int pvo_low_io_MPI_close( pvo_low_io_file_handle_t self )
{
    pvo_low_io_MPI_internal_file_handle_t* h = self->handle;

    if( MPI_File_close( &h->file )) {
        PVO_WARN( "MPI_File_close failed." );
        return -1;
    }
    if( MPI_Comm_free( &h->comm )) {
        PVO_WARN( "MPI_Comm_free failed." );
        return -1;
    }

    pvo_free( self->handle );

    return 0;
}


int pvo_low_io_MPI_write_single( pvo_low_io_file_handle_t self,
                                 void* buf,
                                 int   count )
{
    pvo_low_io_MPI_internal_file_handle_t* h = self->handle;
    int myrank, err;

    MPI_Comm_rank( h->comm, &myrank );

    if( 0 == myrank ) {
        if( MPI_File_write_shared( h->file, buf, count, MPI_BYTE, MPI_STATUS_IGNORE )) {
            PVO_WARN( "MPI_File_write_shared failed." );
            err = -1;
        } else {
            err =  0;
        }
    }

    MPI_Bcast( &err, 1, MPI_INT, 0, h->comm );
    return err;
}


int pvo_low_io_MPI_write_ordered( pvo_low_io_file_handle_t self,
                                  void* buf,
                                  int   count )
{
    pvo_low_io_MPI_internal_file_handle_t* h = self->handle;

    if( MPI_File_write_ordered( h->file, buf, count, MPI_BYTE, MPI_STATUS_IGNORE )) {
            PVO_WARN( "MPI_File_write_ordered failed." );
            return -1;
    }

    return 0;
}

