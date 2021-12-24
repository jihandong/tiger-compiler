/****************************************************************************
 * Includes
 ****************************************************************************/

#include "translate.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

struct TR_level_ {}
struct TR_access_ { TR_level level; FRM_access access; };
struct TR_access_list_ { TR_access head; TR_access_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

TR_access_list TR_mk_access_list(TR_access head, TR_access_list tail)
{
    
}

TR_level TR_root_level(void)
{
    
}

TR_level TR_mk_level(TR_level parent, TMP_label name, UTL_bool_list escapes)
{
    
}

TR_access_list TR_get_paras(TR_level level)
{
    
}

TR_access TR_alloc_local(TR_level level, bool escape)
{
    
}
