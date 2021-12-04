/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct T_type_ *        T_type;
typedef struct T_type_list_ *   T_type_list;
typedef struct T_field_ *       T_field;
typedef struct T_field_list_ *  T_field_list;

struct T_type_
{
    enum {
        T_kind_nil,
        T_kind_int,
        T_kind_str,
        T_kind_void,
        T_kind_type,
        T_kind_func,
        T_kind_array,
        T_kind_record,
    } kind;

    union {
        struct { S_symbol symbol; T_type type; }    name; /*< type alias */
        struct { T_type ret; T_type_list paras; }   func;
        T_type                                      array;
        T_field_list                                record;
    } u;
};

struct T_field_         { S_symbol name; T_type type; };
struct T_field_list_    { T_field head; T_field_list tail; };
struct T_type_list_     { T_type head; T_type_list tail; };

/****************************************************************************
 * Public: basic type
 ****************************************************************************/

/**
 * nil type.
 * @return pre-defined type.
 */
T_type T_nil(void);
/**
 * integer type.
 * @return pre-defined type.
 */
T_type T_int(void);
/**
 * string type.
 * @return pre-defined type.
 */
T_type T_str(void);
/**
 * void type.
 * @return pre-defined type.
 */
T_type T_void(void);

/****************************************************************************
 * Public: type constructor
 ****************************************************************************/

/**
 * make new type name.
 * @param[in] symbol    type name.
 * @param[in] type      type definition.
 * @return analysed type.
 */
T_type T_name(S_symbol symbol, T_type type);
/**
 * make functions type.
 * @param[in] ret   return value type.
 * @param[in] paras parameter types.
 * @return analysed type.
 */
T_type T_func(T_type ret, T_type_list paras);
/**
 * make array type.
 * @param[in] type  array element type.
 * @return analysed type.
 */
T_type T_array(T_type type);
/**
 * make record type.
 * @param[in] fields    each field has name and type.
 * @return analysed type.
 */
T_type T_record(T_field_list fields);

/****************************************************************************
 * Public: parameter/argument constructor
 ****************************************************************************/

/**
 * make record field.
 * @param name  field name.
 * @param type  field type.
 * @return new field.
 */
T_field T_mk_field(S_symbol name, T_type type);
/**
 * make type list node.
 * @param head
 * @param tail
 * @return new field
 */
T_type_list T_mk_type_list(T_type head, T_type_list tail);
/**
 * make record field list node.
 * @param head
 * @param tail
 * @return new field list node.
 */
T_field_list T_mk_field_list(T_field head, T_field_list tail);

/****************************************************************************
 * Public: display functions
 ****************************************************************************/

/**
 * display type definitions.
 * @param type  type definitions.
 */
void T_pr_type(T_type type);
/**
 * display type list: function parameters or record fields.
 * @param types     type list.
 */
void T_pr_type_list(T_type_list types);

