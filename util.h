#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define UTL_NOPOS -1

typedef struct UTL_bool_list_ * UTL_bool_list;

struct UTL_bool_list_ { bool head; UTL_bool_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

/**
 * @brief Bool list constructor.
 * 
 * @param head  Bool.
 * @param tail  Bool list.
 * @return UTL_bool_list
 */
UTL_bool_list UTL_mk_bool_list(bool head, UTL_bool_list tail);
