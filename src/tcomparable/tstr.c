#include "tstr.h"
#include "tcomparable.h"

enum {
  PROP_0,
  PROP_STRING,
  N_PROPERTIES
};

static GParamSpec *str_properties[N_PROPERTIES] = {NULL, };

typedef struct {
  char *string;
} TStrPrivate;

static void t_comparable_interface_init (TComparableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (TStr, t_str, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (TStr)
                         G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))

static void
t_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TStr *self = T_STR (object);
  TStrPrivate *priv = t_str_get_instance_private (self);


  if (property_id == PROP_STRING) {
    if (priv->string)
      g_free (priv->string);
    priv->string = g_strdup (g_value_get_string (value));
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TStr *self = T_STR (object);
  TStrPrivate *priv = t_str_get_instance_private (self);

  if (property_id == PROP_STRING)
    g_value_set_string (value, g_strdup (priv->string));
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_str_finalize (GObject *object) {
  TStr *self = T_STR (object);
  TStrPrivate *priv = t_str_get_instance_private (self);

  if (priv->string)
    g_free (priv->string);
  G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
}

static int
t_str_comparable_cmp (TComparable *self, TComparable *other) {
  g_return_val_if_fail (T_IS_STR (self), -2);
  g_return_val_if_fail (T_IS_STR (other), -2);

  char *s, *o;
  int result;

  s = t_str_get_string (T_STR (self));
  o = t_str_get_string (T_STR (other));

  if (strcmp (s, o) > 0)
    result = 1;
  else if (strcmp (s, o) == 0)
    result = 0;
  else if (strcmp (s, o) < 0)
    result = -1;
  else /* This can't happen. */
    result = -2;
  g_free (s);
  g_free (o);
  return result;
}

static void
t_comparable_interface_init (TComparableInterface *iface) {
  iface->cmp = t_str_comparable_cmp;
}

static void
t_str_init (TStr *self) {
  TStrPrivate *priv = t_str_get_instance_private (self);

  priv->string = NULL;
}

static void
t_str_class_init (TStrClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->finalize = t_str_finalize;
  gobject_class->set_property = t_str_set_property;
  gobject_class->get_property = t_str_get_property;
  str_properties[PROP_STRING] = g_param_spec_string ("string", "str", "string", "", G_PARAM_READWRITE);
  g_object_class_install_properties (gobject_class, N_PROPERTIES, str_properties);
}

/* setter and getter */
void
t_str_set_string (TStr *self, const char *s) {
  g_return_if_fail (T_IS_STR (self));

  g_object_set (self, "string", s, NULL);
}

char *
t_str_get_string (TStr *self) {
  g_return_val_if_fail (T_IS_STR (self), NULL);

  char *s;

  g_object_get (self, "string", &s, NULL);
  return s;
}

TStr *
t_str_concat (TStr *self, TStr *other) {
  g_return_val_if_fail (T_IS_STR (self), NULL);
  g_return_val_if_fail (T_IS_STR (other), NULL);

  char *s1, *s2, *s3;
  TStr *str;

  s1 = t_str_get_string (self);
  s2 = t_str_get_string (other);
  if (s1 && s2)
    s3 = g_strconcat (s1, s2, NULL);
  else if (s1)
    s3 = g_strdup (s1);
  else if (s2)
    s3 = g_strdup (s2);
  else
    s3 = NULL;
  str = t_str_new_with_string (s3);
  if (s1) g_free (s1);
  if (s2) g_free (s2);
  if (s3) g_free (s3);
  return str;
}

/* create a new TStr instance */
TStr *
t_str_new_with_string (const char *s) {
  return T_STR (g_object_new (T_TYPE_STR, "string", s, NULL));
}

TStr *
t_str_new (void) {
  return T_STR (g_object_new (T_TYPE_STR, NULL));
}

