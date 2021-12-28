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
 * Privates
 ****************************************************************************/

static int ntemps;
static int nlabels;

/****************************************************************************
 * Public: temp & label
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

/****************************************************************************
 * Public: map
 ****************************************************************************/

TMP_map TMP_mk_map(TAB_table tab, TMP_map under)
{
    TMP_map m = UTL_alloc(sizeof(*m));

    m->tab   = tab;
    m->under = under;

    return m;
}

TMP_map TMP_empty(void)
{
    return TMP_mk_map(TAB_empty(), NULL);
}

TMP_map TMP_layer_map(TMP_map over, TMP_map under)
{
    if (!over)
        return under;

    return TMP_mk_map(over->tab, TMP_layer_map(over->under, under));
}

void TMP_enter(TMP_map m, TMP_temp t, const char *s);
{
    if (!m || !m->tab)
        UTL_error(UTL_NOPOS, "enter temp to a null map");

    TAB_enter(m->tab, t, s);
}

char *TMP_look(TMP_map m, TMP_temp t)
{
    const char *s;

    if (!m || !m->tab)
        UTL_error(UTL_NOPOS, "enter temp to a null map");

    s = TAB_look(m->tab, t);

    if (s)
        return s;
    else if (m->under)
        return TMP_look(m->under, t);
    else
        return NULL;
}
