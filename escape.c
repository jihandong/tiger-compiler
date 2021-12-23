/****************************************************************************
 * Includes
 ****************************************************************************/

#include "escape.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/**
 * @brief Find escape variables in declaration astnode.
 *
 * @param env   Varibale bind table.
 * @param depth Function call layers.
 * @param d     Declaration astnode.
 */
static void ESC_find_escape_dec(SYM_table env, int depth, AST_dec d);

/**
 * @brief Find escape variables in expression astnode.
 *
 * @param env   Varibale bind table.
 * @param depth Function call layers.
 * @param e     Dxpression astnode.
 */
static void ESC_find_escape_exp(SYM_table env, int depth, AST_exp e);

/**
 * @brief Find escape variables in variable astnode.
 *
 * @param env   Varibale bind table.
 * @param depth Function call layers.
 * @param v     Dariable astnode.
 */
static void ESC_find_escape_var(SYM_table env, int depth, AST_var v);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void ESC_find_escape_dec(SYM_table env, int depth, AST_dec d)
{

}

static void ESC_find_escape_exp(SYM_table env, int depth, AST_exp e)
{

}

static void ESC_find_escape_var(SYM_table env, int depth, AST_var v);
{

}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

void ESC_find_escape(AST_exp root)
{

}
