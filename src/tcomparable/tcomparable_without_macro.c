#include "tcomparable.h"

static guint t_comparable_signal;

/*G_DEFINE_INTERFACE (TComparable, t_comparable, G_TYPE_OBJECT)*/
static void
t_comparable_default_init (TComparableInterface *iface);
GType
t_comparable_get_type (void) {
  static GType type = 0;
  GTypeInfo info;

  if (type == 0) {
    info.class_size = sizeof (TComparableInterface);
    info.base_init = NULL;
    info.base_finalize = NULL;
    info.class_init = (GClassInitFunc)  t_comparable_default_init;
    info.class_finalize = NULL;
    info.class_data = NULL;
    info.instance_size = 0;
    info.n_preallocs = 0;
    info.instance_init = NULL;
    info.value_table = NULL;
    type = g_type_register_static (G_TYPE_INTERFACE, "TComparable", &info, 0);
  }
  return type;
}

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
                0     /* n_params */,
                NULL  /* param_types */);
}

int
t_comparable_cmp (TComparable *self, TComparable *other) {
  if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
    g_signal_emit (self, t_comparable_signal, 0);
  g_return_val_if_fail (T_IS_COMPARABLE (self), -2);
  g_return_val_if_fail (T_IS_COMPARABLE (other), -2);

  TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);
  int result = iface->cmp ? iface->cmp (self, other) : -2;

  if (result == -2)
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

