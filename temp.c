/****************************************************************************
 * Includes
 ****************************************************************************/

#include "table.h"
#include "temp.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

struct TMP_temp_ { int index; };
struct TMP_temp_list { TMP_temp head;  TMP_temp_list tail; };
struct TMP_label_list { TMP_label head; TMP_label_list tail; };
struct TMP_map_ { TAB_table tab; TMP_map under; };

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static int ntemps;
static int nlabels;

/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Public Functions
 ****************************************************************************/

TMP_temp TMP_mk_temp(void)
{
    TMP_temp p = UTL_alloc(sizeof(*p));

    p->index = ntemps++;

    return p;
}

TMP_temp_list TMP_mk_temp_list(TMP_temp head, TMP_temp_list tail)
{
    TMP_temp_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

TMP_label TMP_mk_label(void)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "label%s", nlabels++);

    return TMP_mk_label_named(buf);
}

TMP_label TMP_mk_label_named(const char *name)
{
    return SYM_symbol(name);
}

TMP_label TMP_mk_label_list(TMP_label head, TMP_label_list tail)
{
    TMP_label_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

TMP_label TMP_get_label_name(TMP_label label)
{
    return SYM_get_name(label);
}
