
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_config.h"

#include "pvo_vtp_file.h"
#include "pvo_memory.h"
#include "pvo_report.h"

#include <stdio.h>
#include <string.h>

/// Write a pvtp file
static int pvo_vtp_write_meta( pvo_file_t self, pvo_xml_file_t f )
{
    int        i, err = 0;
    pvo_var_t* p;

    /// Warning: Do not use f->island here, since we use
    ///          the PVO_LOW_IO_SINGLE and hence f->island
    ///          is NULL

    pvo_xml_file_new_group( f, "VTKFile type=\"PPolyData\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "PPolyData" );

    pvo_xml_file_new_group( f, "PPointData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;

        pvo_xml_file_write_element( f, "PDataArray type=\"%s\" Name=\"%s\" "
                                       "NumberOfComponents=\"%d\"",
                                       pvo_var_type_names[p->type], 
                                       p->name, p->ncomps );
    }
    pvo_xml_file_end_group( f, "PPointData" );

    pvo_xml_file_new_group( f, "PCellData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        pvo_xml_file_write_element( f, "PDataArray type=\"%s\" Name=\"%s\" "
                                       "NumberOfComponents=\"%d\"",
                                       pvo_var_type_names[p->type], 
                                       p->name, p->ncomps );
    }
    pvo_xml_file_end_group( f, "PCellData" );

    pvo_xml_file_new_group( f, "PPoints" );
    pvo_xml_file_write_element( f, "PDataArray type=\"Float32\" NumberOfComponents=\"3\"" );
    pvo_xml_file_end_group( f, "PPoints" );

    for( i = 0; i < self->cki->island.nislands; ++i )
        pvo_xml_file_write_element( f, "Piece Source=\"%s-%d.%s\"",
                                       self->name, i, self->suffix );

    pvo_xml_file_end_group( f, "PUnstructuredGrid" );
    pvo_xml_file_end_group( f, "VTKFile" );

    return err;
}

/// Write a vtp file
static int pvo_vtp_write_data( pvo_file_t self, pvo_xml_file_t f )
{
    int err = 0;
    pvo_var_t* p;
    int nbytes;
    MPI_Datatype type;
    size_t offset = 0;

    if( NULL == f->island ) {
        PVO_ERROR( "Invalid input: NULL == fh->base.cki." );
        goto fn_fail;
    }


    pvo_xml_file_new_group( f, "VTKFile type=\"PolyData\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "PolyData" );
    pvo_xml_file_new_group( f, "Piece NumberOfPoints=\"%d\" NumberOfVerts=\"%d\" NumberOfLines=\"%d\" NumberOfStrips=\"%d\" NumberOfPolys=\"%d\"", 
                               0, 0, 0, 0, 0 );

    pvo_xml_file_new_group( f, "PointData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;

        p->offset = offset;
        pvo_xml_file_write_element( f, "DataArray type=\"%s\" Name=\"%s\" NumberOfComponents=\"%d\" format=\"appended\" offset=\"%lu\"", pvo_var_type_names[p->type], p->name, p->ncomps, p->offset );
        offset += pvo_var_type_sizeof[p->type]*p->ncomps*0 + 4; 
    }
    pvo_xml_file_end_group( f, "PointData" );

    pvo_xml_file_new_group( f, "CellData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        p->offset = offset;
        pvo_xml_file_write_element( f, "DataArray type=\"%s\" Name=\"%s\" NumberOfComponents=\"%d\" format=\"appended\" offset=\"%lu\"", pvo_var_type_names[p->type], p->name, p->ncomps, p->offset );
        offset += pvo_var_type_sizeof[p->type]*p->ncomps*0 + 4;
    }
    pvo_xml_file_end_group( f, "CellData" );

    pvo_xml_file_new_group( f, "Points" );
    pvo_xml_file_write_element( f, "DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*0 + 4;
    pvo_xml_file_end_group( f, "Points" );

    pvo_xml_file_new_group( f, "Verts" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"connectivity\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_INT32]*0 + 4;

    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"offsets\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_INT32]*0 + 4;

    pvo_xml_file_end_group( f, "Verts" );

    pvo_xml_file_new_group( f, "Lines" );
    pvo_xml_file_end_group( f, "Lines" );

    pvo_xml_file_new_group( f, "Strips" );
    pvo_xml_file_end_group( f, "Strips" );

    pvo_xml_file_new_group( f, "Polys" );
    pvo_xml_file_end_group( f, "Polys" );

    pvo_xml_file_end_group( f, "Piece" );
    pvo_xml_file_end_group( f, "PolyData" );

    pvo_xml_file_new_group( f, "AppendedData encoding=\"raw\"" );
    pvo_xml_file_write_single( f, "_", 1, MPI_CHAR );
   
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;
    
        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*0;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void *)p->ptr, p->ncomps*0, type );
    }

    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*0;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void* )p->ptr, p->ncomps*0, type );
    }

