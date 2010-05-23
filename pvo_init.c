
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_decls.h"
#include "pvo_report.h"

int pvo_init( MPI_Comm comm ) {
    // If we cannot even duplicate the communicator
    // it doesn't make sense to continue
    if( MPI_Comm_dup( comm, &PVO_COMM_WORLD )) {
        PVO_DIE( "MPI_Comm_dup failed." );
        return -1;
    }

    if( MPI_COMM_NULL == PVO_COMM_WORLD )
        PVO_WARN( "MPI_COMM_NULL == PVO_COMM_WORLD" );

    return 0;
}

