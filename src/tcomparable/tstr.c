#include "tstr.h"
#include "tcomparable.h"

struct _TStr {
  TPtr parent;
};

static void t_comparable_interface_init (TComparableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (TStr, t_str, T_TYPE_PTR,
                         G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))

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
t_str_init (TStr *inst) {
}

static void
t_str_finalize (GObject *object) {
  char *s = (char *) t_ptr_get_pointer (T_PTR (object));

  if (s)
    g_free (s);
  G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
}

static void
t_str_class_init (TStrClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->finalize = t_str_finalize;
}

/* setter and getter */
void
t_str_set_string (TStr *self, const char *s) {
  char *t = g_strdup (s);

  t_ptr_set_pointer (T_PTR (self), t);
}

char *
t_str_get_string (TStr *self) {
  char *s;

  s = (char *) t_ptr_get_pointer (T_PTR (self));
  if (s)
    return g_strdup (s);
  else
    return s; /* NULL */
}

TStr *
t_str_concat (TStr *self, TStr *other) {
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

TStr *
t_str_new_with_string (const char *s) {
  TStr *str;

  str = g_object_new (T_TYPE_STR, NULL);
  t_str_set_string (str,s);
  return str;
}

TStr *
t_str_new (void) {
  TStr *str;

  str = g_object_new (T_TYPE_STR, NULL);
  return str;
}

