#pragma once

/**
 * exit if running out of memory.
 * @param[in] size
 */
void *UTL_alloc(int size);

/**
 * exit if running out of memory.
 * @param[in] s
 */
char *UTL_strdup(const char *s);

/**
 * free everything.
 */
void UTL_free(void);

/**
 * free everything, print error and exit.
 * @param[in] pos   error position.
 * @param[in] msg   formatted error message.
 * @param[in] ...   formatted args.
 */
void UTL_error(int pos, const char *fmt, ...);
