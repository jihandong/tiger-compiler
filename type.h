/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct ty_type_ *               ty_type;
typedef struct ty_type_list_ *          ty_type_list;
typedef struct ty_record_field_ *       ty_rfield;
typedef struct ty_record_field_list_ *  ty_rfield_list;

struct ty_type_
{
    enum
    {
        kind_ty_nil,
        kind_ty_int,
        kind_ty_string,
        kind_ty_array,
        kind_ty_record,
        kind_ty_name,
        kind_ty_void,
    } kind;

    union
    {
        struct { symbol name; ty_type type; }   name;
        ty_type                                 array;
        ty_rfield_list                          record;
    } u;
};

struct ty_record_field_         { symbol name; ty_type type; };
struct ty_type_list_            { ty_type head; ty_type_list tail; };
struct ty_record_field_list_    { ty_rfield head; ty_rfield_list tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

ty_type ty_mk_nil(void);
ty_type ty_mk_int(void);
ty_type ty_mk_string(void);
ty_type ty_mk_void(void);

ty_type ty_mk_name(symbol name, ty_type type);
ty_type ty_mk_array(ty_type type);
ty_type ty_mk_record(ty_rfield_list members);

ty_rfield ty_mk_rfield(symbol name, ty_type type);
ty_type_list ty_mk_type_list(ty_type head, ty_type_list tail);
ty_rfield_list ty_mk_rfield_list(ty_rfield head, ty_rfield_list tail);
