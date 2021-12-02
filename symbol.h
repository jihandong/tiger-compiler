/********************************************************************************
 * Definitions
 ********************************************************************************/

typedef struct S_symbol_ *S_symbol;
typedef struct S_table_ *S_table;

/********************************************************************************
 * Public Functions
 ********************************************************************************/

/**
 * Get symbol or make a new symbol if not exists.
 * Hash symbol name into integer as index.
 * @param[in] mame  symbol name
 * @return new symbol, cannot fail.
 */
S_symbol Sf_symbol(const char *name);

/**
 * Make a new empty symbol table.
 * @return new symbol table, cannot fail.
 */
S_table Sf_table(void);

/**
 * Get symbol name.
 * @param[in] s symbol
 * @return symbol name, cannot fail.
 */
const char *Sf_name(S_symbol s);

/**
 * Add a new bind(symbol->value) to symbol table.
 * If symbol already exists, old value will be shadowed.
 * @param[in] t     symbol table
 * @param[in] s     bind symbol
 * @param[in] v     bind value
 */
void Sf_enter(S_table t, S_symbol s, void *v);

/**
 * Get symbol's current value.
 * @param[in] t     symbol table
 * @param[in] s     symbol
 * @return symbol value, cannot fail.
 */
void *Sf_look(S_table t, S_symbol s);

/**
 * Begin of scope.
 * Push a begin sign into table.
 * @param[in] t     symbol table
 */
void Sf_begin(S_table t);


/**
 * End of scope.
 * @param[in] t     symbol table
 */
void Sf_end(S_table t);
