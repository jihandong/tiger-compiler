#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "table.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/**
 * @brief symbol-bind-table
 *
 * Table has <symbol,void*> type.
 */
typedef TAB_table           SYM_table;
typedef struct SYM_symbol_ *SYM_symbol;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Declare existence of symbol.
 *
 * If symbol name exist, return found symbol, otherwise make a new one.
 *
 * @param name          Symbol name.
 * @return SYM_symbol   New/Found symbol.
 */
SYM_symbol SYM_declare(const char *name);

/**
 * @brief Get symbol name.
 *
 * @param symbol
 * @return const char*  Name.
 */
const char *SYM_get_name(SYM_symbol symbol);

/**
 * @brief Empty symbol-bind-table constructor.
 *
 * @return SYM_table.
 */
SYM_table SYM_empty(void);

/**
 * @brief Enter(push,insert) a symbol-value pair to bind-table.
 *
 * @param[in] table
 * @param[in] symbol
 * @param[in] value
 */
void SYM_enter(SYM_table table, SYM_symbol symbol, void *value);

/**
 * @brief Look(find) a symbol in bind-table.
 *
 * @param[in] table
 * @param[in] symbol
 * @return void*        Value.
 */
void *SYM_look(SYM_table table, SYM_symbol symbol);

/**
 * @brief Begin of scope.
 *
 * Enter an begin sign.
 *
 * @param[in] table.
 */
void SYM_begin(SYM_table table);

/**
 * @brief End of scope.
 *
 * Pop till previous begin sign.
 *
 * @param[in] table.
 */
void SYM_end(SYM_table table);

/**
 * @brief Show content of a bind table.
 *
 * @param[in] table.
 * @param[in] show      Display function.
 */
void SYM_dump(SYM_table table, void (*show)(SYM_symbol s, void *v));
