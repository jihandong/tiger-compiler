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
static void ESC_find_escape_dec(S_table env, int depth, A_dec d);

/**
 * @brief Find escape variables in expression astnode.
 *
 * @param env   Varibale bind table.
 * @param depth Function call layers.
 * @param e     Dxpression astnode.
 */
static void ESC_find_escape_exp(S_table env, int depth, A_exp e);

/**
 * @brief Find escape variables in variable astnode.
 *
 * @param env   Varibale bind table.
 * @param depth Function call layers.
 * @param v     Dariable astnode.
 */
static void ESC_find_escape_var(S_table env, int depth, A_var v);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void ESC_find_escape_dec(S_table env, int depth, A_dec d)
{

}

static void ESC_find_escape_exp(S_table env, int depth, A_exp e)
{

}

static void ESC_find_escape_var(S_table env, int depth, A_var v);
{

}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

void ESC_find_escape(A_exp root)
{

}
