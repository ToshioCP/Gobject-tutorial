#include "tdouble.h"
#include "tint.h"
#include "tcomparable.h"

#define PROP_DOUBLE 1
static GParamSpec *double_property = NULL;

struct _TDouble {
  TNumber parent;
  double value;
};

static void t_comparable_interface_init (TComparableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (TDouble, t_double, T_TYPE_NUMBER,
                         G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))

static int
t_double_comparable_cmp (TComparable *self, TComparable *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), -2);
  g_return_val_if_fail (T_IS_INT (other) || T_IS_DOUBLE (other), -2);

  int i;
  double s, o;

  g_object_get (self, "value", &s, NULL);
  if (T_IS_INT (other)) {
    g_object_get (other, "value", &i, NULL);
    o = (double) i;
  } else
    g_object_get (other, "value", &o, NULL);

  if (s > o)
    return 1;
  else if (s == o)
    return 0;
  else if (s < o)
    return -1;
  else /* This can't happen. */
    return -2;
}

static void
t_comparable_interface_init (TComparableInterface *iface) {
  iface->cmp = t_double_comparable_cmp;
}

static void
t_double_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TDouble *self = T_DOUBLE (object);
  if (property_id == PROP_DOUBLE) {
    self->value = g_value_get_double (value);
  } else
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
t_double_init (TDouble *d) {
}

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
#define t_double_binary_op(op) \
  int i; \
  double d; \
  if (T_IS_INT (other)) { \
    g_object_get (T_INT (other), "value", &i, NULL); \
    return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value op (double) i)); \
  } else { \
    g_object_get (T_DOUBLE (other), "value", &d, NULL); \
    return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value op d)); \
  }

static TNumber *
t_double_add (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);

  t_double_binary_op (+)
}

static TNumber *
t_double_sub (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);

  t_double_binary_op (-)
}

static TNumber *
t_double_mul (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);

  t_double_binary_op (*)
}

static TNumber *
t_double_div (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  int i;
  double d;

  if (T_IS_INT (other)) {
    g_object_get (T_INT (other), "value", &i, NULL);
    if (i == 0) {
      g_signal_emit_by_name (self, "div-by-zero");
      return NULL;
    } else
      return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value / (double) i));
  } else {
    g_object_get (T_DOUBLE (other), "value", &d, NULL);
    if (d == 0) {
      g_signal_emit_by_name (self, "div-by-zero");
      return NULL;
    } else
      return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value / d));
  }
}

static TNumber *
t_double_uminus (TNumber *self) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);

  return T_NUMBER (t_double_new_with_value (- T_DOUBLE(self)->value));
}

static char *
t_double_to_s (TNumber *self) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  double d;

  g_object_get (T_DOUBLE (self), "value", &d, NULL);
  return g_strdup_printf ("%lf", d);
}

static void
t_double_class_init (TDoubleClass *class) {
  TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  /* override virtual functions */
  tnumber_class->add = t_double_add;
  tnumber_class->sub = t_double_sub;
  tnumber_class->mul = t_double_mul;
  tnumber_class->div = t_double_div;
  tnumber_class->uminus = t_double_uminus;
  tnumber_class->to_s = t_double_to_s;

  gobject_class->set_property = t_double_set_property;
  gobject_class->get_property = t_double_get_property;
  double_property = g_param_spec_double ("value", "val", "Double value", -G_MAXDOUBLE, G_MAXDOUBLE, 0, G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_DOUBLE, double_property);
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

