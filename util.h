/**
 * exit if running out of memory.
 * @param[in] size
 */
void *try_malloc(int size);

/**
 * exit if running out of memory.
 * @param[in] s
 */
char *try_strdup(const char *s);

/**
 * free every slices malloced by functions in "util.h".
 */
void all_free(void);
