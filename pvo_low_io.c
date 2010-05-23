
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_low_io.h"
#include "pvo_memory.h"
#include "pvo_report.h"

#include "pvo_low_io_single.h"
#include "pvo_low_io_MPI.h"
#include "pvo_low_io_posix.h"

pvo_low_io_file_handle_t pvo_low_io_file_handle_create( pvo_low_transport_layer_t layer )
{
    pvo_low_io_file_handle_t fh; 
 
    fh         = pvo_malloc( sizeof(struct pvo_low_io_file_handle) );
    fh->layer  = layer;
    fh->handle = NULL;

    switch( layer ) {
    case PVO_LOW_IO_SINGLE:
        fh->open          = pvo_low_io_single_open;
        fh->close         = pvo_low_io_single_close;
        fh->write_single  = pvo_low_io_single_write_single;
        fh->write_ordered = pvo_low_io_single_write_ordered;
        break;
    case PVO_LOW_IO_MPI:
        fh->open          = pvo_low_io_MPI_open;
        fh->close         = pvo_low_io_MPI_close;
        fh->write_single  = pvo_low_io_MPI_write_single;
        fh->write_ordered = pvo_low_io_MPI_write_ordered;
        break;
    case PVO_LOW_IO_POSIX:
        fh->open          = pvo_low_io_posix_open;
        fh->close         = pvo_low_io_posix_close;
        fh->write_single  = pvo_low_io_posix_write_single;
        fh->write_ordered = pvo_low_io_posix_write_ordered;
        break;
    default:
        PVO_ERROR( "Invalid transport layer." );
    }

    return fh;
}


int pvo_low_io_file_handle_delete( pvo_low_io_file_handle_t fh )
{
    pvo_free( fh ); /// pvo->handle already freed by close

    return 0;
}

