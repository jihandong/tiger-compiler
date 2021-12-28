/****************************************************************************
 * Includes
 ****************************************************************************/

#inlcude "frame.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

struct FRM_access_
{
    enum {
        FRM_kind_in_frame;
        FRM_kind_in_reg;
    } kind;

    union {
        int         offset;
        TMP_temp    reg;
    } u;
}

struct FRM_access_list_ { FRM_access head; FRM_access_list tail; };

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static FRM_access FRM_in_frame(int offset)
{
    return NULL;
}

static FRM_access FRM_in_reg(TY_temp reg)
{
    return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

FRM_frame FRM_mk_frame(TMP_label name, UTL_bool_list escapes)
{
    return NULL;
}


TMP_lable FRM_get_name(FRM_frame f)
{
    return NULL;
}

FRM_access_list FRM_get_paras(FRM_frame f)
{
    return NULL;
}

FRM_access FRM_alloc_local(FRM_frame f, bool escape)
{
    return nULL;
}