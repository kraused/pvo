
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_config.h"

#include "pvo_vtu_file.h"
#include "pvo_memory.h"
#include "pvo_report.h"

#include <stdio.h>
#include <string.h>

/// Write a pvtu file
static int pvo_vtu_write_meta( pvo_file_t self, pvo_xml_file_t f )
{
    int        i, err = 0;
    pvo_var_t* p;

    /// Warning: Do not use f->island here, since we use
    ///          the PVO_LOW_IO_SINGLE and hence f->island
    ///          is NULL

    pvo_xml_file_new_group( f, "VTKFile type=\"PUnstructuredGrid\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "PUnstructuredGrid" );

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

/// Convert from the number of corners to the VTK type
static int pvo_vtu_cell_type( int ncorners, uint8_t* type )
{
    int err = 0;

    switch( ncorners )
    {
    case 3:
        *type = 5;
        break;
    case 8:
        *type = 12;
        break;
    case 4:
        *type = 9 | 10; /* FIXME The mapping is not unique! We need additional
                                 information about the element. Probably we can
                                 check if the points are coplanar? */
    default:
        PVO_ERROR( "Don't know how to handle ncorners = %d.", ncorners );
        goto fn_fail;
    }

fn_exit:
    return err;
fn_fail:
    err = 1;
    goto fn_exit;
}

/// Write a vtu file
static int pvo_vtu_write_data( pvo_file_t self, pvo_xml_file_t f )
{
    int err = 0;
    pvo_vtu_file_t fh = (pvo_vtu_file_t )self;
    int ibuf[3], jbuf[3];
    size_t offset = 0;
    int gnnodes, gncells, gnnz, bnodes, bnnz;
    pvo_var_t* p;
    int nbytes;
    MPI_Datatype type;
    size_t i;
    uint8_t* t;

    if( NULL == f->island ) {
        PVO_ERROR( "Invalid input: NULL == fh->base.cki." );
        goto fn_fail;
    }


    ibuf[0] = fh->nnodes;
    ibuf[1] = fh->ncells;
    ibuf[2] = fh->cia[fh->ncells];
    MPI_Allreduce( MPI_IN_PLACE, ibuf, 3, MPI_INT, MPI_SUM, f->island->comm );

    gnnodes = ibuf[0];
    gncells = ibuf[1];
    gnnz    = ibuf[2];


    /* Bottom index for the local process. This value can be added to the local
     * node or cell index to get unique identifiers (within the island
     */
    ibuf[0] = fh->nnodes;
    ibuf[1] = fh->cia[fh->ncells];

    MPI_Exscan( ibuf, jbuf, 2, MPI_INT, MPI_SUM, f->island->comm );

    if( 0 == f->island->rank ) {
        jbuf[0] = 0;
        jbuf[1] = 0;
    }

    bnodes = jbuf[0];
    bnnz   = jbuf[1];

    pvo_xml_file_new_group( f, "VTKFile type=\"UnstructuredGrid\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "UnstructuredGrid" );
    pvo_xml_file_new_group( f, "Piece NumberOfPoints=\"%d\" NumberOfCells=\"%d\"", 
                               gnnodes, gncells );

    pvo_xml_file_new_group( f, "PointData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;

        p->offset = offset;
        pvo_xml_file_write_element( f, "DataArray type=\"%s\" Name=\"%s\" NumberOfComponents=\"%d\" format=\"appended\" offset=\"%lu\"", pvo_var_type_names[p->type], p->name, p->ncomps, p->offset );
        offset += pvo_var_type_sizeof[p->type]*p->ncomps*gnnodes + 4; 
    }
    pvo_xml_file_end_group( f, "PointData" );

    pvo_xml_file_new_group( f, "CellData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        p->offset = offset;
        pvo_xml_file_write_element( f, "DataArray type=\"%s\" Name=\"%s\" NumberOfComponents=\"%d\" format=\"appended\" offset=\"%lu\"", pvo_var_type_names[p->type], p->name, p->ncomps, p->offset );
        offset += pvo_var_type_sizeof[p->type]*p->ncomps*gncells + 4;
    }
    pvo_xml_file_end_group( f, "CellData" );

    pvo_xml_file_new_group( f, "Points" );
    pvo_xml_file_write_element( f, "DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*gnnodes + 4;
    pvo_xml_file_end_group( f, "Points" );

    pvo_xml_file_new_group( f, "Cells" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"connectivity\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_INT32]*gnnz + 4;

    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"offsets\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_INT32]*gncells + 4;

    pvo_xml_file_write_element( f, "DataArray type=\"UInt8\" Name=\"types\" format=\"appended\" offset=\"%lu\"", offset );
    pvo_xml_file_end_group( f, "Cells" );

    pvo_xml_file_end_group( f, "Piece" );
    pvo_xml_file_end_group( f, "UnstructuredGrid" );

    pvo_xml_file_new_group( f, "AppendedData encoding=\"raw\"" );
    pvo_xml_file_write_single( f, "_", 1, MPI_CHAR );
   
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;
    
        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*gnnodes;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void *)p->ptr, p->ncomps*fh->nnodes, type );
    }

    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*gncells;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void* )p->ptr, p->ncomps*fh->ncells, type );
    }

    /* Write point coordinates
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*gnnodes;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    pvo_var_type_mpi( PVO_VAR_FLOAT32, &type );
    pvo_xml_file_write_ordered( f, fh->pts, 3*fh->nnodes, type );

    /* Bottom index for the local process. This value can be added to the local
     * node or cell index to get unique identifiers (within the island
     */


    /* Write connectivity
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_INT32]*gnnz;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    for( i = 0; i < fh->cia[fh->ncells]; ++i )
        fh->cja[i] += bnodes;

    pvo_var_type_mpi( PVO_VAR_INT32, &type );
    pvo_xml_file_write_ordered( f, fh->cja, fh->cia[fh->ncells], type );
    
    for( i = 0; i < fh->cia[fh->ncells]; ++i )
        fh->cja[i] -= bnodes;

    /* Write offsets
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_INT32]*gncells;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    for( i = 1; i <= fh->ncells; ++i )
        fh->cia[i] += bnnz;

    pvo_var_type_mpi( PVO_VAR_INT32, &type );
    pvo_xml_file_write_ordered( f, fh->cia+1, fh->ncells, type );

    for( i = 1; i <= fh->ncells; ++i )
        fh->cia[i] -= bnnz;

    /* Write types
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_UINT8]*gncells;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    /* In order to be able to call pvo_xml_file_write_ordered() we need to have
     * the full array in memory ...
     */
    t = malloc(fh->ncells*sizeof(uint8_t));
    for( i = 0; i < fh->ncells; ++i )
        pvo_vtu_cell_type( fh->cia[i+1]-fh->cia[i], &t[i] );

    pvo_var_type_mpi( PVO_VAR_UINT8, &type );
    pvo_xml_file_write_ordered( f, t, fh->ncells, type );

    free( t );

    pvo_xml_file_end_group( f, "AppendedData" );
    pvo_xml_file_end_group( f, "VTKFile" );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}


