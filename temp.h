#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct TMP_temp_ *      TMP_temp;   /*< temp value, local variable */
typedef struct TMP_temp_list *  TMP_temp_list;
typedef SYM_symbol              TMP_label;  /*< temp address, asm label */
typedef struct TMP_label_list * TMP_label_list;
typedef struct TMP_map_ *       TMP_map;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief Temp Value constructor.
 *
 * @return TMP_temp     new temp value.
 */
TMP_temp TMP_mk_temp(void);

/**
 * @brief Temp Value List constructor.
 *
 * @param[in] head  Temp Value.
 * @param[in] tail  Temp Value list.
 * @return TMP_temp_list
 */
TMP_temp_list TMP_mk_temp_list(TMP_temp head, TMP_temp_list tail);

/**
 * @brief Temp Address constructor.
 *
 * @return TMP_label    New temp address.
 */
TMP_label TMP_mk_label(void);

/**
 * @brief Temp Address constructor.
 *
 * @param[in] name      Label name.
 * @return TMP_label    New temp address.
 */
TMP_label TMP_mk_label_named(const char *name);

/**
 * @brief Temp Address List constructor.
 *
 * @param[in] head  Temp Address
 * @param[in] tail  Temp Address List
 * @return TMP_label
 */
TMP_label TMP_mk_label_list(TMP_label head, TMP_label_list tail);

/**
 * @brief Get Adress label name.
 *
 * @param[in] label
 * @return TMP_lable
 */
TMP_label TMP_get_label_name(TMP_label label);

TMP_map TMP_layer_map(TMP_map over, TMP_map under);

/**
 * @brief Add temp to map table.
 * 
 * @param[in] m     Map table.
 * @param[in] t     Temp as mapping key.
 * @param[in] s     Name as mapping Value.
 */
void TMP_enter(TMP_map m, TMP_temp t, const char *s);

/**
 * @brief Find temp in map table recursively.
 *
 * @param[in] m     Map table.
 * @param[in] t     Temp as mapping key.
 * @return char *   Name on success and NULL on failure.
 */
char *TMP_look(TMP_map m, TMP_temp t);

void TMP_dump_map(FILE *out, TMP_map m);
