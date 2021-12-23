#pragma once

/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <stdbool.h>
#include "ast.h"
#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct TYP_type_ *        TYP_type;
typedef struct TYP_type_list_ *   TYP_type_list;
typedef struct TYP_field_ *       TYP_field;
typedef struct TYP_field_list_ *  TYP_field_list;

struct TYP_type_
{
    enum {
        TYP_kind_nil,
        TYP_kind_int,
        TYP_kind_str,
        TYP_kind_void,
        TYP_kind_name,
        TYP_kind_func,
        TYP_kind_array,
        TYP_kind_record,
    } kind;

    union {
        struct { SYM_symbol symbol; TYP_type type; }  name; /*< type alias */
        struct { TYP_type ret; TYP_type_list paras; } func;
        TYP_type                                      array;
        TYP_field_list                                record;
    } u;
};

struct TYP_field_      { SYM_symbol name; TYP_type type; };
struct TYP_field_list_ { TYP_field head; TYP_field_list tail; };
struct TYP_type_list_  { TYP_type head; TYP_type_list tail; };

/****************************************************************************
 * Public: basic type
 ****************************************************************************/

/**
 * nil type.
 * @return pre-defined type.
 */
TYP_type TYP_nil(void);
/**
 * integer type.
 * @return pre-defined type.
 */
TYP_type TYP_int(void);
/**
 * string type.
 * @return pre-defined type.
 */
TYP_type TYP_str(void);
/**
 * void type.
 * @return pre-defined type.
 */
TYP_type TYP_void(void);

/****************************************************************************
 * Public: type constructor
 ****************************************************************************/

/**
 * make new type name.
 * @param[in] symbol    type name.
 * @param[in] type      type definition.
 * @return analysed type.
 */
TYP_type TYP_mk_name(SYM_symbol symbol, TYP_type type);
/**
 * make functions type.
 * @param[in] ret   return value type.
 * @param[in] paras parameter types.
 * @return analysed type.
 */
TYP_type TYP_mk_func(TYP_type ret, TYP_type_list paras);
/**
 * make array type.
 * @param[in] type  array element type.
 * @return analysed type.
 */
TYP_type TYP_mk_array(TYP_type type);
/**
 * make record type.
 * @param[in] fields    each field has name and type.
 * @return analysed type.
 */
TYP_type TYP_mk_record(TYP_field_list fields);

/****************************************************************************
 * Public: parameter/argument constructor
 ****************************************************************************/

/**
 * make record field.
 * @param[in] name  field name.
 * @param[in] type  field type.
 * @return new field.
 */
TYP_field TYP_mk_field(SYM_symbol name, TYP_type type);
/**
 * make type list node.
 * @param[in] head
 * @param[in] tail
 * @return new field
 */
TYP_type_list TYP_mk_type_list(TYP_type head, TYP_type_list tail);
/**
 * make record field list node.
 * @param[in] head
 * @param[in] tail
 * @return new field list node.
 */
TYP_field_list TYP_mk_field_list(TYP_field head, TYP_field_list tail);

/****************************************************************************
 * Public: tool functions
 ****************************************************************************/

/**
 * print type definitions.
 * @param[in] out   print target.
 * @param[in] type  type definitions.
 */
void TYP_print(FILE *out, TYP_type type);

/**
 * test type match.
 * @param[in] left  left part type.
 * @param[in] right right part type.
 */
bool TYP_match(TYP_type left, TYP_type right);

/**
 * get type kind(skip TYP_kind_name).
 * @param[in] type  type definitions.
 * @param[in] kind  type kind.
 * @return type kind
 */
int TYP_get_kind(TYP_type type);
