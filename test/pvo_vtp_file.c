
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo.h"
#include "pvo_report.h"
#include "pvo_memory.h"

#include <iso646.h>

int main( int argc, char** argv ) {
    pvo_cookie_t cookie;
    pvo_vtp_file_t fh;
    pvo_float3_t pt[9];
    int32_t X[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    pt[0][0] = 2*pvo_world_rank() + 0.5;
    pt[0][1] = 2*pvo_world_rank() + 0.5;
    pt[0][2] = 2*pvo_world_rank() + 0.5;

    pt[1][0] = 2*pvo_world_rank() + 1.5;
    pt[1][1] = 2*pvo_world_rank() + 0.5;
    pt[1][2] = 2*pvo_world_rank() + 0.5;

    pt[2][0] = 2*pvo_world_rank() + 1.5;
    pt[2][1] = 2*pvo_world_rank() + 1.5;
    pt[2][2] = 2*pvo_world_rank() + 0.5;

    pt[3][0] = 2*pvo_world_rank() + 0.5;
    pt[3][1] = 2*pvo_world_rank() + 1.5;
    pt[3][2] = 2*pvo_world_rank() + 0.5;

    pt[4][0] = 2*pvo_world_rank() + 0.5;
    pt[4][1] = 2*pvo_world_rank() + 0.5;
    pt[4][2] = 2*pvo_world_rank() + 1.5;

    pt[5][0] = 2*pvo_world_rank() + 1.5;
    pt[5][1] = 2*pvo_world_rank() + 0.5;
    pt[5][2] = 2*pvo_world_rank() + 1.5;

    pt[6][0] = 2*pvo_world_rank() + 1.5;
    pt[6][1] = 2*pvo_world_rank() + 1.5;
    pt[6][2] = 2*pvo_world_rank() + 1.5;

    pt[7][0] = 2*pvo_world_rank() + 0.5;
    pt[7][1] = 2*pvo_world_rank() + 1.5;
    pt[7][2] = 2*pvo_world_rank() + 1.5;

    pt[8][0] = 2*pvo_world_rank() + 1.75;
    pt[8][1] = 2*pvo_world_rank() + 1.75;
    pt[8][2] = 2*pvo_world_rank() + 1.75;

    if( -1 == pvo_cookie_create( pvo_world_rank()%2, &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32  , 1, "X", X ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( -1 == pvo_vtp_file_open( "test4", cookie, 9, pt, &fh ))
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

