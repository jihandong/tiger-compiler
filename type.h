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
        struct { T_type ret; T_type_list args; }    func;
        T_type                                      array;
        T_field_list                                record;
    } u;
};

struct T_type_list_     { T_type head; T_type_list tail; };
struct T_field_         { S_symbol name; T_type type; };
struct T_field_list_    { T_field head; T_field_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

T_type T_mk_nil(void);
T_type T_mk_int(void);
T_type T_mk_str(void);
T_type T_mk_void(void);

T_type T_mk_name(S_symbol symbol, T_type type);
T_type T_mk_func(T_type ret, T_type_list args);
T_type T_mk_array(T_type type);
T_type T_mk_record(T_field_list fields);

T_field T_mk_field(S_symbol name, T_type type);
T_type_list T_mk_type_list(T_type head, T_type_list tail);
T_field_list T_mk_field_list(T_field head, T_field_list tail);

void T_pr_type(T_type type);
void T_pr_type_list(T_type_list types);

