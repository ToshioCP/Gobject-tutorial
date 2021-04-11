#ifndef __T_COMPARABLE_H__
#define __T_COMPARABLE_H__

#include <glib-object.h>

#define T_TYPE_COMPARABLE  (t_comparable_get_type ())
/*G_DECLARE_INTERFACE (TComparable, t_comparable, T, COMPARABLE, GObject)*/
GType *t_comparable_get_type (void);
typedef struct _TComparableInterface TComparableInterface;
#define T_COMPARABLE(instance)           (G_TYPE_CHECK_INSTANCE_CAST(instance, T_TYPE_COMPARABLE, TComparable)
#define T_IS_COMPARABLE(instance)        (G_TYPE_CHECK_INSTANCE_TYPE(instance, T_TYPE_COMPARABLE)
#define T_COMPARABLE_GET_IFACE(instance) (G_TYPE_INSTANCE_GET_INTERFACE(instance, T_TYPE_COMPARABLE, TComparable)

struct _TComparableInterface {
  GTypeInterface parent;
  /* signal */
  void (*arg_error) (TComparable *self);
  /* virtual function */
  int (*cmp) (TComparable *self, TComparable *other);
};

/* t_comparable_cmp */
/* if self > other, then returns 1 */
/* if self = other, then returns 0 */
/* if self < other, then returns -1 */
/* if error happens, then returns -2 */

int
t_comparable_cmp (TComparable *self, TComparable *other);

gboolean
t_comparable_eq (TComparable *self, TComparable *other);

gboolean
t_comparable_gt (TComparable *self, TComparable *other);

gboolean
t_comparable_lt (TComparable *self, TComparable *other);

gboolean
t_comparable_ge (TComparable *self, TComparable *other);

gboolean
t_comparable_le (TComparable *self, TComparable *other);

#endif /* __T_COMPARABLE_H__ */

