
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_var.h"
#include "pvo_report.h"

#include <string.h>

const char* pvo_var_type_names[5] = 
{
    [PVO_VAR_INT32]   = "Int32",
    [PVO_VAR_INT64]   = "Int64",
    [PVO_VAR_FLOAT32] = "Float32",
    [PVO_VAR_FLOAT64] = "Float64",
    [PVO_VAR_UINT8]   = "UInt8"
};

size_t pvo_var_type_sizeof[5] =
{
    [PVO_VAR_INT32]   = sizeof(int32_t),
    [PVO_VAR_INT64]   = sizeof(int64_t),
    [PVO_VAR_FLOAT32] = sizeof(float),
    [PVO_VAR_FLOAT64] = sizeof(double),
    [PVO_VAR_UINT8]   = sizeof(uint8_t)
};


int pvo_var_type_mpi(int pvo_type, MPI_Datatype* mpi_type)
{
    int err = 0;

    switch(pvo_type)
    {
    case PVO_VAR_INT32:
        *mpi_type = MPI_INT;
        break;
    case PVO_VAR_INT64:
        *mpi_type = MPI_LONG;   // FIXME MPI_INT64 for MPI 2.2
        break;
    case PVO_VAR_FLOAT32:
        *mpi_type = MPI_FLOAT;
        break;
    case PVO_VAR_FLOAT64:
        *mpi_type = MPI_DOUBLE;
        break;
    case PVO_VAR_UINT8:
        *mpi_type = MPI_BYTE;
        break;
    default:
        goto fn_fail;
    }

fn_exit:
    return err;
fn_fail:
    err = 1;
    goto fn_exit;
}

int pvo_var_create( pvo_var_group_t     grp,
                    pvo_var_type_t      type,
                    int                 ncomps,
                    const char*         name,
                    const void*         ptr,
                    pvo_var_t*          var )
{
    var->grp    = grp;
    var->type   = type;
    var->ncomps = ncomps;
    strncpy( var->name, name, 127 );
    var->ptr    = ptr;

    var->next   = 0x0;
    var->offset = 0;

    return 0;
}

int pvo_var_delete( pvo_var_t* var )
{
    return 0;
}

