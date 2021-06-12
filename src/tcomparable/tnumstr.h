#ifndef __T_NUM_STR_H__
#define __T_NUM_STR_H__

#include <glib-object.h>
#include "tstr.h"
#include "../tnumber/tnumber.h"

#define T_TYPE_NUM_STR  (t_num_str_get_type ())
G_DECLARE_FINAL_TYPE (TNumStr, t_num_str, T, NUM_STR, TStr)

/* type of the numeric string */
enum {
  t_none,
  t_int,
  t_double
};

int
t_num_str_is_numeric_string (const char *string);

/* setter and getter */
void
t_num_str_set (TNumStr *self, TNumber *num);

TNumber *
t_num_str_get (TNumStr *self);

int
t_num_str_get_num_type (TNumStr *self);

/* create a new TNumStr instance */
TNumStr *
t_num_str_new_with_int (int i);

TNumStr *
t_num_str_new_with_double (double d);

TNumStr *
t_num_str_new_with_tnumber (TNumber *num);

TNumStr *
t_num_str_new (void);
#endif /* __T_NUM_STR_H__ */

