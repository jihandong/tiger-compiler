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
        FRM_kind_frame;
        FRM_kind_reg;
    } kind;

    union {
        int     offset;
        T_temp  reg;
    } u;
}

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static FRM_access FRM_in_frame(int offset)
{
    
}

static FRM_access FRM_in_reg(T_temp reg)
{
    
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

FRM_frame FRM_mk_frame(T_lable name, FRM_escapes paras)
{

}


T_lable FRM_get_name(FRM_frame f)
{

}

FRM_access_list FRM_get_paras(FRM_frame f)
{

}

FRM_access FRM_alloc_local(FRM_frame f, bool escape)
{

}