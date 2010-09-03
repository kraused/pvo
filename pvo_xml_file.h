
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_XML_FILE_H_INCLUDED
#define PVO_XML_FILE_H_INCLUDED 1

/// @file pvo_xml_file.h
/// XML Binary file. Since we incorporate binary data
/// files do not conform to the XML standard

#include "pvo_low_io.h"
#include "pvo_island.h"
#include "pvo_mpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/// pvo_xml_file: An xml file written in collective fashion
/// 
typedef struct pvo_xml_file {

    /// The version of the file in the form "X.Y". The
    /// default this is "1.0".
    char                        version[4];

    /// Pointer to the island
    const pvo_island_t*         island;

    /// The file handle
    /// This is an opaque handle for the file
    /// used by the (exchangeable) lower level
    /// driver.
    pvo_low_io_file_handle_t    f;

    /// Indentation level. This value is maintained
    /// internally
    int                         ilvl;

    /// Maximal indentation level for xml files.
#define PVO_XML_MAXDEPTH    8

    /// Tab space for indentation
    char                        itabs[PVO_XML_MAXDEPTH+1];

} * pvo_xml_file_t;


/// Create a new XML file. Writes to the xml file are always
/// collective for the whole island.
/// @param[in]  name    the filename
/// @param[in]  island  the attached island.
/// @param[in]  f       the lower level I/O handle
/// @param[out] fh      the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_create( const char*                name,
                         const pvo_island_t*        island,
                         pvo_low_io_file_handle_t   f,
                         pvo_xml_file_t*            fh );

/// Delete the XML file handle. Note that this does <b>not</b>
/// delete the file; create and delete are simply the standard
/// names used in libpvo to denote constructors and destructors.
/// A call to this function does close the file but does not
/// destroy the lower layer I/O file handle.
/// @param[inout]   fh  the file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_delete( pvo_xml_file_t fh );

/// Start a new element in the xml file
/// @param[in]  fh      the file handle
/// @param[in]  fmt     format: The result of sprintf() using this
///                     format will be the content of the tag
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_new_group( pvo_xml_file_t fh, const char* fmt, ... );

/// Mark the end of a group
/// @param[in]  fh      the file handle
/// @param[in]  fmt     format: The result of sprintf() using this
///                     format will be the content of the tag
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_end_group( pvo_xml_file_t fh, const char* fmt, ... );

/// Write a single element
/// @param[in]  fh      the file handle
/// @param[in]  fmt     format: The result of sprintf() using this
///                     format will be the content of the tag
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_write_element( pvo_xml_file_t fh, const char* fmt, ... );

/// Write ordered data into the file. The data written by each process
/// is ordered according to the rank in the island
/// @param[in]  fh          the file handle
/// @param[in]  buf         buffer space to write
/// @param[in]  count       number of items to write
/// @param[in]  datatype    the type of the data to write
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_write_ordered( pvo_xml_file_t fh, void* buf, int count, 
                                MPI_Datatype datatype );

/// Write data on a single process. All processes must pass in the
/// same values
/// @param[in]  fh          the file handle
/// @param[in]  buf         buffer space to write
/// @param[in]  count       number of items to write
/// @param[in]  datatype    the type of the data to write
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_write_single( pvo_xml_file_t fh, void* buf, int count,
                               MPI_Datatype datatype );

/// Get the lower level file handle. This function can be used e.g.,
/// to retrieve the handle for manual destruction.
/// @param[in]  fh          the file handle
/// @param[out] f           the lower layer file handle
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_xml_file_low_io_file_handle( pvo_xml_file_t fh,
                                     pvo_low_io_file_handle_t* f );

#ifdef __cplusplus
}
#endif

#endif

