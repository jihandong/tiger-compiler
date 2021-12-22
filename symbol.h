#pragma once

/********************************************************************************
 * Definitions
 ********************************************************************************/

typedef struct S_symbol_ *S_symbol;
typedef struct S_table_ *S_table;

/********************************************************************************
 * Public Functions
 ********************************************************************************/

/**
 * @brief Get symbol or make a new symbol if not exists.
 * Hash symbol name into integer as index.
 * @param[in] mame  symbol name
 * @return new symbol, cannot fail.
 */
S_symbol S_mk_symbol(const char *name);

/**
 * @brief Make a new empty bind table.
 * @return new bind table, cannot fail.
 */
S_table S_mk_table(void);

/**
 * @brief Get symbol name.
 * @param[in] s symbol
 * @return symbol name, cannot fail.
 */
const char *S_get_name(S_symbol s);

/**
 * @brief Add a new bind(symbol->value) to bind table.
 * If symbol already exists, old value will be shadowed.
 * @param[in] t     bind table
 * @param[in] s     bind symbol
 * @param[in] v     bind value
 */
void S_enter(S_table t, S_symbol s, void *v);

/**
 * @brief Get symbol's current value.
 * @param[in] t     bind table
 * @param[in] s     symbol
 * @return symbol value, cannot fail.
 */
void *S_look(S_table t, S_symbol s);

/**
 * @brief Begin of scope.
 * Push a begin sign into table.
 * @param[in] t     bind table
 * @param[in] name  scope name
 */
void S_begin(S_table t, const char *name);

/**
 * @brief End of scope.
 * Pop out till begin sign.
 * @param[in] t     bind table
 */
void S_end(S_table t);

/**
 * @brief Show content of a bind table.
 * @param[in] t     bind table
 */
void S_show(S_table t);
