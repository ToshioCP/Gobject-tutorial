#include <glib-object.h>
#include "tcomparable.h"
#include "tnumber.h"
#include "tint.h"
#include "tdouble.h"
#include "tstr.h"
#include "tnumstr.h"


static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  int i;
  double d;
  char *s;

  name = g_param_spec_get_name (pspec);
  if (T_IS_INT (gobject) && strcmp (name, "value") == 0) {
    g_object_get (T_INT (gobject), "value", &i, NULL);
    g_print ("Property \"%s\" is set to %d.\n", name, i);
  } else if (T_IS_DOUBLE (gobject) && strcmp (name, "value") == 0) {
    g_object_get (T_DOUBLE (gobject), "value", &d, NULL);
    g_print ("Property \"%s\" is set to %lf.\n", name, d);
  } else if (T_IS_STR (gobject) && strcmp (name, "string") == 0) {
    s = t_str_get_string (T_STR (gobject));
    g_print ("Property \"%s\" is set to %s.\n", name, s);
  }
}

static void
t_print (const char *cmp, TComparable *c1, TComparable *c2) {
  char *s1, *s2;

  if (T_IS_NUMBER (c1))
    s1 = t_number_to_s (T_NUMBER (c1));
  else if (T_IS_STR (c1))
    s1 = t_str_get_string (T_STR (c1));
  else {
    g_print ("c1 isn't TInt, TDouble nor TStr.\n");
    return;
  }
  if (T_IS_NUMBER (c2))
    s2 = t_number_to_s (T_NUMBER (c2));
  else if (T_IS_STR (c2))
    s2 = t_str_get_string (T_STR (c2));
  else {
    g_print ("c2 isn't TInt, TDouble nor TStr.\n");
    return;
  }
  if (T_IS_STR (c1))
    g_print ("\"%s\" %s \"%s\".\n", s1, cmp, s2);
  else
    g_print ("%s %s %s.\n", s1, cmp, s2);
  g_free (s1);
  g_free (s2);
}    

static void
compare (TComparable *c1, TComparable *c2) {
  if (t_comparable_eq (c1, c2))
    t_print ("equals", c1, c2);
  if (t_comparable_gt (c1, c2))
    t_print ("is greater than", c1, c2);
  if (t_comparable_lt (c1, c2))
    t_print ("is less than", c1, c2);
  if (t_comparable_ge (c1, c2))
    t_print ("is greater than or equal to", c1, c2);
  if (t_comparable_le (c1, c2))
    t_print ("is less than or equal to", c1, c2);
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  TInt *i;
  TDouble *d;
  TStr *str1, *str2;
  TNumStr *numstr1, *numstr2;
  gpointer obj;

  i = t_int_new ();
  d = t_double_new ();
  str1 = t_str_new ();
  str2 = t_str_new ();
  numstr1 = t_num_str_new ();
  numstr2 = t_num_str_new ();
  obj = g_object_new (G_TYPE_OBJECT, NULL);

  g_signal_connect (G_OBJECT (i), "notify::value", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (d), "notify::value", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (str1), "notify::string", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (str2), "notify::string", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (numstr1), "notify::string", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (numstr2), "notify::string", G_CALLBACK (notify_cb), NULL);

  g_object_set (i, "value", 124, NULL);
  g_object_set (d, "value", 123.45, NULL);
  t_str_set_string (str1, one);
  t_str_set_string (str2, two);
  t_num_str_set (numstr1, T_NUMBER (i));
  t_num_str_set (numstr2, T_NUMBER (d));

  compare (T_COMPARABLE (i), T_COMPARABLE (d));
  compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
  compare (T_COMPARABLE (numstr1), T_COMPARABLE (numstr2));
  t_comparable_eq (T_COMPARABLE (d), obj);

  g_print ("\n");
  t_str_set_string (str1, "100");
  t_str_set_string (str2, "50");
  t_str_set_string (T_STR (numstr1), "100");
  t_str_set_string (T_STR (numstr2), "50");
  compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
  compare (T_COMPARABLE (numstr1), T_COMPARABLE (numstr2));

  g_object_unref (i);
  g_object_unref (d);
  g_object_unref (str1);
  g_object_unref (str2);
  g_object_unref (numstr1);
  g_object_unref (numstr2);
  g_object_unref (obj);

  return 0;
}

