/****************************************************************************
 * Includes
 ****************************************************************************/

#include "frame.h"
#include "translate.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

struct TR_level_ { TR_level parent;  FRM_frame frame; TR_access_list paras; };
struct TR_access_ { TR_level level; FRM_access access; };
struct TR_access_list_ { TR_access head; TR_access_list tail; };

/****************************************************************************
 * Private Functions
 ****************************************************************************/

TR_access TR_mk_access(TR_level level, FRM_access access)
{
    TR_access p = UTL_alloc(sizeof(*p));

    p->level  = level;
    p->access = access;

    return p;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

TR_access_list TR_mk_access_list(TR_access head, TR_access_list tail)
{
    TR_access_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

TR_level TR_root_level(void)
{
    // should keep root level?
}

TR_level TR_mk_level(TR_level parent, TMP_label name, UTL_bool_list escapes)
{
    TR_level p = UTL_alloc(sizeof(*p));

    p->parent = parent;
    p->frame  = FRM_mk_frame(name, escapes);

    return p;
}

TR_access_list TR_get_paras(TR_level level)
{
    // call FRM_get_paras, then make TR_access_list_?
    // how can I know level of each FRM_access
}

TR_access TR_alloc_local(TR_level level, bool escape)
{
    return TR_mk_access(level, FRM_alloc_local(level->frame, escape));
}
