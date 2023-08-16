#include "tcomparable.h"

static guint t_comparable_signal;

G_DEFINE_INTERFACE (TComparable, t_comparable, G_TYPE_OBJECT)

static void
arg_error_default_cb (TComparable *self) {
  g_printerr ("\nTComparable: argument error.\n");
}

static void
t_comparable_default_init (TComparableInterface *iface) {
  /* virtual function */
  iface->cmp = NULL;
  /* argument error signal */
  iface->arg_error = arg_error_default_cb;
  t_comparable_signal =
  g_signal_new ("arg-error",
                T_TYPE_COMPARABLE,
                G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                G_STRUCT_OFFSET (TComparableInterface, arg_error),
                NULL /* accumulator */,
                NULL /* accumulator data */,
                NULL /* C marshaller */,
                G_TYPE_NONE /* return_type */,
                0     /* n_params */
                );
}

int
t_comparable_cmp (TComparable *self, TComparable *other) {
  g_return_val_if_fail (T_IS_COMPARABLE (self), -2);

  TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);
  
  return (iface->cmp == NULL ? -2 : iface->cmp (self, other));
}

gboolean
t_comparable_eq (TComparable *self, TComparable *other) {
  return (t_comparable_cmp (self, other) == 0);
}

gboolean
t_comparable_gt (TComparable *self, TComparable *other) {
  return (t_comparable_cmp (self, other) == 1);
}

gboolean
t_comparable_lt (TComparable *self, TComparable *other) {
  return (t_comparable_cmp (self, other) == -1);
}

gboolean
t_comparable_ge (TComparable *self, TComparable *other) {
  int result = t_comparable_cmp (self, other);
  return (result == 1 || result == 0);
}

gboolean
t_comparable_le (TComparable *self, TComparable *other) {
  int result = t_comparable_cmp (self, other);
  return (result == -1 || result == 0);
}
