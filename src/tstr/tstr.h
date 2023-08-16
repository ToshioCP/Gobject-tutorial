#pragma once

#include <glib-object.h>

#define T_TYPE_STR  (t_str_get_type ())
G_DECLARE_DERIVABLE_TYPE (TStr, t_str, T, STR, GObject)

struct _TStrClass {
  GObjectClass parent_class;
  /* expect that descendants will override the setter */
  void (*set_string)  (TStr *self, const char *s);
};

TStr *
t_str_concat (TStr *self, TStr *other);

/* setter and getter */
void
t_str_set_string (TStr *self, const char *s);

char *
t_str_get_string (TStr *self);

/* create a new TStr instance */
TStr *
t_str_new_with_string (const char *s);

TStr *
t_str_new (void);
