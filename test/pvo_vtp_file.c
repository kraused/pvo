
/// vim: tabstop=4:expandtab:hlsearch

/* Copyright 2010 University of Lugano. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of the University of Lugano.
 */

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

