#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct FRM_frame_       FRM_frame;
typedef struct FRM_access_      FRM_access;
typedef struct FRM_access_list_ FRM_access_list;

struct FRM_access_list_ { FRM_access head; FRM_access_list tail; };
struct FRM_escapes { int npara; int mask; }

/****************************************************************************
 * Public Functions
 ****************************************************************************/

FRM_frame FRM_mk_frame(TYP_lable name, FRM_escapes paras);

TYP_lable FRM_get_name(FRM_frame f);

FRM_access_list FRM_get_paras(FRM_frame f);

FRM_access FRM_alloc_local(FRM_frame f, bool escape);
