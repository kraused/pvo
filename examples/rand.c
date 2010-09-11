
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pvo.h>
#include "parse.h"


#ifdef HAVE_MPI
void compute_offset(double* o)
{
    int cartdim[] = { 0, 0, 0 };
    MPI_Dims_create( pvo_world_size(), 3, cartdim );

    o[0] =  pvo_world_rank()%cartdim[0];
    o[1] = (pvo_world_rank()/cartdim[0]) % cartdim[1];
    o[2] =  pvo_world_rank()/(cartdim[0]*cartdim[1]);
}
#endif

void create_random_points( long N, pvo_float3_t* P, double* U )
{
    long i;
    double o[3];

#ifdef HAVE_MPI
    compute_offset(o);
#else
    o[0] = 0.0;
    o[1] = 0.0;
    o[2] = 0.0;
#endif

    srand( 0 );
    for(i = 0; i < N; ++i)
    {
        P[i][0]  = (1.0*rand())/(1.0*RAND_MAX) + o[0];
        P[i][1]  = (1.0*rand())/(1.0*RAND_MAX) + o[1];
        P[i][2]  = (1.0*rand())/(1.0*RAND_MAX) + o[2];

        U[3*i+0] = (1.0*rand())/(1.0*RAND_MAX);
        U[3*i+1] = (1.0*rand())/(1.0*RAND_MAX);
        U[3*i+2] = (1.0*rand())/(1.0*RAND_MAX);
    }
}

// String representation of the low-level I/O layer choice
const char* str_low_io_layer[3] = 
{
    [PVO_LOW_IO_SINGLE] = "PVO_LOW_IO_SINGLE",
    [PVO_LOW_IO_MPI] = "PVO_LOW_IO_MPI",
    [PVO_LOW_IO_POSIX] = "PVO_LOW_IO_POSIX"
};

int main( int argc, char** argv )
{
    long N;
    int ni;
    pvo_float3_t* P;
    pvo_cookie_t cookie;
    pvo_vtp_file_t fh;
    double* U;
    double t0, t1, t2, t3;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );


    MPI_Barrier( MPI_COMM_WORLD );
    t0 = MPI_Wtime();

    N  = parse_cmdline_int();
    P  = malloc( N*sizeof(pvo_float3_t) );
    U  = malloc( N*3*sizeof(double) );
    ni = parse_cmdline_int();

    if( 0 == pvo_world_rank() )
    {
        printf( " +----------------------------------------+\n" );
        printf( " |         PVO RAND VTP BENCHMARK         |\n" );
        printf( " +----------------------------------------+\n" );
        printf( "\n");
        printf( " PVO_DEFAULT_LOW_IO_LAYER   : \"%s\"\n", str_low_io_layer[PVO_DEFAULT_LOW_IO_LAYER] );
        printf( " no of cores                : %d\n", pvo_world_size() );
        printf( " no of particles per core   : %ld\n", N );
        printf( " no of islands              : %d\n", ni );
    }

    create_random_points( N, P, U );

    if( -1 == pvo_cookie_create( pvo_world_rank()%ni, &cookie ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_FLOAT64, 3, "U", U ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_vtp_file_open( "rand", cookie, N, P, &fh ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    MPI_Barrier( MPI_COMM_WORLD );
    t1 = MPI_Wtime();

    pvo_file_write( (pvo_file_t )fh );

    MPI_Barrier( MPI_COMM_WORLD );
    t2 = MPI_Wtime();

    if( 0 == pvo_world_rank() )
    {
            printf( " time [sec]                 : %f\n", t2-t1 );
            printf( " bandwidth [MB/sec]         : %f\n", (36.0*N*pvo_world_size())/(1048576*(t2-t1)) );
    }

    if( -1 == pvo_vtp_file_close( fh ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_cookie_delete( cookie ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    MPI_Barrier( MPI_COMM_WORLD );
    t3 = MPI_Wtime();

    if( 0 == pvo_world_rank() )
    {
        printf( " total execution time [sec] : %f\n", t3-t0 );
        printf( "\n" );
    }

    pvo_quit();
    return MPI_Finalize();
}

