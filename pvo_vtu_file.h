
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_VTU_FILE_H_INCLUDED
#define PVO_VTU_FILE_H_INCLUDED 1

/// @file pvo_vtu_file.h
/// Parallel VTK Output for VTU files

#include "pvo_file.h"

#ifdef __cplusplus
extern "C" {
#endif

/// List of supported cell types. We do not support
/// all the VTK cell types (even though we easily
/// could do if this is required).
///
/// It would be nice to get around this being necessary
/// but unfortunately the mapping from the number of
/// corners to the type itself is not one-to-one and
/// hence we need this additional information
///
typedef enum
{
    /// A single isolated vertex
    PVO_VTU_VERTEX        = 1,
    /// A line
    PVO_VTU_LINE          = 3,
    /// A triangle
    PVO_VTU_TRIANGLE      = 5,
    /// A quadrilateral
    PVO_VTU_QUADRILATERAL = 9,
    /// A tetrahedron
    PVO_VTU_TETRAHEDRON   = 10,
    /// A hexahedron
    PVO_VTU_HEXAHEDRON    = 12,
    /// A wedge
    PVO_VTU_WEDGE         = 13,
    /// A pyramid
    PVO_VTU_PYRAMID       = 14

} pvo_vtu_cell_type_t;

/// pvo_vtu_file: Derives from pvo_file
/// This file type can be used to write unstructured meshes
/// into the vtu file format.
///
/// It is assumed that the mesh (which might be finite-element
/// or finite-volume mesh, but also finite-differences) is 
/// partitioned by elements in a non-overlapping fashion. Hence,
/// some nodes are shared between the processes. We write these
/// nodes multiple times.
///
/// The connectivity graph is stored in CRS format (omitting the
/// third array which normally stores the values).
/// 
struct pvo_vtu_file {

    /// Base 
    struct pvo_file base;

    /// Number of nodes on the processor. This
    /// includes shared nodes
    int64_t         nnodes;

    /// Number of cells on the processor
    int64_t         ncells;

    /// Mesh node coordinates
    pvo_float3_t*   pts;

    /// The row pointers for the connectivity graph stored in
    /// CRS format. This array must have length nnodes+1
    /// We store these values as 32 bit integers to
    /// keep storage requirements low
    int32_t*        cia;

    /// The column indices for the connectivity graph
    /// We store these values as 32 bit integers to
    /// keep storage requirements low
    int32_t*        cja;

    /// Cell types
    uint8_t*        types;

};

/// Allow for opaque handling of the pvo_vtu_file type
typedef struct pvo_vtu_file*    pvo_vtu_file_t;
    

/// Open a new file.
///
/// To keep the memory requirements minimal, the arrays
/// pts, cia and cja are <b>not</b> copied. It is the 
/// responsibility of the user to ensure that these arrays
/// have a sufficiently long lifetime.
///
/// @param[in]  filename    name of the file to open without
///                         extension. The filename should be
///                         the same on all processes.
/// @param[in]  cki         A persistent cookie can be used to pass
///                         information between different files.
/// @param[in]  nnodes      The number of nodes
/// @param[in]  pts         Coordinates of the mesh nodes
/// @param[in]  ncells      The number of cells
/// @param[in]  cia         Row pointer for connectivity graph
/// @param[in]  cja         Column indices for connectivity graph
/// @param[in]  types       The cell types. Ideally this would be
///                         an array of type pvo_vtu_cell_type_t ,
///                         but by requiring it to be of type uint8_t
///                         we can get around an additional memory
///                         copy.
/// @param[out] fh          the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_vtu_file_open( const char*      filename,
                       pvo_cookie_t     cki,
                       int64_t          nnodes,
                       pvo_float3_t*    pts,
                       int64_t          ncells,
                       int32_t*         cia,
                       int32_t*         cja,
                       uint8_t*         types,
                       pvo_vtu_file_t*  fh );

/// Close a file opened with pvo_file_vtu_open()
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_vtu_file_close( pvo_vtu_file_t fh );

#ifdef __cplusplus
}
#endif

#endif

