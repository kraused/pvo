
/// vim: tabstop=4:expandtab:hlsearch:

#ifndef PVO_REPORT_H_INCLUDED
#define PVO_REPORT_H_INCLUDED 1

#ifdef __cplusplus
extern "C" {
#endif

/// Report an error together with its position
void pvo_error( const char* filename, const char* funct, long line, 
                const char* err, ... );
/// Report an error together with its position and kill the application
void pvo_die  ( const char* filename, const char* funct, long line, 
                const char* err, ... );
/// Display a warning together with its position
void pvo_warn ( const char* filename, const char* funct, long line, 
                const char* err, ... );
/// Display an information for the user. This is intended for reporting
/// information which is useful for developers to spot (e.g., performance)
/// problems. It is not meant for printing output for users.
void pvo_info ( const char* filename, const char* funct, long line, 
                const char* err, ... );

/// Macro wrappers which provide the filename, funct and line
/// values
#define PVO_ERROR( err, ... )   pvo_error( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )
#define PVO_DIE( err, ... )     pvo_die  ( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )
#define PVO_WARN( err, ... )    pvo_warn ( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )
#define PVO_INFO( err, ... )    pvo_info ( __FILE__, __func__, __LINE__, \
                                           err , ## __VA_ARGS__ )

#ifdef __cplusplus
}
#endif

#endif

