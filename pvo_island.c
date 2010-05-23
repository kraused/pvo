
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_island.h"
#include "pvo_report.h"

int pvo_create_island( int color, pvo_island_t* island ) {
    int t;

    if( MPI_Comm_split( PVO_COMM_WORLD, color, 
                        pvo_world_rank(), &island->comm ))
        PVO_DIE( "MPI_Comm_split failed." );
    
    if( MPI_Comm_size( island->comm, &island->size ))
        PVO_DIE( "MPI_Comm_size failed." );
    if( MPI_Comm_rank( island->comm, &island->rank ))
        PVO_DIE(" MPI_Comm_rank failed." );
    
    t = ( 0 == island->rank ) ? 1 : 0;
    if( MPI_Allreduce( &t, &island->nislands, 1, MPI_INT,
                       MPI_SUM, PVO_COMM_WORLD ))
        PVO_DIE( "MPI_Allreduce failed." );
    
    if( MPI_Exscan( &t, &island->no, 1, MPI_INT,
                    MPI_SUM, PVO_COMM_WORLD ))
        PVO_DIE( "MPI_Exscan failed." );
    // Need to set no explicitely on rank zero because
    // the MPI standard allows undefined values in the 
    // receive buffer on rank zero
    if( 0 == pvo_world_rank() )
        island->no = 0;
    if( MPI_Bcast( &island->no, 1, MPI_INT, 0, island->comm ))
        PVO_DIE( "MPI_Bcast failed." ); 

    return 0;
}

int pvo_delete_island( pvo_island_t* island ) {
    if( MPI_Comm_free( &island->comm )) {
        PVO_WARN( "MPI_Comm_free failed." );
        return -1;
    }

    return 0;
}

