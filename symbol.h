#pragma once

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct SYM_symbol_ *SYM_symbol;
typedef struct SYM_table_ *SYM_table;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Get symbol or make a new symbol if not exists.
 * Hash symbol name into integer as index.
 * @param[in] mame  symbol name
 * @return new symbol, cannot fail.
 */
SYM_symbol SYM_mk_symbol(const char *name);

/**
 * @brief Make a new empty bind table.
 * @return new bind table, cannot fail.
 */
SYM_table SYM_mk_table(void);

/**
 * @brief Get symbol name.
 * @param[in] s symbol
 * @return symbol name, cannot fail.
 */
const char *SYM_get_name(SYM_symbol s);

/**
 * @brief Add a new bind(symbol->value) to bind table.
 * If symbol already exists, old value will be shadowed.
 * @param[in] t     bind table
 * @param[in] s     bind symbol
 * @param[in] v     bind value
 */
void SYM_enter(SYM_table t, SYM_symbol s, void *v);

/**
 * @brief Get symbol's current value.
 * @param[in] t     bind table
 * @param[in] s     symbol
 * @return symbol value, cannot fail.
 */
void *SYM_look(SYM_table t, SYM_symbol s);

/**
 * @brief Begin of scope.
 * Push a begin sign into table.
 * @param[in] t     bind table
 * @param[in] name  scope name
 */
void SYM_begin(SYM_table t, const char *name);

/**
 * @brief End of scope.
 * Pop out till begin sign.
 * @param[in] t     bind table
 */
void SYM_end(SYM_table t);

/**
 * @brief Show content of a bind table.
 * @param[in] t     bind table
 */
void SYM_show(SYM_table t);
