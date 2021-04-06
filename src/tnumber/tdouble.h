#ifndef __T_DOUBLE_H__
#define __T_DOUBLE_H__

#include <glib-object.h>
#include "tnumber.h"

#define T_TYPE_DOUBLE  (t_double_get_type ())
G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, TNumber)

/* create a new TDouble instance */
TDouble *
t_double_new_with_value (double value);

TDouble *
t_double_new (void);
#endif /* __T_DOUBLE_H__ */

