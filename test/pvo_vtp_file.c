
/// vim: tabstop=4:expandtab:hlsearch

#include "../pvo.h"
#include "../pvo_report.h"
#include "../pvo_memory.h"

#include <iso646.h>

int main( int argc, char** argv ) {
    pvo_cookie_t cookie;
    pvo_vtp_file_t fh;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( -1 == pvo_cookie_create( pvo_world_rank()%2, &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

//    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32  , 8, "X", X ))
//        PVO_DIE( "pvo_cookie_insert_var() failed." );
//    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "Y", Y ))
//        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( -1 == pvo_vtp_file_open( "test4", cookie, &fh ))
        PVO_DIE( "pvo_vtp_file_open() failed." );

    pvo_file_write( (pvo_file_t )fh );

    if( -1 == pvo_vtp_file_close( fh ))
        PVO_DIE( "pvo_file_vtp_close() failed." );

    if( -1 == pvo_cookie_delete( cookie ))
        PVO_DIE( "pvo_cookie_delete() failed." );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

