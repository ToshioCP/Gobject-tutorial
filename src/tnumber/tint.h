#pragma once

#include <glib-object.h>

#define T_TYPE_INT  (t_int_get_type ())
G_DECLARE_FINAL_TYPE (TInt, t_int, T, INT, TNumber)

/* create a new TInt instance */
TInt *
t_int_new_with_value (int value);

TInt *
t_int_new (void);
