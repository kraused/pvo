
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
    pvo_vti_file_t fh;
    int whole_extent[6], local_extent[6];
    int X[3][3][3];
    float Y[2][2][2];
    int i, j, k;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    for(i = 0; i < 3; ++i) {
        for(j = 0; j < 3; ++j) {
            for(k = 0; k < 3; ++k) {
                X[i][j][k] = k + 3*(j + 3*i);
            }
        }
    }

    for(i = 0; i < 2; ++i) {
        for(j = 0; j < 2; ++j) {
            for(k = 0; k < 2; ++k) {
                Y[i][j][k] = pvo_world_rank();
            }
        }
    }

    whole_extent[0] = 0;
    whole_extent[1] = 2*pvo_world_size();
    whole_extent[2] = 0;
    whole_extent[3] = 2;
    whole_extent[4] = 0;
    whole_extent[5] = 2;

    local_extent[0] = 2*pvo_world_rank();
    local_extent[1] = local_extent[0] + 2;
    local_extent[2] = 0;
    local_extent[3] = local_extent[2] + 2;
    local_extent[4] = 0;
    local_extent[5] = local_extent[4] + 2;

    /* Write one file per processing element.
     */
    if( -1 == pvo_cookie_create( pvo_world_rank(), &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32  , 1, "X", X ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );
    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "Y", Y ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( -1 == pvo_vti_file_open( "test5", cookie,
                                 whole_extent,
                                 0,
                                 (float   [3]){ 0.0, 0.0, 0.0 },
                                 (float   [3]){ 0.1, 0.1, 0.1 },
                                 local_extent, &fh ))
        PVO_DIE( "pvo_vti_file_open() failed." );

    pvo_file_write( (pvo_file_t )fh );

    if( -1 == pvo_vti_file_close( fh ))
        PVO_DIE( "pvo_file_vti_close() failed." );

    if( -1 == pvo_cookie_delete( cookie ))
        PVO_DIE( "pvo_cookie_delete() failed." );

    /* Write one file in total.
     */
    if( -1 == pvo_cookie_create( 0, &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32  , 1, "X", X ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );
    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "Y", Y ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( -1 == pvo_vti_file_open( "test6", cookie,
                                 whole_extent,
                                 0,
                                 (float   [3]){ 0.0, 0.0, 0.0 },
                                 (float   [3]){ 0.1, 0.1, 0.1 },
                                 local_extent, &fh ))
        PVO_DIE( "pvo_vti_file_open() failed." );

    pvo_file_write( (pvo_file_t )fh );

    if( -1 == pvo_vti_file_close( fh ))
        PVO_DIE( "pvo_file_vti_close() failed." );

    if( -1 == pvo_cookie_delete( cookie ))
        PVO_DIE( "pvo_cookie_delete() failed." );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

