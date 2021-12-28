/****************************************************************************
 * Includes
 ****************************************************************************/

#include "env.h"
#include "type.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

ENV_entry ENV_mk_entry_var(TR_access access, TY_type type)
{
    ENV_entry p = UTL_alloc(*p);

    p->kind         = ENV_KIND_ENTRY_VAR;
    p->u.var.access = access;
    p->u.var.type   = type;

    return p;
}

ENV_entry ENV_mk_entry_func(TR_level level, TMP_label label,
                            TY_type_list paras, TY_type ret)
{
    ENV_entry p = UTL_alloc(*p);

    p->kind         = ENV_KIND_ENTRY_FUNC;
    p->u.func.level = level;
    p->u.func.label = label;
    p->u.func.paras = paras;
    p->u.func.ret   = ret;

    return p;
}

SYM_table ENV_base_venv(void)
{
    SYM_table base = SYM_empty();

    SYM_enter(base, SYM_declare("int"), TY_int());
    SYM_enter(base, SYM_declare("string"), TY_str());
    // not complete yet

    return base;
}

SYM_table ENV_base_tenv(void)
{
    SYM_table base = SYM_empty();

    // not complete yet

    return SYM_empty();
}