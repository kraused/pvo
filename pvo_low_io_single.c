
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_low_io_single.h"
#include "pvo_report.h"
#include "pvo_memory.h"

/// We are using the standard I/O provided by stdio.h
/// This should be fine since this transport layer is
/// anyway designed for metadata output, i.e., for small
/// data volumes.
#include <stdio.h>

int pvo_low_io_single_open( pvo_low_io_file_handle_t self,    
                            const char* name,   
                            MPI_Comm    comm )
{
    self->handle = fopen( name, "w" );

    if( NULL == ( self->handle = fopen( name, "w" ))) {
        PVO_WARN( "Opening file \"%s\" failed.", name );
        return -1;
    }

    return 0;
}


int pvo_low_io_single_close( pvo_low_io_file_handle_t self )
{
    if( EOF == fclose( (FILE* )self->handle ))
        return -1;
    
    return 0;
}


/// A thin wrapper around fwrite which performs checks and
/// makes sure we return the correct values
static int pvo_low_io_single_write( FILE* fh, void* buf, size_t count )
{
    if( NULL == fh ) {
        PVO_WARN( "Attempt to write to a file which is not open." );
        return -1;
    }

    if( count != fwrite( buf, 1, count, fh ))
        return -1;

    return 0;
}


int pvo_low_io_single_write_single( pvo_low_io_file_handle_t self,
                                    void* buf,
                                    int   count )
{
    if( NULL == self ) {
        PVO_WARN( "Invalid input: NULL == self." );
        return -1;
    }

    return pvo_low_io_single_write( (FILE* )self->handle, buf, count );
}


int pvo_low_io_single_write_ordered( pvo_low_io_file_handle_t self,
                                     void* buf,
                                     int   count )
{
    if( NULL == self ) {
        PVO_WARN( "Invalid input: NULL == self." );
        return -1;
    }

    /// An ordered write with a single process is the same as
    /// a normal write
    return pvo_low_io_single_write( (FILE* )self->handle, buf, count );
}

