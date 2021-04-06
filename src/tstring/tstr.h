#ifndef __T_STR_H__
#define __T_STR_H__

#include <glib-object.h>

#define T_TYPE_STR  (t_str_get_type ())
G_DECLARE_FINAL_TYPE (TStr, t_str, T, STR, GObject)

char *
t_str_get_string (TStr *self);

TStr *
t_str_concat (TStr *self, TStr *other);

/* create a new TStr instance */
TStr *
t_str_new_with_string (const char *str);

TStr *
t_str_new (void);
#endif /* __T_STR_H__ */

