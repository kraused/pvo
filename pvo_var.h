
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_VAR_H_INCLUDED
#define PVO_VAR_H_INCLUDED 1

/// @file pvo_var.h
/// Contains metadata description structure
/// for variables

#include "pvo_types.h"
#include "pvo_MPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Enum values representing the datatypes
typedef enum {
    PVO_VAR_INT32   = 0,    /// 32 bit integer data

/// Note that VTK by default doesn't support 64 bit
/// integer types. This should be considered when
/// writing these variables since e.g. Paraview might
/// not be able to read the data
    PVO_VAR_INT64   = 1,    /// 64 bit integer data

    PVO_VAR_FLOAT32 = 2,    /// 32 bit floating point data
    PVO_VAR_FLOAT64 = 3,    /// 64 bit floating point data

    PVO_VAR_UINT8   = 4     /// 8 bit unsigned int
} pvo_var_type_t;

/// Mapping from variable types to string representations
/// for output
extern const char* pvo_var_type_names[5];

/// Mapping from variable types to the size of the type
extern size_t pvo_var_type_sizeof[5];

/// Query the correct mpi type for the pvo datatype
/// @param[in]  pvo_type    the pvo type identifier
/// @param[out] mpi_type    the MPI datatype corresponding to pvo_type
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_var_type_mpi(int pvo_type, MPI_Datatype* mpi_type);

/// Enum values to specify the "group" of data.
/// Basically pvo supports two kinds of data (as provided
/// by the vtu format): nodal data and cell data.
typedef enum {
    PVO_VAR_NODEDATA = 0,   /// Data attached to nodes
    PVO_VAR_CELLDATA = 1    /// Data attached to cells
} pvo_var_group_t;

/// Description of a variable
typedef struct pvo_var {

    /// The group of the data. This
    /// determines the length of the input
    /// array which must equal either the
    /// number of nodes or the number of
    /// cells in the mesh
    pvo_var_group_t     grp;

    /// Datatype. This value must match
    /// the type of the input array. 
    /// Conversion of data is not supported
    pvo_var_type_t      type;

    /// Number of components of the data per
    /// entry (e.g., per node).
    int                 ncomps;

    /// Name of the data. The length is
    /// restricted to 127 chars.
    char                name[128];

    /// Pointer to the data. We assume that the
    /// user stores the data in a contiguous buffer
    ///
    /// FIXME
    /// Currently the only way to reassign this ptr
    /// would be to delete the variable and create a
    /// new one
    const void*         ptr;

    /// Next pointer in a linked list
    struct pvo_var*     next;

    /// Offset value. This is used to identify
    /// data in the big "appended" data session
    /// The value is assigned internally
    size_t              offset;

} pvo_var_t;


/// Create a new variable
/// @param[in]  grp     the group of the data
/// @param[in]  type    the type  of the data
/// @param[in]  ncomps  number of components
/// @param[in]  name    name of the variable. The maximal
///                     length is 127 chars.
/// @param[in]  ptr     Pointer to the variable content in memory
/// @param[out] var     on exit: initialized variable
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_var_create( pvo_var_group_t     grp,
                    pvo_var_type_t      type,
                    int                 ncomps,
                    const char*         name,
                    const void*         ptr,
                    pvo_var_t*          var );

/// Delete a variable
/// @param[inout]   var the variable
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_var_delete( pvo_var_t* var );

#ifdef __cplusplus
}
#endif

#endif

