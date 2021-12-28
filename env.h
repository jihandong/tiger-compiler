#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "symbol.h"
#include "translate.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef ENV_entry_ *ENV_entry;

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
 * @brief 
 * 
 * @param[in] access 
 * @param[in] type 
 * @return ENV_entry 
 */
ENV_entry ENV_mk_entry_var(TR_access access, TY_type type);

/**
 * @brief 
 * 
 * @param[in] level 
 * @param[in] label 
 * @param[in] paras 
 * @param[in] ret 
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