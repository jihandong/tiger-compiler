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

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static TR_level root_level;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static TR_access_list TR_mk_access_list(TR_access head, TR_access_list tail)
{
    TR_access_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

static TR_access TR_mk_access(TR_level level, FRM_access access)
{
    TR_access p = UTL_alloc(sizeof(*p));

    p->level  = level;
    p->access = access;

    return p;
}

static TR_level TR_mk_level_(TR_level parent, TMP_label name,
                             UTL_bool_list escapes)
{
    TR_level p = UTL_alloc(sizeof(*p));

    p->parent = parent;
    p->frame  = FRM_mk_frame(name, escapes);
    p->tparas = NULL;

    return p;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

TR_level TR_mk_level(TR_level parent, TMP_label name, UTL_bool_list escapes)
{
    TR_level level;
    TR_access_list tparas, p;
    FRM_access_list fparas;

    level  = TR_mk_level_(parent, name, escapes);
    fparas = FRM_get_paras(level->frame);

    /* Already alloc parameters in frame and get their accesses, here we
     * have TR_access = FRM_access + level(level field keeps static link).
     */
    for (tparas = NULL; fparas; fparas = fparas->tail) {
        TR_access access;

        access = TR_mk_access(level, fparas->head);

        if (!tparas) {
            tparas = TR_mk_access_list(access, NULL);
            p = tparas;
        } else {
            p->tail = TR_mk_access_list(access, NULL);
            p = p->tail;
        }
    }

    level->paras = tparas

    return level;
}

TR_level TR_root_level(void)
{
    if (!root_level)
        root = TR_mk_level(NULL, TMP_mk_label_named("__root__"), NULL);

    return root;
}

TR_access_list TR_get_paras(TR_level level)
{
    return level->paras;
}

TR_access TR_alloc_local(TR_level level, bool escape)
{
    return TR_mk_access(level, FRM_alloc_local(level->frame, escape));
}
