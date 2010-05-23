
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_config.h"

#include "pvo_file.h"
#include "pvo_memory.h"
#include "pvo_report.h"

#include <stdio.h>
#include <string.h>

int pvo_file_open( const char*           filename,
                   pvo_file_byte_order_t bo,
                   pvo_cookie_t          cki,
                   pvo_file_t*           fh ) {
    int err = 0;
    pvo_low_io_file_handle_t f;

    char buf[128];
    if( sizeof(buf) <= snprintf( buf, sizeof(buf),
                                 "%s-%d.vtu",
                                 filename, cki->island.no )) {
        PVO_ERROR( "Filename including extension exceeds limit." );
        goto fn_fail;
    }

    *fh        = pvo_malloc( sizeof(*fh) );
    (*fh)->bo  = bo;
    (*fh)->cki = cki;
    strncpy( (*fh)->name, filename, sizeof((*fh)->name) );

    if( -1 == pvo_low_io_file_handle_create( PVO_DEFAULT_LOW_IO_LAYER, &f ))
        goto fn_fail;
    if( -1 == pvo_xml_file_create( buf, &cki->island, f, &(*fh)->f ))
        goto fn_fail;

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

int pvo_file_write( pvo_file_t fh ) {
    int err = 0;

    if( NULL == fh->write ) {
        PVO_ERROR( "Invalid file: NULL == fh->write." );
        goto fn_fail;
    }

    err = fh->write( fh );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

int pvo_file_close( pvo_file_t fh ) {
    int err = 0;
    pvo_low_io_file_handle_t f;

    if( -1 == pvo_xml_file_low_io_file_handle( fh->f, &f ))
        goto fn_fail;
    if( -1 == pvo_low_io_file_handle_delete( f ))
        goto fn_fail;
    if( -1 == pvo_xml_file_delete( fh->f ))
        goto fn_fail;

    pvo_free( fh );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

