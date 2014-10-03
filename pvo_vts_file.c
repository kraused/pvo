
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

#include "pvo_vts_file.h"
#include "pvo_memory.h"
#include "pvo_report.h"
#include "pvo_utils.h"

#include <stdio.h>
#include <string.h>

/// Write a pvts file
static int pvo_vts_write_meta( pvo_file_t self, pvo_xml_file_t f )
{
    int        i, err = 0;
    pvo_var_t* p;
    pvo_vts_file_t fh = (pvo_vts_file_t )self;

    /// Warning: Do not use f->island here, since we use
    ///          the PVO_LOW_IO_SINGLE and hence f->island
    ///          is NULL

    pvo_xml_file_new_group( f, "VTKFile type=\"PStructuredGrid\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "PStructuredGrid WholeExtent=\"%d %d %d %d %d %d\" "
                               "GhostLevel=\"%d\" ",
                               fh->whole_extent[0],
                               fh->whole_extent[1],
                               fh->whole_extent[2],
                               fh->whole_extent[3],
                               fh->whole_extent[4],
                               fh->whole_extent[5],
                               fh->ghost_level );

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
        pvo_xml_file_write_element( f, "Piece Extent=\"%d %d %d %d %d %d\" Source=\"%s-%d.%s\"",
                                       fh->extents[0 + 6*i],
                                       fh->extents[1 + 6*i],
                                       fh->extents[2 + 6*i],
                                       fh->extents[3 + 6*i],
                                       fh->extents[4 + 6*i],
                                       fh->extents[5 + 6*i],
                                       pvo_basename(self->name), i, self->suffix );

    pvo_xml_file_end_group( f, "PStructuredGrid" );
    pvo_xml_file_end_group( f, "VTKFile" );

    return err;
}

/// Write a vts file
static int pvo_vts_write_data( pvo_file_t self, pvo_xml_file_t f )
{
    int err = 0;
    pvo_vts_file_t fh = (pvo_vts_file_t )self;
    size_t offset = 0;
    int ibuf[2], jbuf[2];
    int nnodes, ncells, gnnodes, gncells;
    pvo_var_t* p;
    int nbytes;
    MPI_Datatype type;
    size_t i;

    if( NULL == f->island ) {
        PVO_ERROR( "Invalid input: NULL == fh->base.cki." );
        goto fn_fail;
    }

    ncells = ( fh->local_extent[1] - fh->local_extent[0] )*
             ( fh->local_extent[3] - fh->local_extent[2] )*
             ( fh->local_extent[5] - fh->local_extent[4] );
    nnodes = ( fh->local_extent[1] - fh->local_extent[0] + 1 )*
             ( fh->local_extent[3] - fh->local_extent[2] + 1 )*
             ( fh->local_extent[5] - fh->local_extent[4] + 1 );

    ibuf[0] = nnodes;
    ibuf[1] = ncells;
    if( MPI_Allreduce( MPI_IN_PLACE, ibuf, 2, MPI_INT, MPI_SUM, f->island->comm )) {
        PVO_WARN( "MPI_Allreduce failed." );
        goto fn_fail;
    }

    gnnodes = ibuf[0];
    gncells = ibuf[1];

    pvo_xml_file_new_group( f, "VTKFile type=\"StructuredGrid\" "
                               "version=\"0.1\" "
                               "byte_order=\"%s\"", self->bo_str );
    pvo_xml_file_new_group( f, "StructuredGrid WholeExtent=\"%d %d %d %d %d %d\" ",
                               fh->piece_extent[0],
                               fh->piece_extent[1],
                               fh->piece_extent[2],
                               fh->piece_extent[3],
                               fh->piece_extent[4],
                               fh->piece_extent[5] );
    pvo_xml_file_new_group( f, "Piece Extent=\"%d %d %d %d %d %d\"",
                               fh->piece_extent[0],
                               fh->piece_extent[1],
                               fh->piece_extent[2],
                               fh->piece_extent[3],
                               fh->piece_extent[4],
                               fh->piece_extent[5] );

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

    pvo_xml_file_end_group( f, "Piece" );
    pvo_xml_file_end_group( f, "StructuredGrid" );

    pvo_xml_file_new_group( f, "AppendedData encoding=\"raw\"" );
    pvo_xml_file_write_single( f, "_", 1, MPI_CHAR );

    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_NODEDATA != p->grp )
            continue;

        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*gnnodes;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void *)p->ptr, p->ncomps*nnodes, type );
    }

    for( p = self->cki->vlist; p; p = p->next ) {
        if( PVO_VAR_CELLDATA != p->grp )
            continue;

        nbytes = pvo_var_type_sizeof[p->type]*p->ncomps*gncells;
        pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

        pvo_var_type_mpi(p->type, &type);
        pvo_xml_file_write_ordered( f, (void* )p->ptr, p->ncomps*ncells, type );
    }

    /* Write point coordinates
     */
    nbytes = pvo_var_type_sizeof[PVO_VAR_FLOAT32]*3*gnnodes;
    pvo_xml_file_write_single ( f, &nbytes, 1, MPI_INT );

    pvo_var_type_mpi( PVO_VAR_FLOAT32, &type );
    pvo_xml_file_write_ordered( f, fh->pts, 3*nnodes, type );

    pvo_xml_file_end_group( f, "AppendedData" );
    pvo_xml_file_end_group( f, "VTKFile" );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

