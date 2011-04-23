
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

#include "pvo_config.h"

#include "pvo_file.h"
#include "pvo_memory.h"
#include "pvo_report.h"

#include <stdio.h>
#include <string.h>

int pvo_file_create( const char*           filename,
                     pvo_file_byte_order_t bo,
                     pvo_cookie_t          cki,
                     pvo_file_t            fh )
{
    int err = 0;

    fh->bo  = bo;
    if( PVO_FILE_BYTE_ORDER_MACHINE == fh->bo ) {
#if 1 ==  PVO_HAVE_SYSTEM_LE
        fh->bo = PVO_FILE_BYTE_ORDER_LE;
#endif
#if 1 == PVO_HAVE_SYSTEM_BE
        fh->bo = PVO_FILE_BYTE_ORDER_BE;
#endif
    }

    switch( fh->bo ) {
    case PVO_FILE_BYTE_ORDER_LE:
        snprintf( fh->bo_str, sizeof(fh->bo_str), "LittleEndian" );
        break;
    case PVO_FILE_BYTE_ORDER_BE:
        snprintf( fh->bo_str, sizeof(fh->bo_str), "BigEndian" ); 
        break;
    default:
        PVO_ERROR( "Invalid byte order." );
        goto fn_fail;
    }

    fh->cki = cki;
    strncpy( fh->name, filename, sizeof(fh->name) );

    fh->suffix[0] = 0x0;    /// Make sure the default suffix has
                            /// length zero

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

/// Write the meta-data file used to merge the files
/// written by different islands into one. This merge
/// is done automatically by the post-processing programs
/// like paraview
static int pvo_file_write_meta( pvo_file_t fh )
{
    int err = 0;
    pvo_low_io_file_handle_t f;
    pvo_xml_file_t xml;
    char buf[128];

    if( 0 != fh->cki->island.no || 0 != fh->cki->island.rank )
        goto fn_exit;

    if( 0 == strlen( fh->suffix )) {
        PVO_ERROR( "File suffix has length zero." );
        goto fn_fail;
    }

    if( sizeof(buf) <= snprintf( buf, sizeof(buf),
                                 "%s.p%s",
                                 fh->name, fh->suffix )) {
        PVO_ERROR( "Filename including extension exceeds limit." );
        goto fn_fail;
    }

    if( -1 == pvo_low_io_file_handle_create( PVO_LOW_IO_SINGLE, &f ))
        goto fn_fail;
    if( -1 == pvo_xml_file_create( buf, NULL, f, &xml ))
        goto fn_fail;

    err = fh->write_meta( fh, xml );

    if( -1 == pvo_xml_file_delete( xml ))
        goto fn_fail;
    if( -1 == pvo_low_io_file_handle_delete( f ))
        goto fn_fail;

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

/// Write the data
static int pvo_file_write_data( pvo_file_t fh )
{
    int err;
    pvo_low_io_file_handle_t f;
    pvo_xml_file_t xml;
    char buf[128];

    if( 0 == strlen( fh->suffix )) {
        PVO_ERROR( "File suffix has length zero." );
        goto fn_fail;
    }

    if( sizeof(buf) <= snprintf( buf, sizeof(buf),
                                 "%s-%d.%s",
                                 fh->name, fh->cki->island.no, fh->suffix )) {
        PVO_ERROR( "Filename including extension exceeds limit." );
        goto fn_fail;
    }

    if( -1 == pvo_low_io_file_handle_create( PVO_DEFAULT_LOW_IO_LAYER, &f ))
        goto fn_fail;
    if( -1 == pvo_xml_file_create( buf, &fh->cki->island, f, &xml ))
        goto fn_fail;

    err = fh->write_data( fh, xml );

    if( -1 == pvo_xml_file_delete( xml ))
        goto fn_fail;
    if( -1 == pvo_low_io_file_handle_delete( f ))
        goto fn_fail;

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}


int pvo_file_write( pvo_file_t fh ) {
    int err = 0;

    if( NULL == fh->write_meta || NULL == fh->write_data ) {
        PVO_ERROR( "Invalid file: Missing implementation of "
                   "write_meta or write_data" );
        goto fn_fail;
    }

// Write the meta data file also if we have only one island. This
// will ensure that the data is named correctly and we can load
// time datain Paraview.
//
//    if( 1 < fh->cki->island.nislands )
        err = pvo_file_write_meta( fh );
//    else
//        err = 0;    

    if( -1 == err )
        PVO_ERROR( "Writing parallel vtk file failed." );
    /// Go on to prevent deadlocking but do not touch
    /// err (or at least do not clean it)

    if( -1 == pvo_file_write_data( fh ))
        goto fn_fail;

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

int pvo_file_delete( pvo_file_t fh ) {
    int err = 0;

    /* Nothing to do */

    return err;
}

