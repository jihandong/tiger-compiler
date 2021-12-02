/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct T_type_ *       T_type;
typedef struct T_type_list_ *  T_type_list;
typedef struct T_field_ *      T_field;
typedef struct T_field_list_ * T_field_list;

struct T_type_
{
    enum
    {
        Tk_nil,
        Tk_int,
        Tk_string,
        Tk_void,
        Tk_name,
        Tk_func,
        Tk_array,
        Tk_record,
    } kind;

    union
    {
        struct { S_symbol symbol; T_type type; }    name;
        struct { T_type ret; T_type_list args; }    func;
        T_type                                      array;
        T_field_list                                record;
    } u;
};

struct T_type_list_     { T_type head; T_type_list tail; };
struct T_field_         { S_symbol field; T_type type; };
struct T_field_list_    { T_field head; T_field_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

T_type Tm_nil(void);
T_type Tm_int(void);
T_type Tm_string(void);
T_type Tm_void(void);

T_type Tm_name(S_symbol name, T_type type);
T_type Tm_func(T_type ret, T_type_list args);
T_type Tm_array(T_type type);
T_type Tm_record(T_field_list members);

T_field Tm_field(S_symbol name, T_type type);
T_type_list Tm_type_list(T_type head, T_type_list tail);
T_field_list Tm_field_list(T_field head, T_field_list tail);

void Tp_type(T_type type);
void Tp_type_list(T_type_list types);
