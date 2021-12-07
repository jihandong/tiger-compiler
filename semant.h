#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "ast.h"
#include "type.h"

/****************************************************************************
 * Definition
 ****************************************************************************/

typedef void *I_ir; /*< ir not implemented yet */
typedef struct T_tyir_ T_tyir; /*< ir with type */

struct T_tyir_ {
    I_ir    ir;
    T_type  type;
};

/****************************************************************************
 * Public Function
 ****************************************************************************/

/**
 * semantic check on ast.
 * @param[in] root  ast root node.
 * @return check result.
 */
T_tyir T_trans(A_exp root);

