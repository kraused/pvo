
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
#include <math.h>
#include <pvo.h>
#include "parse.h"


void decompose( int* N_r, double* r_min, double* r_max )
{
    // Mesh spacing in r direction
    double dr = (*r_max - *r_min)/(*N_r);
    
    int N = (int )((double )(*N_r)/(double )pvo_world_size());

    if( pvo_world_size() == pvo_world_rank()+1 )
    {
        /* Need to account for the possibility that N_r is not 
           divisible by the number of cores. This is a dump load
           balancing function but sufficies for meshes which are
           large enough */
        *N_r = *N_r - (pvo_world_size()-1)*N;
    } else
        *N_r = N;

    *r_min = *r_min + pvo_world_rank()*N*dr;
    *r_max = *r_min + (*N_r)*dr;
}

void create_chunk( int N_u, int N_v, int N_r, 
                   double r_min, double r_max, double gamma,
                   long nnodes, pvo_float3_t* pts, 
                   long ncells, int* cia, int* cja, uint8_t* types,
                   double* U, float* V )
{
    int iu, iv, ir, i, k;
    double theta, phi, r, t, u, v, z;
    // Mesh spacing
    double dr = (r_max - r_min)/N_r;

    /* We compute the values on the sphere by projection from the
       plane with this z value on the sphere. The closer this value
       is to 1.0, the lower is the curvature
     */
    z = sqrt(0.5*(1 - gamma*gamma));

    for( ir = 0; ir <= N_r; ++ir )
        for( iv = 0; iv <= N_v; ++iv )
            for( iu = 0; iu <= N_u; ++iu )
            {
                u = -z + ( (2*z)/(double )N_u )*iu;
                v = -z + ( (2*z)/(double )N_v )*iv;
                r = r_min + ir*dr;

                t = sqrt(u*u + v*v + gamma*gamma);

                k = iu + (N_u+1)*iv + (N_u+1)*(N_v+1)*ir;
                pts[k][0] = u    *r/t;
                pts[k][1] = v    *r/t;
                pts[k][2] = gamma*r/t;
            }

    for( ir = 0; ir < N_r; ++ir )
        for( iv = 0; iv < N_v; ++iv )
            for( iu = 0; iu < N_u; ++iu )
            {
                k = 8*(iu + N_u*iv + N_u*N_v*ir);
                
#undef  INDEX
#define INDEX(iu,iv,ir) ( (iu) + (N_u+1)*(iv) + (N_u+1)*(N_v+1)*(ir) )
                cja[k+0] = INDEX(iu  ,iv  ,ir  );
                cja[k+1] = INDEX(iu+1,iv  ,ir  );
                cja[k+2] = INDEX(iu+1,iv+1,ir  );
                cja[k+3] = INDEX(iu  ,iv+1,ir  );
                cja[k+4] = INDEX(iu  ,iv  ,ir+1);
                cja[k+5] = INDEX(iu+1,iv  ,ir+1);
                cja[k+6] = INDEX(iu+1,iv+1,ir+1);
                cja[k+7] = INDEX(iu  ,iv+1,ir+1);
            }

    for( i = 0; i <= ncells; ++i )
        cia[i] = 8*i;
    for( i = 0; i <  ncells; ++i )
        types[i] = PVO_VTU_HEXAHEDRON;

    srand( 0 );

    for( i = 0; i < nnodes; ++i ) {
        U[3*i+0] = (1.0*rand())/(1.0*RAND_MAX);
        U[3*i+1] = (1.0*rand())/(1.0*RAND_MAX);
        U[3*i+2] = (1.0*rand())/(1.0*RAND_MAX);
    }

    for( i = 0; i < ncells; ++i )
        V[i] = (1.0*rand())/(1.0*RAND_MAX);
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
    int ni;
    long N;
    pvo_cookie_t  cookie;
    pvo_float3_t* pts;
    int*          cia;
    int*          cja;
    uint8_t*      types;
    pvo_vtu_file_t fh;
    double* U;
    float*  V;
    int*    ranks;
    double t0, t1, t2, t3;
    long i, nnodes, ncells;
    int N_u, N_v, N_r;
    double r_min, r_max, gamma;

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );


    MPI_Barrier( MPI_COMM_WORLD );
    t0 = MPI_Wtime();

    // Minimal and maximal value for the radius
    r_min = parse_cmdline_double();
    r_max = parse_cmdline_double();

    gamma = parse_cmdline_double();

    // Number of grid cells for each direction
    N_u = parse_cmdline_double();
    N_v = parse_cmdline_double();
    N_r = parse_cmdline_double();

    /* The data is decomposed by slicing in the radial
       direction
     */
    decompose( &N_r, &r_min, &r_max );

    nnodes = (N_u+1)*(N_v+1)*(N_r+1);
    ncells =  N_u   * N_v   * N_r   ;

    pts   = malloc( nnodes*sizeof(pvo_float3_t) );
    cia   = malloc( (ncells+1)*sizeof(int) );
    cja   = malloc( 8*ncells*sizeof(int) );
    U     = malloc( nnodes*3*sizeof(double) );
    V     = malloc( ncells*sizeof(float) );
    types = malloc( ncells*sizeof(uint8_t) );
    ranks = malloc( ncells*sizeof(int) );

    ni = parse_cmdline_int();

    if( 0 == pvo_world_rank() )
    {
        printf( " +-----------------------------------------+\n" );
        printf( " |         PVO CHUNK VTU BENCHMARK         |\n" );
        printf( " +-----------------------------------------+\n" );
        printf( "\n");
        printf( " PVO_DEFAULT_LOW_IO_LAYER   : \"%s\"\n", str_low_io_layer[PVO_DEFAULT_LOW_IO_LAYER] );
        printf( " no of cores                : %d\n", pvo_world_size() );
        printf( " no of nodes                : %d\n", nnodes );
        printf( " no of cells                : %d\n", ncells );
        printf( " no of islands              : %d\n", ni );
    }

    create_chunk( N_u, N_v, N_r, r_min, r_max, gamma, nnodes, pts, ncells, cia, cja, types, U, V );
    for( i = 0; i < ncells; ++i )
       ranks[i] = pvo_world_rank(); 

    if( -1 == pvo_cookie_create( pvo_world_rank()%ni, &cookie ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_NODEDATA, PVO_VAR_FLOAT64, 3, "U", U ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_FLOAT32, 1, "V", V ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_cookie_insert_var( cookie, PVO_VAR_CELLDATA, PVO_VAR_INT32, 1, "ranks", ranks ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    if( -1 == pvo_vtu_file_open( "chunk", cookie, nnodes, pts, ncells, cia, cja, types, &fh ))
        MPI_Abort( MPI_COMM_WORLD, __LINE__ );

    MPI_Barrier( MPI_COMM_WORLD );
    t1 = MPI_Wtime();

    pvo_file_write( (pvo_file_t )fh );

    MPI_Barrier( MPI_COMM_WORLD );
    t2 = MPI_Wtime();

    if( 0 == pvo_world_rank() )
        printf( " time [sec]                 : %f\n", t2-t1 );
            
     N = (3*4/*pts*/ + 3*8/*U*/)*nnodes + (4/*cia*/ + 8*4/*cja*/ + 1/*types*/ + 4/*V*/ + 4/*ranks*/)*ncells;
     MPI_Allreduce( MPI_IN_PLACE, &N, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    if( 0 == pvo_world_rank() )
        printf( " bandwidth [MB/sec]         : %f\n", (1.0*N)/(1048576*(t2-t1)) );

    if( -1 == pvo_vtu_file_close( fh ))
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

