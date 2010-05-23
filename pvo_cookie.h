
/// vim: tabstop=4:expandtab:hlsearch

#ifndef PVO_COOKIE_H_INCLUDED
#define PVO_COOKIE_H_INCLUDED 1

/// @file pvo_cookie.h
/// Defines datastructures for "persistent" storage

#include "pvo_types.h"
#include "pvo_var.h"
#include "pvo_island.h"

#ifdef __cplusplus
extern "C" {
#endif

/// pvo_cookie: Persistent storage item
/// Cookies allow to pass persistent information between different
/// PVO files. This especially applies to cached data which should
/// be reused by files written at different timesteps.
struct pvo_cookie {
    
    /// Linked list of variables in the
    /// output.
    pvo_var_t*                  vlist;

    /// The decomposition of PVO_COMM_WORLD into the
    /// different I/O islands will typically be very
    /// static unless adaptivity requires us to redefine
    /// the grouping. Therefore it is stored in the
    /// cookie.
    pvo_island_t                island;

};

/// Users should not bother about the internal structure of
/// the cookie. Rather they are supposed to work with the opaque
/// pvo_cookie_t structure
typedef struct pvo_cookie*  pvo_cookie_t;

/// Create a new cookie.
/// @param[in]  color   the color for the creation of the islands
/// @param[out] cookie  the cookie to be created
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_cookie_create( int color, pvo_cookie_t* cookie );

/// Delete a cookie and free the cache memory
/// @param[inout]   cookie  the cookie to delete
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_cookie_delete( pvo_cookie_t cookie );

/// Insert a new variable into the list of variables registered for
/// the output.
/// The last four parameters are directly passed to the constructor
/// of pvo_var_t.
/// @param[inout]   cookie  the cookie
/// @param[in]      grp     the data group
/// @param[in]      type    the type of the data
/// @param[in]      ncomps  the number of components
/// @param[in]      name    the name of the variable. The name should
///                         be unique
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_cookie_insert_var( pvo_cookie_t    cookie,
                           pvo_var_group_t grp,
                           pvo_var_type_t  type,
                           int             ncomps,
                           const char*     name );

/// Remove a variable from the variable list using the
/// name
/// @param[inout]   cookie  the cookie
/// @param[in]      name    the name of the variable to remove
/// @returns    0 if everything wents fine. -1 otherwise
int pvo_cookie_remove_var( pvo_cookie_t    cookie, 
                           const char*     name );

/// Query the number of variables
/// @param[in]      cookie the cookie
/// @returns        the number
int  pvo_cookie_number_var( pvo_cookie_t    cookie );

#ifdef __cplusplus
}
#endif

#endif

