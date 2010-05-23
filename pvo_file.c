
/// vim: tabstop=4:expandtab:hlsearch

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
        PVO_WARN( "Filename including extension exceeds limit." );
        goto fn_fail;
    }

    *fh        = pvo_malloc( sizeof(*fh) );
    (*fh)->bo  = bo;
    (*fh)->cki = cki;
    strncpy( (*fh)->name, filename, sizeof((*fh)->name) );

    if( -1 == pvo_low_io_file_handle_create( -1, &f ))
        goto fn_fail;
    if( -1 == pvo_xml_file_create( buf, &cki->island, f, &(*fh)->f ))
        goto fn_fail;

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

//void write_pvtu() {
//    
//}
//
//
//void write_vtu () {
//
 //   /// FIXME Get byte order from arch
//    pvo_xml_new_group( ?, "VTKFile type=\"UnstructuredGrid\" "
//                          "version=\"0.1\" "
//                          "byte_order=\"LittleEndian\"" );
//    pvo_xml_new_group( ?, "UnstructuredGrid" );
//    pvo_xml_new_group( ?, "Piece NumberOfPoints=\"%d\" NumberOfCells=\"%d\"" );
//
//    pvo_xml_new_group( ?, "AppendedData encoding=\"raw\"" );
//    /// ?
//    pvo_xml_end_group( ?, "AppendedData" );
//    pvo_xml_end_group( ?, "VTKFile" );
//}


int pvo_file_write( pvo_file_t fh ) {
//    write_pvtu( fh );
//    write_vtu ( fh );
    return 0;
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

