#include <stdlib.h>
#include <ctype.h>
#include "tnumstr.h"
#include "tnumber.h"
#include "tint.h"
#include "tdouble.h"
#include "tcomparable.h"

/* override string property */

enum {
  PROP_0,
  PROP_STRING,
  N_PROPERTIES
};

struct _TNumStr {
  TStr parent;
  char *string;
  int type;
  int ivalue;
  double dvalue;
};

static void t_comparable_interface_init (TComparableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (TNumStr, t_num_str, T_TYPE_STR,
                         G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))

static int
t_str_comparable_cmp (TComparable *self, TComparable *other) {
  g_return_val_if_fail (T_IS_NUM_STR (self), -2);
  g_return_val_if_fail (T_IS_NUM_STR (other), -2);

  TNumStr *ns_self = T_NUM_STR (self);
  TNumStr *ns_other = T_NUM_STR (other);
  double s, o;
  TNumber *num;
  int i, result;

  if (ns_self->type == t_int)
    s = (double) ns_self->ivalue;
  else if (ns_self->type == t_double)
    s = ns_self->dvalue;
  else
    return -2; /* error */

  num = t_num_str_get (ns_other);
  if (T_IS_INT (num)) {
    g_object_get (num, "value", &i, NULL);
    o = (double) i;
  } else if (T_IS_DOUBLE (num))
    g_object_get (num, "value", &o, NULL);
  else
    return -2; /* error */
 

  if (s > o)
    result = 1;
  else if (s == o)
    result = 0;
  else if (s < o)
    result = -1;
  else /* This can't happen. */
    result = -2;
  return result;
}

static void
t_comparable_interface_init (TComparableInterface *iface) {
  iface->cmp = t_str_comparable_cmp;
}

int
t_num_str_is_numeric_string (const char *string) {
  const char *t;
  int stat, input;
  /* state matrix */
  int m[4][5] = {
    {1, 2, 3, 6, 6},
    {6, 2, 3, 6, 6},
    {6, 2, 3, 4, 6},
    {6, 3, 6, 5, 6}
  };

  stat = 0;
  for (t = string; ; ++t) {
    if (*t == '+' || *t == '-')      
      input = 0;
    else if (isdigit (*t))
      input = 1;
    else if (*t == '.')
      input = 2;
    else if (*t == '\0')
      input = 3;
    else
      input = 4;

    stat = m[stat][input];

    if (stat >= 4 || *t == '\0')
      break;
  }
  if (stat == 4)
    return t_int;
  else if (stat == 5)
    return t_double;
  else
    return t_none;
}

static void
t_num_str_set_string_property (TNumStr *self, const char *string) {
  int type;

  type = t_num_str_is_numeric_string (string);
  if (type == t_int) {
    if (self->string)
      g_free (self->string);
    self->string = g_strdup (string);
    self->type = t_int;
    self->ivalue = atoi (self->string);
  } else if (type == t_double) {
    if (self->string)
      g_free (self->string);
    self->string = g_strdup (string);
    self->type = t_double;
    self->dvalue = atof (self->string);
  } else /* type == NS_OTHER */
    /* The string is not a number. No update. */
    ;
}

static void
t_num_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TNumStr *self = T_NUM_STR (object);

  if (property_id == PROP_STRING)
    t_num_str_set_string_property (self, g_value_get_string (value));
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_num_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TNumStr *self = T_NUM_STR (object);

  if (property_id == PROP_STRING)
    g_value_set_string (value, g_strdup (self->string));
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_num_str_finalize (GObject *object) {
  TNumStr *self = T_NUM_STR (object);

  if (self->string)
    g_free (self->string);
  G_OBJECT_CLASS (t_num_str_parent_class)->finalize (object);
}

static void
t_num_str_init (TNumStr *self) {
  self->type = t_none;
  self->string = NULL;
}

static void
t_num_str_class_init (TNumStrClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->finalize = t_num_str_finalize;
  gobject_class->set_property = t_num_str_set_property;
  gobject_class->get_property = t_num_str_get_property;
  g_object_class_override_property (gobject_class, PROP_STRING, "string");
}

/* setter and getter */
void
t_num_str_set (TNumStr *self, TNumber *num) {
  g_return_if_fail (T_IS_NUM_STR (self));
  g_return_if_fail (T_IS_NUMBER (num));

  char *s;

  if (T_IS_INT (num)) {
    s = t_number_to_s (T_NUMBER (num));
    t_str_set_string (T_STR (self), s);
    g_free (s);
  } else {
    s = t_number_to_s (T_NUMBER (num));
    t_str_set_string (T_STR (self), s);
    g_free (s);
  }
}

TNumber *
t_num_str_get (TNumStr *self) {
  g_return_val_if_fail (T_IS_NUM_STR (self), NULL);

  if (self->type == t_int)
    return T_NUMBER (t_int_new_with_value (self->ivalue));
  else if (self->type == t_double)
    return T_NUMBER (t_double_new_with_value (self->dvalue));
  else
    return NULL;
}

int
t_num_str_get_num_type (TNumStr *self) {
  g_return_val_if_fail (T_IS_NUM_STR (self), G_TYPE_INVALID);

  return self->type;
}

/* create a new TNumStr instance */
TNumStr *
t_num_str_new_with_int (int i) {
  char *s;
  TNumStr *numstr;

  numstr = t_num_str_new ();
  s = g_strdup_printf ("%d", i);
  t_str_set_string (T_STR (numstr), s);
  g_free (s);
  return numstr;
}

TNumStr *
t_num_str_new_with_double (double d) {
  char *s;
  TNumStr *numstr;

  numstr = t_num_str_new ();
  s = g_strdup_printf ("%f", d);
  t_str_set_string (T_STR(numstr), s);
  g_free (s);
  return numstr;
}

TNumStr *
t_num_str_new_with_tnumber (TNumber *num) {
  g_return_val_if_fail (T_IS_NUMBER (num), NULL);

  TNumStr *numstr;

  numstr = t_num_str_new ();
  t_num_str_set (numstr, num);
  return numstr;
}

TNumStr *
t_num_str_new (void) {
  return T_NUM_STR (g_object_new (T_TYPE_NUM_STR, NULL));
}

