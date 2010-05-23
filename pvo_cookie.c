
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_cookie.h"
#include "pvo_report.h"
#include "pvo_memory.h"

#include <iso646.h>

int pvo_cookie_create( int color, pvo_cookie_t* cookie ) {
    int err = 0;

    *cookie = pvo_calloc( 1, sizeof(struct pvo_cookie) );
    err     = pvo_create_island( color, &(*cookie)->island );
    
    return err;
}

int pvo_cookie_delete( pvo_cookie_t cookie ) {
    int err;

    if( NULL == cookie ) {
        PVO_WARN( "Invalid input: cookie == NULL." );
        goto fn_fail;
    }

    err = pvo_delete_island( &cookie->island );
    pvo_free( cookie );

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

int pvo_cookie_insert_var( pvo_cookie_t    cookie,
                            pvo_var_group_t grp,
                            pvo_var_type_t  type,
                            int             ncomps,
                            const char*     name ) {
    pvo_var_t* p;
    pvo_var_t* q;
    int        err = 0;

    if( NULL == cookie ) {
        PVO_WARN( "Invalid input: cookie == NULL." );
        goto fn_fail;
    }

    if( NULL == cookie->vlist )
        err = pvo_create_var( grp, type, ncomps, name,
                             ( cookie->vlist = pvo_malloc( sizeof(pvo_var_t) )) );
    else {
        // Go to the end of the list
        for( p = cookie->vlist; p and p->next; p = p->next ) { }

        if( NULL == p )
            PVO_DIE( "Unexpected null pointer." );
        if( NULL != p->next )
            PVO_WARN( "Expected p->next == NULL. "
                      "This might produce a memory leak." );

        err = pvo_create_var( grp, type, ncomps, name, 
                              ( p->next = pvo_malloc( sizeof(pvo_var_t) )) );
    }

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

int pvo_cookie_remove_var( pvo_cookie_t    cookie,
                            const char*     name ) {
    pvo_var_t* p;
    pvo_var_t* q;
    int        err = 0;

    for( p = cookie->vlist, q = NULL; p; q = p, p = p->next ) {
        if( 0 != strcmp( name, p->name ))
            continue;

        if( q )
            q->next       = p->next; 
        else
            cookie->vlist = p->next;

        pvo_delete_var( p );
        pvo_free( p );

        goto fn_exit;   // Assume names are unique
    }

    PVO_WARN( "Attempt to remove variable \"%s\" which was not "
              "found in the list", name );
    goto fn_fail;

fn_exit:
    return err;
fn_fail:
    err = -1;
    goto fn_exit;
}

int  pvo_cookie_number_var( pvo_cookie_t    cookie ) {
    long       N;
    pvo_var_t* p;

    if( NULL == cookie ) {
        PVO_ERROR( "Invalid input: cookie == NULL." );
        return 0;
    }

    N = 0;
    for( p = cookie->vlist; p; p = p->next ) { 
        N += 1;
    }

    return N;
}

