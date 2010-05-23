
/// vim: tabstop=4:expandtab:hlsearch

#include "../pvo.h"
#include "../pvo_report.h"

#include <iso646.h>

int main( int argc, char** argv ) {
    pvo_cookie_t cookie;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( -1 == pvo_cookie_create( pvo_world_rank()%2, &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32, 8, "X" ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( 1 != pvo_cookie_number_var( cookie ))
        PVO_DIE( " pvo_cookie_number_var = %d.", 
                   pvo_cookie_number_var( cookie ));

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "Y" ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( 2 != pvo_cookie_number_var( cookie ))
        PVO_DIE( " pvo_cookie_number_var = %d.", 
                   pvo_cookie_number_var( cookie ));

    if( -1 == pvo_cookie_remove_var( cookie, "X" ))
        PVO_DIE( "pvo_cookie_remove_var() failed." );

    if( 1 != pvo_cookie_number_var( cookie ))
        PVO_DIE( " pvo_cookie_number_var = %d.", 
                   pvo_cookie_number_var( cookie ));

    if( -1 == pvo_cookie_remove_var( cookie, "Y" ))
        PVO_DIE( "pvo_cookie_remove_var() failed." );

    if( 0 != pvo_cookie_number_var( cookie ))
        PVO_DIE( " pvo_cookie_number_var = %d.", 
                   pvo_cookie_number_var( cookie ));

    pvo_cookie_delete( cookie );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

