#ifndef __T_DOUBLE_H__
#define __T_DOUBLE_H__

#include <glib-object.h>

#define T_TYPE_DOUBLE  (t_double_get_type ())
G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, GObject)

gboolean
t_double_get_value (TDouble *d, double *value);

void
t_double_set_value (TDouble *d, double value);

TDouble *
t_double_new (double value);

#endif /* __T_DOUBLE_H__ */

