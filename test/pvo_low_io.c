
/// vim: tabstop=4:expandtab:hlsearch

#include "../pvo.h"
#include "../pvo_report.h"

int main( int argc, char** argv ) {
    pvo_low_io_file_handle_t fh;
    char c = 'A';

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( 0 == pvo_world_rank() ) {
        fh = pvo_low_io_file_handle_create( PVO_LOW_IO_SINGLE );
        if( -1 == fh->open( fh, "t0.dat", PVO_COMM_WORLD ))
            PVO_DIE( "open failed." );
        if( -1 == fh->write_single( fh, &c, 1 ))
            PVO_DIE( "write_single failed." );
        if( -1 == fh->write_ordered( fh, &c, 1 ))
            PVO_DIE( "write_ordered failed." );
        if( -1 == fh->close( fh ))
            PVO_DIE( "close failed." );
    }

    fh = pvo_low_io_file_handle_create( PVO_LOW_IO_MPI );
    if( -1 == fh->open( fh, "t1.dat", PVO_COMM_WORLD ))
        PVO_DIE( "open failed." );
    if( -1 == fh->write_single( fh, &c, 1 ))
        PVO_DIE( "write_single failed." );
    if( -1 == fh->write_ordered( fh, &c, 1 ))
        PVO_DIE( "write_ordered failed." );
    if( -1 == fh->close( fh ))
        PVO_DIE( "close failed." );

    fh = pvo_low_io_file_handle_create( PVO_LOW_IO_POSIX );
    if( -1 == fh->open( fh, "t2.dat", PVO_COMM_WORLD ))
        PVO_DIE( "open failed." );
    if( -1 == fh->write_single( fh, &c, 1 ))
        PVO_DIE( "write_single failed." );
    if( -1 == fh->write_ordered( fh, &c, 1 ))
        PVO_DIE( "write_ordered failed." );
    if( -1 == fh->close( fh ))
        PVO_DIE( "close failed." );
    
    pvo_quit();
    MPI_Finalize();

    return 0;
}

