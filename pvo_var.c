
/// vim: tabstop=4:expandtab:hlsearch

#include "pvo_var.h"
#include "pvo_report.h"

#include <string.h>

int pvo_create_var( pvo_var_group_t     grp,
                     pvo_var_type_t      type,
                     int                 ncomps,
                     const char*         name,
                     pvo_var_t*          var )
{
    var->grp    = grp;
    var->type   = type;
    var->ncomps = ncomps;
    strncpy( var->name, name, 127 );

    var->next   = 0x0;
    var->offset = 0;

    return 0;
}

int pvo_delete_var( pvo_var_t* var )
{
    return 0;
}


