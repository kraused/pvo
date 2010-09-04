
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo.h"
#include "pvo_report.h"

#include <iso646.h>

int main( int argc, char** argv ) {
    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    pvo_island_t island;
    pvo_create_island( pvo_world_rank()%2, &island );

    if( pvo_world_size() > 1 ) {
        if( 2 != island.nislands )
            PVO_DIE( "island.nislands = %d.", island.nislands );

        if( 0 != island.no and 1 != island.no )
            PVO_DIE( "island.no = %d.", island.no );
    }

    pvo_delete_island( &island );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

