#include "tstr.h"

#define PROP_STR 1
static GParamSpec *str_property = NULL;

struct _TStr {
  GObject parent;
  char *string;
};

G_DEFINE_TYPE (TStr, t_str, G_TYPE_OBJECT)

static void
t_str_finalize (GObject *object) {
  TStr *self = T_STR (object);

  if (self->string)
    g_free (self->string);
}

static void
t_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TStr *self = T_STR (object);

  if (property_id == PROP_STR) {
    self->string = g_value_dup_string (value);
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TStr *self = T_STR (object);

  if (property_id == PROP_STR)
    g_value_set_string (value, self->string);
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_str_class_init (TStrClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->finalize = t_str_finalize;

  gobject_class->set_property = t_str_set_property;
  gobject_class->get_property = t_str_get_property;
  str_property = g_param_spec_string ("string", "str", "string", "", G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_STR, str_property);
}

static void
t_str_init (TStr *s) {
}

char *
t_str_get_string (TStr *self) {
  char *s;
  g_object_get (self, "string", &s, NULL);
  return s;
}

TStr *
t_str_concat (TStr *self, TStr *other) {
  char *s1, *s2, *s3;
  TStr *s;

  g_object_get (self, "string", &s1, NULL);
  g_object_get (other, "string", &s2, NULL);
  if (s1 && s2)
    s3 = g_strconcat (s1, s2, NULL);
  else if (s1)
    s3 = g_strdup (s1);
  else if (s2)
    s3 = g_strdup (s2);
  else
    s3 = NULL;
  s = t_str_new_with_string (s3);
  g_free (s1);
  g_free (s2);
  g_free (s3);
  return s;
}

TStr *
t_str_new_with_string (const char *string) {
  TStr *s;

  s = g_object_new (T_TYPE_STR, "string", string, NULL);
  return s;
}

TStr *
t_str_new (void) {
  TStr *s;

  s = g_object_new (T_TYPE_STR, NULL);
  return s;
}

