/**
 * exit if running out of memory.
 * @param[in] size
 */
void *Ualloc(int size);

/**
 * exit if running out of memory.
 * @param[in] s
 */
char *Ustrdup(const char *s);

/**
 * free everything.
 */
void Ufree(void);

/**
 * free everything, print error and exit.
 * @param[in] pos   error position.
 * @param[in] msg   error message.
 */
void Uerror(int pos, const char *msg);
