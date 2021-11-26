/********************************************************************************
 * Definitions
 ********************************************************************************/

typedef struct symbol_ *        symbol;
typedef struct symbol_table_*   symbol_table;

/********************************************************************************
 * Public Functions
 ********************************************************************************/

/**
 * Make a new symbol.
 * Hash symbol name into integer as index.
 * @param[in] mame  symbol name
 * @return new symbol, cannot fail.
 */
symbol sym_new(const char *name);

/**
 * get symbol name.
 * @param[in] s symbol
 * @return symbol name, cannot fail.
 */
const char *sym_name(symbol s);

/**
 * make a new empty symbol table.
 * @return new symbol table, cannot fail.
 */
symbol_table sym_table_new(void);

/**
 * add a new bind(symbol->value) to symbol table.
 * @param[in] t     symbol table
 * @param[in] s     bind symbol
 * @param[in] v     bind value
 */
void sym_table_add(symbol_table t, symbol s, void *v);

/**
 * get symbol's current value.
 * @param[in] t     symbol table
 * @param[in] s     symbol
 * @return symbol value, cannot fail.
 */
void *sym_table_get(symbol_table t, symbol s);

/**
 * begin of scope.
 * @param[in] t     symbol table
 */
void sym_table_begin(symbol_table t);


/**
 * end of scope.
 * @param[in] t     symbol table
 */
void sym_table_end(symbol_table t);
