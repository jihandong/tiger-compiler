#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct TMP_temp_ *      TMP_temp;
typedef struct TMP_temp_list *  TMP_temp_list;
typedef SYM_symbol              TMP_label;
typedef struct TMP_label_list * TMP_label_list;

struct TMP_temp_list { TMP_temp head;  TMP_temp_list tail; };
struct TMP_label_list { TMP_label head; TMP_label_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

TMP_temp TMP_mk_temp(void);

TMP_temp_list TMP_mk_temp_list(TMP_temp head, TMP_temp_list tail);

TMP_label TMP_mk_label(void);

TMP_label TMP_mk_label_with_name(const char *name);

TMP_label TMP_mk_label_list(TMP_label head, TMP_label_list tail);

TMP_lable TMP_get_name(TMP_label label);