//    /* Write point coordinates
//     */
//    nbytes = pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*0;
//    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );
//
//    pvo_var_type_mpi( PVO_VAR_FLOAT32, &type );
//    pvo_xml_file_write_ordered( f, fh->pts, 3*0, type );
//
//    /* Bottom index for the local process. This value can be added to the local
//     * node or cell index to get unique identifiers (within the island
//     */
//
//
//    /* Write connectivity
//     */
//   nbytes = pvo_var_type_sizeof[PVO_VAR_INT32]*0;
//    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );
//
//    for( i = 0; i < fh->cia[0]; ++i )
//        fh->cja[i] += bnodes;
//
//    pvo_var_type_mpi( PVO_VAR_INT32, &type );
//    pvo_xml_file_write_ordered( f, fh->cja, fh->cia[0], type );
//    
//    for( i = 0; i < fh->cia[0]; ++i )
//        fh->cja[i] -= bnodes;
//
//    /* Write offsets
//     */
//    nbytes = pvo_var_type_sizeof[PVO_VAR_INT32]*0;
//    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );
//
//    for( i = 1; i <= 0; ++i )
//        fh->cia[i] += bnnz;
//
//    pvo_var_type_mpi( PVO_VAR_INT32, &type );
//    pvo_xml_file_write_ordered( f, fh->cia+1, 0, type );
//
//    for( i = 1; i <= 0; ++i )
//        fh->cia[i] -= bnnz;
//
//    /* Write types
//     */
//    nbytes = pvo_var_type_sizeof[PVO_VAR_UINT8]*0;
//    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );
//
//    /* In order to be able to call pvo_xml_file_write_ordered() we need to have
//     * the full array in memory ...
//     */
//    t = malloc(0*sizeof(uint8_t));
//    for( i = 0; i < 0; ++i )
//        pvo_vtp_cell_type( fh->cia[i+1]-fh->cia[i], &t[i] );
//
//    pvo_var_type_mpi( PVO_VAR_UINT8, &type );
//    pvo_xml_file_write_ordered( f, t, 0, type );
//
//    free( t );

    pvo_xml_file_end_group( f, "AppendedData" );
    pvo_xml_file_end_group( f, "VTKFile" );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}


int pvo_vtp_file_open( const char*      filename,
                       pvo_cookie_t     cki,
                       pvo_vtp_file_t*  fh )
{
    int err = 0;

    *fh = pvo_malloc( sizeof(struct pvo_vtp_file) );
    if( -1 == pvo_file_create( filename, 
                               PVO_FILE_BYTE_ORDER_MACHINE,
                               cki,
                               &(*fh)->base ))
        goto fn_fail;

    if( 0 ) {
        PVO_WARN( "Invalid input of some kind." );
        goto fn_fail;
    }

    snprintf( (*fh)->base.suffix, sizeof((*fh)->base.suffix), "vtp" );

    (*fh)->base.write_meta = pvo_vtp_write_meta;
    (*fh)->base.write_data = pvo_vtp_write_data;

fn_exit:
    return err;
fn_fail:
    free( *fh );
    err = -1;
    goto fn_exit;
}

int pvo_vtp_file_close( pvo_vtp_file_t fh )
{
    int err = 0;

    if( -1 == pvo_file_delete( &fh->base ))
        goto fn_fail;

    pvo_free( fh );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

