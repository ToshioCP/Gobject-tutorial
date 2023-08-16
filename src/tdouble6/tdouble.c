#include "tdouble.h"

static guint t_double_signal;

#define PROP_DOUBLE 1
static GParamSpec *double_property = NULL;

struct _TDouble {
  GObject parent;
  double value;
};

G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)

static void
div_by_zero_default_cb (TDouble *self) {
  g_printerr ("\nError: division by zero.\n\n");
}

static void
t_double_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TDouble *self = T_DOUBLE (object);

  if (property_id == PROP_DOUBLE)
    self->value = g_value_get_double (value);
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_double_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TDouble *self = T_DOUBLE (object);

  if (property_id == PROP_DOUBLE)
    g_value_set_double (value, self->value);
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);

}

static void
t_double_class_init (TDoubleClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  t_double_signal =
  g_signal_new_class_handler ("div-by-zero",
                              G_TYPE_FROM_CLASS (class),
                              G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                              G_CALLBACK (div_by_zero_default_cb),
                              NULL /* accumulator */,
                              NULL /* accumulator data */,
                              NULL /* C marshaller */,
                              G_TYPE_NONE /* return_type */,
                              0     /* n_params */
                              );

  gobject_class->set_property = t_double_set_property;
  gobject_class->get_property = t_double_get_property;
  double_property = g_param_spec_double ("value", "val", "Double value", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_DOUBLE, double_property);
}

static void
t_double_init (TDouble *self) {
}

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
#define t_double_binary_op(op) \
  g_object_get (other, "value", &value, NULL); \
  return t_double_new_with_value (self->value op value);

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

  g_object_get (other, "value", &value, NULL);
  if (value == 0) {
    g_signal_emit (self, t_double_signal, 0);
    return NULL;
  }
  return t_double_new_with_value (self->value / value);
}

TDouble *
t_double_uminus (TDouble *self) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);

  return t_double_new_with_value (-self->value);
}

TDouble *
t_double_new_with_value (double value) {
  TDouble *d;

  d = g_object_new (T_TYPE_DOUBLE, "value", value, NULL);
  return d;
}

TDouble *
t_double_new (void) {
  TDouble *d;

  d = g_object_new (T_TYPE_DOUBLE, NULL);
  return d;
}

