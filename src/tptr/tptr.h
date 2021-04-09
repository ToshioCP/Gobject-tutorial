#ifndef __T_PTR_H__
#define __T_PTR_H__

#include <glib-object.h>

#define T_TYPE_PTR             (t_ptr_get_type ())
G_DECLARE_DERIVABLE_TYPE (TPtr, t_ptr, T, PTR, GObject)

struct _TPtrClass {
  GObjectClass parent_class;
};

/* setter and getter */
void
t_ptr_set_pointer (TPtr *self, gpointer p);

gpointer
t_ptr_get_pointer (TPtr *self);

/* create a new TPtr instance */
TPtr *
t_ptr_new_with_pointer (gpointer pointer);

TPtr *
t_ptr_new (void);

#endif /* __T_PTR_H__ */

