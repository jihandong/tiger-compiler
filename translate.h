#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>
#include "temp.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct TR_level_ *          TR_level;
typedef struct TR_access_ *         TR_access;
typedef struct TR_access_list_ *    TR_access_list;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Access List constructor.
 *
 * @param[in] head
 * @param[in] tail
 * @return TR_access_list
 */
TR_access_list TR_mk_access_list(TR_access head, TR_access_list tail);

/**
 * @brief Get root level.
 *
 * To see the whole static list.
 *
 * @return TR_level
 */
TR_level TR_root_level(void);

/**
 * @brief Call Level constructor.
 *
 * For each function-call, a new level will be created, a new frame will be
 * created too.
 *
 * @param[in] parent    Caller level.
 * @param[in] name      Callee label.
 * @param[in] escapes   Wether parameters escapable.
 * @return TR_level
 */
TR_level TR_mk_level(TR_level parent, TMP_label name, UTL_bool_list escapes);

/**
 * @brief Get call level parameters(access entries).
 *
 * @param[in] level     Call level.
 * @return TR_access_list
 */
TR_access_list TR_get_paras(TR_level level);

/**
 * @brief Alloc local variable in Call Level.
 *
 * @param[in] level     Call level.
 * @param[in] escape    Whether variable escapable.
 * @return TR_access    Alloc result.
 */
TR_access TR_alloc_local(TR_level level, bool escape);
