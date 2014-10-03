
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

#include <iso646.h>

int main( int argc, char** argv ) {
    pvo_cookie_t cookie;
    void* p = NULL;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( -1 == pvo_cookie_create( pvo_world_rank()%2, &cookie ))
        PVO_DIE( "pvo_cookie_create() failed." );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_INT32, 8, "X", p ))
        PVO_DIE( "pvo_cookie_insert_var() failed." );

    if( 1 != pvo_cookie_number_var( cookie ))
        PVO_DIE( " pvo_cookie_number_var = %d.",
                   pvo_cookie_number_var( cookie ));

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "Y", p ))
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

    if( -1 == pvo_cookie_delete( cookie ))
        PVO_DIE( "pvo_cookie_delete() failed." );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

