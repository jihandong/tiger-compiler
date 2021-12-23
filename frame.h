#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "temp.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct FRM_frame_ *         FRM_frame;
typedef struct FRM_access_ *        FRM_access;
typedef struct FRM_access_list_ *   FRM_access_list;

struct FRM_access_list_ { FRM_access head; FRM_access_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Frame constructor.
 *
 * @param[in] name      Name.
 * @param[in] escapes   Parameters are escapable.
 * @return FRM_frame    New Frame.
 */
FRM_frame FRM_mk_frame(TMP_label name, UTL_bool_list escapes);

/**
 * @brief Get Frame name.
 *
 * @param[in] f         Frame.
 * @return TMP_label    Frame name.
 */
TMP_label FRM_get_name(FRM_frame f);

/**
 * @brief Get Frame parameters.
 * 
 * @param[in] f             Frame
 * @return FRM_access_list  Frame parameters.
 */
FRM_access_list FRM_get_paras(FRM_frame f);

/**
 * @brief Alloc local variable.
 * 
 * @param[in] f         Frame.
 * @param[in] escape    Is escapable.
 * @return FRM_access   Alloc result(in-frame or in-reg).
 */
FRM_access FRM_alloc_local(FRM_frame f, bool escape);
