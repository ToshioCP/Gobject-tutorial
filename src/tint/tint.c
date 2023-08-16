#include "tint.h"

static guint t_int_signal;

#define PROP_INT 1
static GParamSpec *int_property = NULL;

struct _TInt {
  GObject parent;
  int value;
};

G_DEFINE_TYPE (TInt, t_int, G_TYPE_OBJECT)

static void
div_by_zero_default_cb (TInt *self, gpointer user_data) {
  g_printerr ("\nError: division by zero.\n\n");
}

static void
t_int_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TInt *self = T_INT (object);

  if (property_id == PROP_INT) {
    self->value = g_value_get_int (value);
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_int_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TInt *self = T_INT (object);

  if (property_id == PROP_INT)
    g_value_set_int (value, self->value);
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);

}

static void
t_int_class_init (TIntClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  t_int_signal =
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

  gobject_class->set_property = t_int_set_property;
  gobject_class->get_property = t_int_get_property;
  int_property = g_param_spec_int ("value", "val", "Int value", G_MININT, G_MAXINT, 0, G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_INT, int_property);
}

static void
t_int_init (TInt *self) {
}

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
#define t_int_binary_op(op) \
  g_object_get (other, "value", &value, NULL); \
  return t_int_new_with_value (self->value op value);

TInt *
t_int_add (TInt *self, TInt *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);
  g_return_val_if_fail (T_IS_INT (other), NULL);
  int value;

  t_int_binary_op (+)
}

TInt *
t_int_sub (TInt *self, TInt *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);
  g_return_val_if_fail (T_IS_INT (other), NULL);
  int value;

  t_int_binary_op (-)
}

TInt *
t_int_mul (TInt *self, TInt *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);
  g_return_val_if_fail (T_IS_INT (other), NULL);
  int value;

  t_int_binary_op (*)
}

TInt *
t_int_div (TInt *self, TInt *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);
  g_return_val_if_fail (T_IS_INT (other), NULL);
  int value;

  g_object_get (other, "value", &value, NULL);
  if (value == 0) {
    g_signal_emit (self, t_int_signal, 0);
    return NULL;
  }
  return t_int_new_with_value (self->value / value);
}

TInt *
t_int_uminus (TInt *self) {
  g_return_val_if_fail (T_IS_INT (self), NULL);

  return t_int_new_with_value (-self->value);
}

TInt *
t_int_new_with_value (int value) {
  TInt *i;

  i = g_object_new (T_TYPE_INT, "value", value, NULL);
  return i;
}

TInt *
t_int_new (void) {
  TInt *i;

  i = g_object_new (T_TYPE_INT, NULL);
  return i;
}
