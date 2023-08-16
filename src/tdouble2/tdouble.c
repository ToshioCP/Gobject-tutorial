#include "tdouble.h"

struct _TDouble {
  GObject parent;
  double value;
};

G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)

static void
t_double_class_init (TDoubleClass *class) {
}

static void
t_double_init (TDouble *d) {
}

/* getter and setter */
gboolean
t_double_get_value (TDouble *self, double *value) {
  g_return_val_if_fail (T_IS_DOUBLE (self), FALSE);

  *value = self->value;
  return TRUE;
}

void
t_double_set_value (TDouble *self, double value) {
  g_return_if_fail (T_IS_DOUBLE (self));

  self->value = value;
}

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
#define t_double_binary_op(op) \
  if (! t_double_get_value (other, &value)) \
    return NULL; \
  return t_double_new (self->value op value);

TDouble *
t_double_add (TDouble *self, TDouble *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
  double value;

  t_double_binary_op (+)
}

TDouble *
t_double_sub (TDouble *self, TDouble *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
  double value;

  t_double_binary_op (-)
}

TDouble *
t_double_mul (TDouble *self, TDouble *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
  double value;

  t_double_binary_op (*)
}

TDouble *
t_double_div (TDouble *self, TDouble *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
  double value;

  if ((! t_double_get_value (other, &value)) || value == 0)
    return NULL;
  return t_double_new (self->value / value);
}

TDouble *
t_double_uminus (TDouble *self) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);

  return t_double_new (-self->value);
}

TDouble *
t_double_new (double value) {
  TDouble *d;

  d = g_object_new (T_TYPE_DOUBLE, NULL);
  d->value = value;
  return d;
}
