#include "tcomparable.h"

static guint t_comparable_signal;

G_DEFINE_INTERFACE (TComparable, t_comparable, G_TYPE_OBJECT)

static void
arg_error_default_cb (TComparable *self) {
  g_printerr ("\nTComparable: argument error.\n");
}

static void
t_comparable_default_init (TComparableInterface *iface) {
  /* virtual functio */
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
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), -2);
  g_return_val_if_fail (T_IS_COMPARABLE (other), -2);

  int result;
  TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);

  g_return_val_if_fail (iface->cmp, -2);
  if ((result = iface->cmp (self, other)) == -2)
    g_signal_emit (self, t_comparable_signal, 0);
  return result;
}

gboolean
t_comparable_eq (TComparable *self, TComparable *other) {
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
  g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);

  if (t_comparable_cmp (self, other) == 0)
    return TRUE;
  else
    return FALSE;
}

gboolean
t_comparable_gt (TComparable *self, TComparable *other) {
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
  g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);

  if (t_comparable_cmp (self, other) == 1)
    return TRUE;
  else
    return FALSE;
}

gboolean
t_comparable_lt (TComparable *self, TComparable *other) {
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
  g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);

  if (t_comparable_cmp (self, other) == -1)
    return TRUE;
  else
    return FALSE;
}

gboolean
t_comparable_ge (TComparable *self, TComparable *other) {
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
  g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);


  int result = t_comparable_cmp (self, other);
  if (result == 1 || result == 0)
    return TRUE;
  else
    return FALSE;
}

gboolean
t_comparable_le (TComparable *self, TComparable *other) {
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
  g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);


  int result = t_comparable_cmp (self, other);
  if (result == -1 || result == 0)
    return TRUE;
  else
    return FALSE;
}

