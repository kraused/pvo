
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

