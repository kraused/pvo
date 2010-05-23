
/// vim: tabstop=4:expandtab:hlsearch

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

    /// The file handle.
    /// This is an opaque handle for the file
    /// used by the (exchangeable) lower level
    /// driver.
    pvo_xml_file_t              f;

    /// Write file
    int (*write)( struct pvo_file* self );
    
};

/// The pvo_file should be considered an internal datastructure. 
/// We provide the opaque pvo_file_t type for external use. The
/// library should provide all the necessary functions to access
/// members of the pvo_file structure
typedef struct pvo_file*    pvo_file_t;

///
/// @param[in]  filename    name of the file to open without
///                         extension. The filename should be
///                         the same on all processes.
/// @param[in]  bo          The byte order to use. On most systems
///                         this will be little endian (x86).
/// @param[in]  cki         A persistent cookie cna be used to pass
///                         information between different files.
/// @param[out] fh          the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_file_open( const char*           filename,
                   pvo_file_byte_order_t bo,
                   pvo_cookie_t          cki,
                   pvo_file_t*           fh );

/// Write to a file
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_file_write( pvo_file_t fh );

/// Close a file opened with pvo_file_open()
/// @param[in]  fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_file_close( pvo_file_t fh );


#ifdef __cplusplus
}
#endif

#endif

