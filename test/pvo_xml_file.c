
/// vim: tabstop=4:expandtab:hlsearch

#include "../pvo.h"
#include "../pvo_report.h"

#include <stdio.h>

int main( int argc, char** argv ) {
    pvo_island_t island;
    char filename[128];
    pvo_xml_file_t           fh;
    pvo_low_io_file_handle_t f;
    char c = 'B';

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( -1 == pvo_create_island( pvo_world_rank()%2, &island ))
        PVO_DIE( "pvo_create_island() failed." );

    snprintf( filename, sizeof(filename), "test%d.xml", island.no );

    if( -1 == pvo_low_io_file_handle_create( PVO_DEFAULT_LOW_IO_LAYER, &f ))
        PVO_DIE( "pvo_low_io_file_handle_create() failed." );

    if( -1 == pvo_xml_file_create( filename, &island, f, &fh ))
        PVO_DIE( "pvo_xml_file_create() failed." );

    if( -1 == pvo_xml_file_new_group    ( fh, "A%03d", 5 ))
        PVO_DIE( "pvo_xml_file_new_group() failed." );
    if( -1 == pvo_xml_file_write_ordered( fh, &c, 1, MPI_CHAR ))
        PVO_DIE( "pvo_xml_file_write_ordered() failed." );
    if( -1 == pvo_xml_file_end_group    ( fh, "A%03d", 5 ))
        PVO_DIE( "pvo_xml_file_end_group() failed." );

    if( -1 == pvo_xml_file_delete( fh ))
        PVO_DIE( "pvo_xml_file_delete() failed." );
    if( -1 == pvo_low_io_file_handle_delete( f ))
        PVO_DIE( "pvo_low_io_file_handle_delete() failed." );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

