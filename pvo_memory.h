
/// vim: tabstop=4:expandtab:hlsearch:

#ifndef PVO_MEMORY_H_INCLUDED
#define PVO_MEMORY_H_INCLUDED 1

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Allocate memory
void* pvo_malloc( size_t n );
void* pvo_calloc( size_t n, size_t k );

/// Free memory
void  pvo_free  ( void* p );

#ifdef __cplusplus
}
#endif

#endif

