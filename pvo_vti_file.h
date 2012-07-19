
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

#ifndef PVO_VTI_FILE_H_INCLUDED
#define PVO_VTI_FILE_H_INCLUDED 1

/// @file pvo_vti_file.h
/// Parallel VTK Output for VTI files

#include "pvo_file.h"

#ifdef __cplusplus
extern "C" {
#endif

/// pvo_vti_file: Derives from pvo_file
/// This file type can be used to write structured meshes
/// into the vti file format.
/// 
struct pvo_vti_file {

    /// Base 
    struct pvo_file base;

    /// Extent of the whole domain.
    int32_t         whole_extent[6];

    /// Number of ghost levels/overlap.
    int32_t         ghost_level;

    /// Origin of the data.
    float           origin[3];

    /// Spacing of the data.
    float           spacing[3];

    /// Extent of the local part of the mesh on
    /// this processing element.
    int32_t         local_extent[6];

    /// Extent of the local piece of the mesh.
    int32_t         piece_extent[6];

    /// List of all piece extents. These are
    /// required on the root processing element
    /// to write the <Piece> element.
    int32_t*        extents;

};

/// Allow for opaque handling of the pvo_vti_file type
typedef struct pvo_vti_file*    pvo_vti_file_t;
    

/// Open a new file.
///
/// @param[in]  filename        name of the file to open without
///                             extension. The filename should be
///                             the same on all processes.
/// @param[in]  cki             A persistent cookie can be used to pass
///                             information between different files.
/// @param[in]  whole_extent    Extent of the whole domain.
/// @param[in]  ghost_level     Number of ghost levels
/// @param[in]  origin          Origin of the data.
/// @param[in]  spacing         Spacing of the data.
/// @param[in]  local_extent    Extent of the local piece of the mesh
///                             on the calling processing element.
/// @param[in]  pts             Coordinates of the mesh nodes
/// @param[out] fh              the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_vti_file_open( const char*      filename,
                       pvo_cookie_t     cki,
                       const int32_t*   whole_extent,
                       int32_t          ghost_level,
                       const float*     origin,
                       const float*     spacing,
                       const int32_t*   local_extent,
                       pvo_vti_file_t*  fh );

/// Close a file opened with pvo_file_vti_open()
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_vti_file_close( pvo_vti_file_t fh );

#ifdef __cplusplus
}
#endif

#endif

