#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "translate.h"
#include "type.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct ENV_entry_ *ENV_entry;

struct ENV_entry_
{
    enum {
        ENV_KIND_ENTRY_VAR,
        ENV_KIND_ENTRY_FUNC,
    } kind;

    union {
        struct { TR_access access; TY_type type; } var;
        struct {
            TR_level level; TMP_label label; TY_type_list paras; TY_type ret;
        } func;
    } u;
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Variable Entry constructor.
 *
 * @param[in] access    Belonged-level and alloc result(in-frame or in-reg).
 * @param[in] type      Variable type.
 * @return ENV_entry
 */
ENV_entry ENV_mk_entry_var(TR_access access, TY_type type);

/**
 * @brief Function Entry constructor.
 *
 * @param[in] level     Level contains statc-link and frame allocation.
 * @param[in] label     Function asm label.
 * @param[in] paras     Parameter types.
 * @param[in] ret       Return type.
 * @return ENV_entry
 */
ENV_entry ENV_mk_entry_func(TR_level level, TMP_label label,
                            TY_type_list paras, TY_type ret);

/**
 * @brief Basic value env.
 *
 * @return SYM_table    <symbol,ENV_entry>
 */
SYM_table ENV_base_venv(void);

/**
 * @brief Baisc type env.
 *
 * @return SYM_table    <symbol,TY_type>
 */
SYM_table ENV_base_tenv(void);