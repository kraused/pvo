
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

    pvo_xml_file_end_group( f, "PPolyData" );
    pvo_xml_file_end_group( f, "VTKFile" );

    return err;
}

/// Write a vtp file
static int pvo_vtp_write_data( pvo_file_t self, pvo_xml_file_t f )
{
    int err = 0;
    pvo_vtp_file_t fh = (pvo_vtp_file_t )self;
    pvo_var_t* p;
    int nbytes;
    MPI_Datatype type;
    size_t offset = 0;
    int gnpoints, ibuf, jbuf, bpoints;
    size_t i;
    int* t;

    if( NULL == f->island ) {
        PVO_ERROR( "Invalid input: NULL == fh->base.cki." );
        goto fn_fail;
    }


    ibuf = fh->npoints;
    MPI_Allreduce( &ibuf, &gnpoints, 1, MPI_INT, MPI_SUM, f->island->comm );

    ibuf = fh->npoints;
    MPI_Exscan( &ibuf, &jbuf, 1, MPI_INT, MPI_SUM, f->island->comm );

    if(0 == f->island->rank )
        jbuf = 0;

    bpoints = jbuf;


    pvo_xml_file_new_group( f, "VTKFile type=\"PolyData\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "PolyData" );
    pvo_xml_file_new_group( f, "Piece NumberOfPoints=\"%d\" NumberOfVerts=\"%d\" NumberOfLines=\"%d\" NumberOfStrips=\"%d\" NumberOfPolys=\"%d\"", 
                               gnpoints, 1, 0, 0, 0 );

    pvo_xml_file_new_group( f, "PointData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;

        p->offset = offset;
        pvo_xml_file_write_element( f, "DataArray type=\"%s\" Name=\"%s\" NumberOfComponents=\"%d\" format=\"appended\" offset=\"%lu\"", pvo_var_type_names[p->type], p->name, p->ncomps, p->offset );
        offset += pvo_var_type_sizeof[p->type]*p->ncomps*gnpoints + 4; 
    }
    pvo_xml_file_end_group( f, "PointData" );

    pvo_xml_file_new_group( f, "CellData" );
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        PVO_ERROR( "CellData currently not supported." );

        p->offset = offset;
        pvo_xml_file_write_element( f, "DataArray type=\"%s\" Name=\"%s\" NumberOfComponents=\"%d\" format=\"appended\" offset=\"%lu\"", pvo_var_type_names[p->type], p->name, p->ncomps, p->offset );
        offset += pvo_var_type_sizeof[p->type]*p->ncomps*0 + 4;
    }
    pvo_xml_file_end_group( f, "CellData" );

    pvo_xml_file_new_group( f, "Points" );
    pvo_xml_file_write_element( f, "DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*gnpoints + 4;
    pvo_xml_file_end_group( f, "Points" );

    pvo_xml_file_new_group( f, "Verts" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"connectivity\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_INT32]*gnpoints + 4;

    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"offsets\" format=\"appended\" offset=\"%lu\"", offset );
    offset += pvo_var_type_sizeof[PVO_VAR_INT32]*1 + 4;

    pvo_xml_file_end_group( f, "Verts" );

    pvo_xml_file_new_group( f, "Lines" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"" );
    pvo_xml_file_end_group( f, "Lines" );

    pvo_xml_file_new_group( f, "Strips" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"" );
    pvo_xml_file_end_group( f, "Strips" );

    pvo_xml_file_new_group( f, "Polys" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"" );
    pvo_xml_file_write_element( f, "DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"" );
    pvo_xml_file_end_group( f, "Polys" );

    pvo_xml_file_end_group( f, "Piece" );
    pvo_xml_file_end_group( f, "PolyData" );

    pvo_xml_file_new_group( f, "AppendedData encoding=\"raw\"" );
    pvo_xml_file_write_single( f, "_", 1, MPI_CHAR );
   
    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;
    
        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*gnpoints;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void *)p->ptr, p->ncomps*fh->npoints, type );
    }

    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*0;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void* )p->ptr, p->ncomps*0, type );
    }

    /* Write point coordinates
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*gnpoints;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    pvo_var_type_mpi( PVO_VAR_FLOAT32, &type );
    pvo_xml_file_write_ordered( f, fh->pts, 3*fh->npoints, type );

    /* Write verts
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_INT32]*gnpoints;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    t = malloc(fh->npoints*sizeof(int));
    for( i = 0; i < fh->npoints; ++i )
        t[i] = i + bpoints;

    pvo_var_type_mpi( PVO_VAR_INT32, &type );
    pvo_xml_file_write_ordered( f, t, fh->npoints, type );

    free(t);

    nbytes = pvo_var_type_sizeof[PVO_VAR_INT32];
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    ibuf = gnpoints;
    pvo_xml_file_write_single ( f, &ibuf, 1, MPI_INT );

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
                       int64_t          npoints,
                       pvo_float3_t*    pts,
                       pvo_vtp_file_t*  fh )
{
    int err = 0;

    *fh = pvo_malloc( sizeof(struct pvo_vtp_file) );
    if( -1 == pvo_file_create( filename, 
                               PVO_FILE_BYTE_ORDER_MACHINE,
                               cki,
                               &(*fh)->base ))
        goto fn_fail;

    if( npoints < 0 || !pts ) {
        PVO_WARN( "Invalid input of some kind." );
        goto fn_fail;
    }

    (*fh)->npoints = npoints;
    (*fh)->pts     = pts;

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

