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
 * Public: temp & label
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
 * @param[in] head  Temp Address.
 * @param[in] tail  Temp Address List.
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

/****************************************************************************
 * Public: map
 ****************************************************************************/

/**
 * @brief Map constructor.
 *
 * @param[in] tab
 * @param[in] under
 * @return TMP_map
 */
TMP_map TMP_mk_map(TAB_table tab, TMP_map under);

/**
 * @brief Empty map constructor.
 *
 * @return TMP_map
 */
TMP_map TMP_empty(void);

/**
 * @brief Combine new map list.
 *
 * Example:
 *  over:   t1 -> t2 -> t3
 *  under:  t4 -> t5
 *  return: t1 -> t2 -> t3 -> t4 -> t5
 *
 * @param[in] over  Front map list.
 * @param[in] under Back map list.
 * @return TMP_map  Combined map list.
 */
TMP_map TMP_layer_map(TMP_map over, TMP_map under);

/**
 * @brief Add temp to table in map node.
 * 
 * @param[in] m     Map node.
 * @param[in] t     Temp as mapping key.
 * @param[in] s     Name as mapping Value.
 */
void TMP_enter(TMP_map m, TMP_temp t, const char *s);

/**
 * @brief Find temp in table in map node, recursively.
 *
 * @param[in] m     Map node.
 * @param[in] t     Temp as mapping key.
 * @return char *   Name on success and NULL on failure.
 */
char *TMP_look(TMP_map m, TMP_temp t);
