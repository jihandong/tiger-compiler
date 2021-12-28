#pragma once

/****************************************************************************
 * Definitions
 ****************************************************************************/

/**
 * @brief "hash stack".
 *
 * As Hash Table, it has O(1) modify/query speed (assume no hash confilict).
 * As Stack, it has FILO feature.
 */
typedef struct TAB_table_ *TAB_table;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Empty Table constructor
 *
 * @return TAB_table
 */
TAB_table TAB_empty(void);

/**
 * @brief Enter(push,insert) a key-value pair to table.
 *
 * @param[in] table
 * @param[in] key
 * @param[in] value
 */
void TAB_enter(TAB_table t, void *key, void *value);

/**
 * @brief Look(find) a key in table.
 *
 * @param[in] table
 * @param[in] key
 * @return void*    Value looked.
 */
void *TAB_look(TAB_table t, void *key);

/**
 * @brief Pop out a key-value pair from table.
 *
 * @param[in] table
 * @return void*    Key poped.
 */
void *TAB_pop(TAB_table table);

/**
 * @brief Dump(show)
 *
 * @param[in] table
 * @param[in] show  Display Function.
 */
void TAB_dump(TAB_table table, void (*show)(void *key, void *value));
