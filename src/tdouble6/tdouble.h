#pragma once

#include <glib-object.h>

#define T_TYPE_DOUBLE  (t_double_get_type ())
G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, GObject)

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
TDouble *
t_double_add (TDouble *self, TDouble *other);

TDouble *
t_double_sub (TDouble *self, TDouble *other);

TDouble *
t_double_mul (TDouble *self, TDouble *other);

TDouble *
t_double_div (TDouble *self, TDouble *other);

TDouble *
t_double_uminus (TDouble *self);

/* create a new TDouble instance */
TDouble *
t_double_new_with_value (double value);

TDouble *
t_double_new (void);

