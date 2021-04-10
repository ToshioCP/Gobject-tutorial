#ifndef __T_INT_H__
#define __T_INT_H__

#include <glib-object.h>
#include "tnumber.h"

#define T_TYPE_INT  (t_int_get_type ())
G_DECLARE_FINAL_TYPE (TInt, t_int, T, INT, TNumber)

/* create a new TInt instance */
TInt *
t_int_new_with_value (int value);

TInt *
t_int_new (void);
#endif /* __T_INT_H__ */

