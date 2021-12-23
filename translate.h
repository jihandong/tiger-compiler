#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "frame.h"
#include "temp.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct TR_access_ *         TR_access;
typedef struct TR_access_list_ *    TR_access_list;

struct TR_access_list_ { TR_access head; TR_access_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

TR_access_list TR_mk_access_list(TR_access head, TR_access_list tail);

TR_level TR_mk_level(TR_level parent, TMP_label name, FRM_escapes paras);

TR_access_list TR_get_paras(TR_level level);

TR_access TR_alloc_local(TR_level level, bool escape);

