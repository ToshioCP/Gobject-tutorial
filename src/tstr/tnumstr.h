#pragma once

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

/* get the type of the TNumStr object */
int
t_num_str_get_string_type (TNumStr *self);

/* setter and getter */
void
t_num_str_set_from_t_number (TNumStr *self, TNumber *num);

// TNumStr can have any string, which is t_none, t_int or t_double type.
// If the type is t_none, t_num_str_get_t_number returns NULL.
// It is good idea to call t_num_str_get_string_type and check the type in advance. 

TNumber *
t_num_str_get_t_number (TNumStr *self);

/* create a new TNumStr instance */
TNumStr *
t_num_str_new_with_tnumber (TNumber *num);

TNumStr *
t_num_str_new (void);
