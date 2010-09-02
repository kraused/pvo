
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_VTP_FILE_H_INCLUDED
#define PVO_VTP_FILE_H_INCLUDED 1

/// @file pvo_vtp_file.h
/// Parallel VTK Output for VTP files

#include "pvo_file.h"

#ifdef __cplusplus
extern "C" {
#endif

/// pvo_vtp_file: Derives from pvo_file
/// This file type can be used to write polydata
/// into the vtp file format.
/// 
struct pvo_vtp_file {

    /// Base 
    struct pvo_file base;

};

/// Allow for opaque handling of the pvo_vtp_file type
typedef struct pvo_vtp_file*    pvo_vtp_file_t;
    

/// Open a new file.
///
/// @param[in]  filename    name of the file to open without
///                         extension. The filename should be
///                         the same on all processes.
/// @param[in]  cki         A persistent cookie can be used to pass
///                         information between different files.
/// @param[out] fh          the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_vtp_file_open( const char*      filename,
                       pvo_cookie_t     cki,
                       pvo_vtp_file_t*  fh );

/// Close a file opened with pvo_file_vtp_open()
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_vtp_file_close( pvo_vtp_file_t fh );

#ifdef __cplusplus
}
#endif

#endif

