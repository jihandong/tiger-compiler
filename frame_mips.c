/****************************************************************************
 * Includes
 ****************************************************************************/

#include "frame.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define FRM_REG_MAX     4
#define FRM_WORD_SIZE   4

struct FRM_frame_
{
    TMP_label       name;
    FRM_access_list paras;
    int             offset;
};

struct FRM_access_
{
    enum {
        FRM_kind_in_frame;
        FRM_kind_in_reg;
    } kind;

    union {
        int      offset;
        TMP_temp reg;
    } u;
}

struct FRM_access_list_ { FRM_access head; FRM_access_list tail; };

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * @brief Alloc temp in frame.
 *
 * @param[in] offset    Offset in frame.
 * @return FRM_access   Alloc result.
 */
static FRM_access FRM_alloc_in_frame(int offset)
{
    FRM_access p = UTL_alloc(sizeof(*p));

    p->kind = FRM_kind_in_frame;
    p->u.offset = offset;

    return p;
}

/**
 * @brief Alloc temp in register.
 *
 * @param[in] reg       Alloced temp.
 * @return FRM_access   Alloc result.
 */
static FRM_access FRM_alloc_in_reg(TMP_temp reg)
{
    FRM_access p = UTL_alloc(sizeof(*p));

    p->kind = FRM_kind_in_reg;
    p->u.reg = reg;

    return p;
}

/**
 * @brief Access list constructor.
 *
 * @param head
 * @param tail
 * @return FRM_frame_list
 */
static FRM_frame_list FRM_mk_access_list(FRM_access head, FRM_access_list tail)
{
    FRM_access_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

/**
 * @brief Empty frame constructor.
 * 
 * @param[in] name      Frame label.
 * @return FRM_frame
 */
static FRM_frame FRM_mk_frame_(TMP_label name)
{
    FRM_frame p = UTL_alloc(sizeof(*p));

    p->name   = name;
    p->paras  = NULL;
    p->offset = 0;

    return p
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

FRM_access FRM_alloc_local(FRM_frame f, bool escape)
{
    FRM_access access;

    /* Trival alloc strategy ...
     * Alloc escaped variable in frame, others in register.
     */
    if (escape) {
        access = FRM_alloc_in_frame(f->offset);
        f->offset += FRM_WORD_SIZE;
    } else {
        access = FRM_alloc_in_reg(TMP_mk_temp());
    }

    return access;
}

FRM_frame FRM_mk_frame(TMP_label name, UTL_bool_list escapes)
{
    FRM_frame frame;
    FRM_access_list paras, p;

    frame = FRM_mk_frame_(name);

    for (paras = NULL; escapes; escapes = escapes->tail) {
        FRM_access access;

        access = FRM_alloc_local(frame, escapes->head);

        if (!paras) {
            paras = FRM_mk_access_list(access, NULL);
            p = paras;
        } else {
            p->tail = FRM_mk_access_list(access, NULL);
            p = p->tail;
        }
    }

    frame->paras = paras;

    return frame;
}

TMP_lable FRM_get_name(FRM_frame f)
{
    return f->name;
}

FRM_access_list FRM_get_paras(FRM_frame f)
{
    return f->paras;
}
