
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

#include <stdio.h>

int main( int argc, char** argv ) {
    pvo_island_t island;
    char filename[128];
    pvo_xml_file_t           fh;
    pvo_low_io_file_handle_t f;
    char c = 'B';

    MPI_Init( &argc, &argv );
    pvo_init( MPI_COMM_WORLD );

    if( -1 == pvo_create_island( pvo_world_rank()%2, &island ))
        PVO_DIE( "pvo_create_island() failed." );

    snprintf( filename, sizeof(filename), "test%d.xml", island.no );

    if( -1 == pvo_low_io_file_handle_create( PVO_DEFAULT_LOW_IO_LAYER, &f ))
        PVO_DIE( "pvo_low_io_file_handle_create() failed." );

    if( -1 == pvo_xml_file_create( filename, &island, f, &fh ))
        PVO_DIE( "pvo_xml_file_create() failed." );

    if( -1 == pvo_xml_file_new_group    ( fh, "A%03d", 5 ))
        PVO_DIE( "pvo_xml_file_new_group() failed." );
    if( -1 == pvo_xml_file_write_ordered( fh, &c, 1, MPI_CHAR ))
        PVO_DIE( "pvo_xml_file_write_ordered() failed." );
    if( -1 == pvo_xml_file_end_group    ( fh, "A%03d", 5 ))
        PVO_DIE( "pvo_xml_file_end_group() failed." );

    if( -1 == pvo_xml_file_delete( fh ))
        PVO_DIE( "pvo_xml_file_delete() failed." );
    if( -1 == pvo_low_io_file_handle_delete( f ))
        PVO_DIE( "pvo_low_io_file_handle_delete() failed." );

    pvo_quit();
    MPI_Finalize();

    return 0;
}

