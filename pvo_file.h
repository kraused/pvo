
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

#ifndef PVO_FILE_H_INCLUDED
#define PVO_FILE_H_INCLUDED 1

/// @file pvo_file.h
/// This file contains the central file
/// datastructure

#include "pvo_xml_file.h"
#include "pvo_var.h"
#include "pvo_cookie.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Byte order for the pvo file
typedef enum {
    PVO_FILE_BYTE_ORDER_MACHINE = -1, /// use the endianess of the
                                      /// machine we are running on
    PVO_FILE_BYTE_ORDER_LE      =  0, /// little endian byte order
    PVO_FILE_BYTE_ORDER_BE      =  1  /// big    endian byte order
} pvo_file_byte_order_t;

/// pvo_file: A single file
/// This structure represent a single pvo file.
///
/// The data written can be either a .pvtu or .vtu file depending
/// on the various tuning parameters (especially how many islands we
/// are using). Data is always written in binary ("raw" encoding) form.
struct pvo_file {

    /// The version of the file in the form "X.Y". The
    /// default this is "0.1".
    char                        version[3];

    /// The byte order used
    pvo_file_byte_order_t       bo;    

    /// VTK conforming string representation of the byte order
    char                        bo_str[32];

    /// The user provided cookie. The idea of the
    /// cookies is to seperate data which might be valid for
    /// different pvo_file instances (e.g., thinking about writing
    /// the same data for different timesteps) from the 
    /// actual file instance such that the information (and
    /// chached data) can be reused.
    ///
    /// Note that also the list of variables is part of the
    /// cookie rather than the pvo_file structure since is likely
    /// reusable.
    pvo_cookie_t                cki;

    /// Common prefix for all vtu and the pvtu file
    char                        name[128];

    ///  File suffix. This must be set by derived classes
    char                        suffix[8];

    /// Write a pvtx file. This function is implemented by
    /// derived structs as they depend on the format to use
    /// (e.g., unstructured meshes, poly data, ... )
    int (*write_meta)( struct pvo_file* self, pvo_xml_file_t f );

    /// Write the vtx file. 
    int (*write_data)( struct pvo_file* self, pvo_xml_file_t f );
    
};

/// The pvo_file should be considered an internal datastructure. 
/// We provide the opaque pvo_file_t type for external use. The
/// library should provide all the necessary functions to access
/// members of the pvo_file structure
typedef struct pvo_file*    pvo_file_t;

/// Open a file. Please note the differences in the handling of
/// the fh parameter: Since this class is used as the base class
/// for all pvo_XXX_file types we typically need to initialize
/// an instance of pvo_file which is already allocated (as part
/// of the instance of the derived type).
/// @param[in]  filename    name of the file to open without
///                         extension. The filename should be
///                         the same on all processes.
/// @param[in]  bo          The byte order to use. On most systems
///                         this will be little endian (x86).
/// @param[in]  cki         A persistent cookie cna be used to pass
///                         information between different files.
/// @param[inout]   fh      The file handle. Note that this is
///                         passed by value and hence assumed to
///                         be allready allocated.
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_file_create( const char*           filename,
                     pvo_file_byte_order_t bo,
                     pvo_cookie_t          cki,
                     pvo_file_t            fh );

/// Write to a file.
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_file_write( pvo_file_t fh );

/// Close a file opened with pvo_file_open()
/// @param[in]  fh  The file handle. The storage is <b>not</b>
///                 freed.
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_file_delete( pvo_file_t fh );

#ifdef __cplusplus
}
#endif

#endif