/* Compute piece_extent by merging the local_extents from all
 * processing elements in the island.
 */
static void compute_piece_extent( pvo_cookie_t      cki,
                                  const int32_t*    local_extent,
                                  int32_t*          piece_extent )
{
    int t1[3], t2[3];


    t1[0] = local_extent[0];
    t1[1] = local_extent[2];
    t1[2] = local_extent[4];
    if( MPI_Reduce( t1, t2, 3, MPI_INTEGER, MPI_MIN, 0, cki->island.comm ) )
        PVO_DIE( "MPI_Reduce failed." );
    piece_extent[0] = t2[0];
    piece_extent[2] = t2[1];
    piece_extent[4] = t2[2];

    t1[0] = local_extent[1];
    t1[1] = local_extent[3];
    t1[2] = local_extent[5];
    if( MPI_Reduce( t1, t2, 3, MPI_INTEGER, MPI_MAX, 0, cki->island.comm ) )
        PVO_DIE( "MPI_Reduce failed." );
    piece_extent[1] = t2[0];
    piece_extent[3] = t2[1];
    piece_extent[5] = t2[2];
}

/* Gather the piece_extents on the root processing element.
 *
 * We implicitly assume that the PVO_COMM_WORLD root processing
 * element is also the root processing element of the local communicator
 * of the first island.
 */
static void gather_extents_on_root( pvo_cookie_t      cki,
                                    const int32_t*    piece_extent,
                                    int32_t*          extents )
{
    int color;
    MPI_Comm comm;

    color = ( 0 == cki->island.rank ) ? 1 : MPI_UNDEFINED;
    if( MPI_Comm_split( PVO_COMM_WORLD, color,
                        pvo_world_rank(),
                        &comm ))
        PVO_DIE( "MPI_Comm_split failed." );

    if(1 == color) {
        if( MPI_Gather( (void *)piece_extent, 6, MPI_INTEGER, extents, 6, MPI_INTEGER, 0, comm ))
            PVO_DIE( "MPI_Gather failed." );

        if( MPI_Comm_free( &comm ))
            PVO_WARN( "MPI_Comm_free failed." );
    }
}


int pvo_vts_file_open( const char*      filename,
                       pvo_cookie_t     cki,
                       const int32_t*   whole_extent,
                       int32_t          ghost_level,
                       const int32_t*   local_extent,
                       pvo_float3_t*    pts,
                       pvo_vts_file_t*  fh )
{
    int err = 0;

    *fh = pvo_malloc( sizeof(struct pvo_vts_file) );
    if( -1 == pvo_file_create( filename,
                               PVO_FILE_BYTE_ORDER_MACHINE,
                               cki,
                               &(*fh)->base ))
        goto fn_fail;

    if( NULL == whole_extent || ghost_level < 0 || NULL == local_extent || NULL == pts ) {
        PVO_WARN( "Invalid input of some kind." );
        goto fn_fail;
    }

    /* Since the islands do not respect the structured mesh
     * layout (they could if the user passes in the correct
     * color) we do not allow I/O with more than one islands
     * or with islands containing more than one processing
     * element.
     *
     * TODO Support 1 == cki->island.nislands.
     */
    if( cki->island.nislands != pvo_world_size() ) {
        PVO_WARN( "Only single-file I/O is currently supported. "
                  "Please correct the color passed to pvo_cookie_create." );
        goto fn_fail;
    }

    memcpy( (*fh)->whole_extent, whole_extent, 6*sizeof(int32_t) );
    (*fh)->ghost_level = ghost_level;
    memcpy( (*fh)->local_extent, local_extent, 6*sizeof(int32_t) );
    (*fh)->pts         = pts;

    compute_piece_extent( cki,
                          (*fh)->local_extent,
                          (*fh)->piece_extent );

    if( 0 == pvo_world_rank() ) {
        (*fh)->extents = pvo_malloc( 6*cki->island.nislands*sizeof(int32_t) );
    } else {
        (*fh)->extents = NULL;
    }
    gather_extents_on_root( cki,
                            (*fh)->piece_extent,
                            (*fh)->extents );

    snprintf( (*fh)->base.suffix, sizeof((*fh)->base.suffix), "vts" );

    (*fh)->base.write_meta = pvo_vts_write_meta;
    (*fh)->base.write_data = pvo_vts_write_data;

fn_exit:
    return err;
fn_fail:
    free( *fh );
    err = -1;
    goto fn_exit;
}

int pvo_vts_file_close( pvo_vts_file_t fh )
{
    int err = 0;

    if( 0 == pvo_world_rank() ) {
        pvo_free( fh->extents );
    }

    if( -1 == pvo_file_delete( &fh->base ))
        goto fn_fail;

    pvo_free( fh );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

