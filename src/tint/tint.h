#pragma once

#include <glib-object.h>

#define T_TYPE_INT  (t_int_get_type ())
G_DECLARE_FINAL_TYPE (TInt, t_int, T, INT, GObject)

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
TInt *
t_int_add (TInt *self, TInt *other);

TInt *
t_int_sub (TInt *self, TInt *other);

TInt *
t_int_mul (TInt *self, TInt *other);

TInt *
t_int_div (TInt *self, TInt *other);

TInt *
t_int_uminus (TInt *self);

/* create a new TInt instance */
TInt *
t_int_new_with_value (int value);

TInt *
t_int_new (void);
