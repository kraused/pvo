
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

#include "pvo_xml_file.h"
#include "pvo_report.h"
#include "pvo_memory.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int pvo_xml_file_create( const char*                name,
                         const pvo_island_t*        island,
                         pvo_low_io_file_handle_t   f,
                         pvo_xml_file_t*            fh )
{
    static const char string[] = "<?xml version=\"1.0\"?>\n";
    int err = 0;
    MPI_Comm comm;

    *fh = pvo_malloc( sizeof(struct pvo_xml_file) );

    strncpy( (*fh)->version, "1.0", 3 );

    (*fh)->island   = island;
    (*fh)->f        = f;
    (*fh)->ilvl     = 0;
    (*fh)->itabs[0] = 0x0;

    if( NULL == island ) /// Make sure we do not dereference island
        comm = MPI_COMM_NULL;
    else
        comm = island->comm;

    if( -1 == (*fh)->f->open( (*fh)->f, name, comm ))
        goto fn_fail;
    if( -1 == (*fh)->f->write_single( (*fh)->f, (void* )string, strlen( string )))
        goto fn_fail;

fn_exit:
    return err;
fn_fail:
    pvo_free( *fh );
    PVO_DIE( "pvo_xml_file_create() failed. Giving up ..." );
    err = -1;
    goto fn_exit;
}

int pvo_xml_file_delete( pvo_xml_file_t fh ) {
    int err = 0;

    if( NULL == fh ) {
        PVO_WARN( "Invalid input: NULL == fh." );
        goto fn_fail;        
    }

    if( 0 != fh->ilvl )
        PVO_WARN( "Found inconsistent nesting. This indicates a problem "
                  "with the XML file." );

    err = fh->f->close( fh->f );
    pvo_free( fh );
    
fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

/// Return the content to write for the routines pvo_xml_new_group(),
/// pvo_xml_end_group() and pvo_xml_file_write_element()
inline char* print( pvo_xml_file_t fh, 
                    const char*     delimiter[2],
                    const char*     fmt, 
                    va_list         vl ) {
           char str[256];
    static char buf[256];

    if( sizeof(str) <= snprintf( str, sizeof(str),
                                 "%s%s%s%s\n", fh->itabs, 
                                 delimiter[0], fmt, 
                                 delimiter[1] ))
        PVO_DIE( "Line too long." );
    if( sizeof(buf) <= vsnprintf( buf, sizeof(buf),
                                  str, vl ))
        PVO_DIE( "snprintf failed." );

    return buf;
}

int pvo_xml_file_new_group( pvo_xml_file_t fh, const char* fmt, ... ) {
    va_list vl;
    char*   buf;

    va_start( vl, fmt );
    buf = print( fh, (const char* [2]){ "<", ">" }, fmt, vl );
    va_end  ( vl );

    fh->itabs[  fh->ilvl] = '\t';
    fh->itabs[++fh->ilvl] = 0x0;

    if( fh->ilvl > PVO_XML_MAXDEPTH) {
        PVO_WARN( "XML file uses too much nesting." );
        fh->ilvl = PVO_XML_MAXDEPTH;
    }

    return fh->f->write_single( fh->f, (void* )buf, strlen( buf ));
}

int pvo_xml_file_end_group( pvo_xml_file_t fh, const char* fmt, ... ) {
    va_list vl;
    char*   buf;

    if( 0 > (--fh->ilvl))
        fh->ilvl = 0;   // Clip range.
    fh->itabs[fh->ilvl] = 0x0;

    va_start( vl, fmt );
    buf = print( fh, (const char* [2]){ "</", ">" }, fmt, vl );
    va_end  ( vl );

    return fh->f->write_single( fh->f, (void* )buf, strlen( buf ));
}

int pvo_xml_file_write_element( pvo_xml_file_t fh, const char* fmt, ... ) {
    va_list vl;
    char*   buf;

    va_start( vl, fmt );
    buf = print( fh, (const char* [2]){ "<", "/>" }, fmt, vl );
    va_end  ( vl );

    return fh->f->write_single( fh->f, (void* )buf, strlen( buf ));
}

int pvo_xml_file_write_ordered( pvo_xml_file_t fh, void* buf, int count, 
                                MPI_Datatype datatype ) {
    int size;
    MPI_Type_size( datatype, &size );

    return fh->f->write_ordered( fh->f, buf, count*size );
}

int pvo_xml_file_write_single( pvo_xml_file_t fh, void* buf, int count,
                               MPI_Datatype datatype )
{
    int size;
    MPI_Type_size( datatype, &size );

    return fh->f->write_single( fh->f, buf, count*size );
}

int pvo_xml_file_low_io_file_handle( pvo_xml_file_t fh,
                                     pvo_low_io_file_handle_t* f )
{
    *f = fh->f;
    return 0;
}


