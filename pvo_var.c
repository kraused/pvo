
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

