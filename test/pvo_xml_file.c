
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

    pvo_create_island( pvo_world_rank()%2, &island );

    snprintf( filename, sizeof(filename), "test%d.xml", island.no );

    f  = pvo_low_io_file_handle_create( PVO_LOW_IO_MPI );
    fh = pvo_xml_file_create( filename, &island, f );

    pvo_xml_file_new_group    ( fh, "A%03d", 5 );
    pvo_xml_file_write_ordered( fh, &c, 1, MPI_CHAR );
    pvo_xml_file_end_group    ( fh, "A%03d", 5 );

    pvo_xml_file_delete( fh );
    pvo_low_io_file_handle_delete( f );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

