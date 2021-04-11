#include "tint.h"
#include "tdouble.h"
#include "tcomparable.h"

#define PROP_INT 1
static GParamSpec *int_property = NULL;

struct _TInt {
  TNumber parent;
  int value;
};

static void t_comparable_interface_init (TComparableInterface *iface);

/*G_DEFINE_TYPE_WITH_CODE (TInt, t_int, T_TYPE_NUMBER,*/
/*                         G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))*/
static void
t_int_class_init (TIntClass *class);
static void
t_int_init (TInt *d);

GType 
t_int_get_type (void)
{
  static GType type = 0;
  if (type == 0) {
    const GTypeInfo info = {
      sizeof (TIntClass),
      NULL,   /* base_init */
      NULL,   /* base_finalize */
      (GClassInitFunc) t_int_class_init,   /* class_init */
      NULL,   /* class_finalize */
      NULL,   /* class_data */
      sizeof (TInt),
      0,      /* n_preallocs */
      (GInstanceInitFunc) t_int_init,    /* instance_init */
      NULL /* value table */
    };
    const GInterfaceInfo comparable_info = {
      (GInterfaceInitFunc) t_comparable_interface_init,  /* interface_init */
      NULL,   /* interface_finalize */
      NULL    /* interface_data */
    };
    type = g_type_register_static (T_TYPE_NUMBER, "TInt", &info, 0);
    g_type_add_interface_static (type, T_TYPE_COMPARABLE, &comparable_info);
  }
  return type;
}



static int
t_int_comparable_cmp (TComparable *self, TComparable *other) {
  g_return_val_if_fail (T_IS_INT (self), -2);
  g_return_val_if_fail (T_IS_INT (other) || T_IS_DOUBLE (other), -2);

  int i;
  double s, o;

  g_object_get (self, "value", &i, NULL);
  s = (double) i;
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
  iface->cmp = t_int_comparable_cmp;
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
t_int_init (TInt *d) {
}

/* arithmetic operator */
/* These operators create a new instance and return a pointer to it. */
#define t_int_binary_op(op) \
  int i; \
  double d; \
  if (T_IS_INT (other)) { \
    g_object_get (T_INT (other), "value", &i, NULL); \
    return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op i)); \
  } else { \
    g_object_get (T_DOUBLE (other), "value", &d, NULL); \
    return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op (int) d)); \
  }

static TNumber *
t_int_add (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);

  t_int_binary_op (+)
}

static TNumber *
t_int_sub (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);

  t_int_binary_op (-)
}

static TNumber *
t_int_mul (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);

  t_int_binary_op (*)
}

static TNumber *
t_int_div (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_INT (self), NULL);
  int i;
  double d;

  if (T_IS_INT (other)) {
    g_object_get (T_INT (other), "value", &i, NULL);
    if (i == 0) {
      g_signal_emit_by_name (self, "div-by-zero");
      return NULL;
    } else
      return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / i));
  } else {
    g_object_get (T_DOUBLE (other), "value", &d, NULL);
    if (d == 0) {
      g_signal_emit_by_name (self, "div-by-zero");
      return NULL;
    } else
      return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / (int)  d));
  }
}

static TNumber *
t_int_uminus (TNumber *self) {
  g_return_val_if_fail (T_IS_INT (self), NULL);

  return T_NUMBER (t_int_new_with_value (- T_INT(self)->value));
}

static char *
t_int_to_s (TNumber *self) {
  g_return_val_if_fail (T_IS_INT (self), NULL);
  int i;

  g_object_get (T_INT (self), "value", &i, NULL); 
  return g_strdup_printf ("%d", i);
}

static void
t_int_class_init (TIntClass *class) {
  TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  /* override virtual functions */
  tnumber_class->add = t_int_add;
  tnumber_class->sub = t_int_sub;
  tnumber_class->mul = t_int_mul;
  tnumber_class->div = t_int_div;
  tnumber_class->uminus = t_int_uminus;
  tnumber_class->to_s = t_int_to_s;

  gobject_class->set_property = t_int_set_property;
  gobject_class->get_property = t_int_get_property;
  int_property = g_param_spec_int ("value", "val", "Integer value", G_MININT, G_MAXINT, 0, G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_INT, int_property);
}

TInt *
t_int_new_with_value (int value) {
  TInt *d;

  d = g_object_new (T_TYPE_INT, "value", value, NULL);
  return d;
}

TInt *
t_int_new (void) {
  TInt *d;

  d = g_object_new (T_TYPE_INT, NULL);
  return d;
}

