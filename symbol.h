/********************************************************************************
 * Definitions
 ********************************************************************************/

typedef struct symbol_ *        symbol;
typedef struct symbol_table_*   symbol_table;

/********************************************************************************
 * Public Functions
 ********************************************************************************/

/**
 * Get symbol or make a new symbol if not exists.
 * Hash symbol name into integer as index.
 * @param[in] mame  symbol name
 * @return new symbol, cannot fail.
 */
symbol sym_get(const char *name);

/**
 * Get symbol name.
 * @param[in] s symbol
 * @return symbol name, cannot fail.
 */
const char *sym_get_name(symbol s);

/**
 * Make a new empty symbol table.
 * @return new symbol table, cannot fail.
 */
symbol_table sym_table_new(void);

/**
 * Add a new bind(symbol->value) to symbol table.
 * If symbol already exists, old value will be shadowed.
 * @param[in] t     symbol table
 * @param[in] s     bind symbol
 * @param[in] v     bind value
 */
void sym_table_add(symbol_table t, symbol s, void *v);

/**
 * Get symbol's current value.
 * @param[in] t     symbol table
 * @param[in] s     symbol
 * @return symbol value, cannot fail.
 */
void *sym_table_get(symbol_table t, symbol s);

/**
 * Begin of scope.
 * Push a begin sign into table.
 * @param[in] t     symbol table
 */
void sym_table_begin(symbol_table t);


/**
 * End of scope.
 * @param[in] t     symbol table
 */
void sym_table_end(symbol_table t);
