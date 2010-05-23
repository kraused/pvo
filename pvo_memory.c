
/// vim: tabstop=4:expandtab:hlsearch:

#include "pvo_memory.h"
#include "pvo_report.h"

void* pvo_malloc( size_t n ) {
    void* p;

    if( NULL == ( p = malloc( n )))
        PVO_DIE( "failed to allocate %lu bytes.", n );

    return p;
}

void* pvo_calloc( size_t n, size_t k ) {
    void* p;

    if( NULL == ( p = calloc( n, k )))
        PVO_DIE( "failed to allocate %lu bytes.", n*k );

    return p;
}

void  pvo_free  ( void* p ) {
    free( p );
}

