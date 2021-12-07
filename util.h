#pragma once

/**
 * exit if running out of memory.
 * @param[in] size
 */
void *U_alloc(int size);

/**
 * exit if running out of memory.
 * @param[in] s
 */
char *U_strdup(const char *s);

/**
 * free everything.
 */
void U_free(void);

/**
 * free everything, print error and exit.
 * @param[in] pos   error position.
 * @param[in] msg   formatted error message.
 * @param[in] ...   formatted args.
 */
void U_error(int pos, const char *fmt, ...);
