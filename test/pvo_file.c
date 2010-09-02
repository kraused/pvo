
/// vim: tabstop=4:expandtab:hlsearch

#include "../pvo.h"
#include "../pvo_report.h"
#include "../pvo_memory.h"

#include <iso646.h>

int main( int argc, char** argv ) {
    pvo_cookie_t cookie;
    pvo_file_t   fh;
    void* p = NULL;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( -1 == pvo_cookie_create( pvo_world_rank()%2, &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32  , 8, "X", p ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );
    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "Y", p ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    fh = pvo_malloc( sizeof(*fh) );
    if( -1 == pvo_file_create( "test2", PVO_FILE_BYTE_ORDER_MACHINE, cookie, fh ))
        PVO_DIE( "pvo_file_create() failed." );
    if( -1 == pvo_file_delete( fh ))
        PVO_DIE( "pvo_file_delete() failed." );
    pvo_free( fh );

    if( -1 == pvo_cookie_delete( cookie ))
        PVO_DIE( "pvo_cookie_delete() failed." );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