int pvo_vtu_file_open( const char*      filename,
                       pvo_cookie_t     cki,
                       int64_t          nnodes,
                       pvo_float3_t*    pts,
                       int64_t          ncells,
                       int32_t*         cia,
                       int32_t*         cja,
                       pvo_vtu_file_t*  fh )
{
    int err = 0;

    *fh = pvo_malloc( sizeof(struct pvo_vtu_file) );
    if( -1 == pvo_file_create( filename, 
                               PVO_FILE_BYTE_ORDER_MACHINE,
                               cki,
                               &(*fh)->base ))
        goto fn_fail;

    if( nnodes < 0 || ncells < 0 || NULL == pts || NULL == cia || NULL == cja ) {
        PVO_WARN( "Invalid input of some kind." );
        goto fn_fail;
    }

    (*fh)->nnodes = nnodes;
    (*fh)->ncells = ncells;
    (*fh)->pts    = pts;
    (*fh)->cia    = cia;
    (*fh)->cja    = cja;

    snprintf( (*fh)->base.suffix, sizeof((*fh)->base.suffix), "vtu" );

    (*fh)->base.write_meta = pvo_vtu_write_meta;
    (*fh)->base.write_data = pvo_vtu_write_data;

fn_exit:
    return err;
fn_fail:
    free( *fh );
    err = -1;
    goto fn_exit;
}

int pvo_vtu_file_close( pvo_vtu_file_t fh )
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

