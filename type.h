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

typedef struct TY_type_ *        TY_type;
typedef struct TY_type_list_ *   TY_type_list;
typedef struct TY_field_ *       TY_field;
typedef struct TY_field_list_ *  TY_field_list;

struct TY_type_
{
    enum {
        TY_kind_nil,
        TY_kind_int,
        TY_kind_str,
        TY_kind_void,
        TY_kind_name,
        TY_kind_func,
        TY_kind_array,
        TY_kind_record,
    } kind;

    union {
        struct { SYM_symbol symbol; TY_type type; }  name; /*< type alias */
        struct { TY_type ret; TY_type_list paras; } func;
        TY_type                                      array;
        TY_field_list                                record;
    } u;
};

struct TY_field_      { SYM_symbol name; TY_type type; };
struct TY_field_list_ { TY_field head; TY_field_list tail; };
struct TY_type_list_  { TY_type head; TY_type_list tail; };

/****************************************************************************
 * Public: basic type
 ****************************************************************************/

/**
 * nil type.
 * @return pre-defined type.
 */
TY_type TY_nil(void);
/**
 * integer type.
 * @return pre-defined type.
 */
TY_type TY_int(void);
/**
 * string type.
 * @return pre-defined type.
 */
TY_type TY_str(void);
/**
 * void type.
 * @return pre-defined type.
 */
TY_type TY_void(void);

/****************************************************************************
 * Public: type constructor
 ****************************************************************************/

/**
 * make new type name.
 * @param[in] symbol    type name.
 * @param[in] type      type definition.
 * @return analysed type.
 */
TY_type TY_mk_name(SYM_symbol symbol, TY_type type);
/**
 * make functions type.
 * @param[in] ret   return value type.
 * @param[in] paras parameter types.
 * @return analysed type.
 */
TY_type TY_mk_func(TY_type ret, TY_type_list paras);
/**
 * make array type.
 * @param[in] type  array element type.
 * @return analysed type.
 */
TY_type TY_mk_array(TY_type type);
/**
 * make record type.
 * @param[in] fields    each field has name and type.
 * @return analysed type.
 */
TY_type TY_mk_record(TY_field_list fields);

/****************************************************************************
 * Public: parameter/argument constructor
 ****************************************************************************/

/**
 * make record field.
 * @param[in] name  field name.
 * @param[in] type  field type.
 * @return new field.
 */
TY_field TY_mk_field(SYM_symbol name, TY_type type);
/**
 * make type list node.
 * @param[in] head
 * @param[in] tail
 * @return new field
 */
TY_type_list TY_mk_type_list(TY_type head, TY_type_list tail);
/**
 * make record field list node.
 * @param[in] head
 * @param[in] tail
 * @return new field list node.
 */
TY_field_list TY_mk_field_list(TY_field head, TY_field_list tail);

/****************************************************************************
 * Public: tool functions
 ****************************************************************************/

/**
 * print type definitions.
 * @param[in] out   print target.
 * @param[in] type  type definitions.
 */
void TY_print(FILE *out, TY_type type);

/**
 * test type match.
 * @param[in] left  left part type.
 * @param[in] right right part type.
 */
bool TY_match(TY_type left, TY_type right);

/**
 * get type kind(skip TY_kind_name).
 * @param[in] type  type definitions.
 * @param[in] kind  type kind.
 * @return type kind
 */
int TY_get_kind(TY_type type);
