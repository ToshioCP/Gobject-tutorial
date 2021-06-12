#include <glib-object.h>
#include <string.h>
#include "tstr.h"
#include "tnumstr.h"
#include "../tnumber/tint.h"
#include "../tnumber/tdouble.h"

static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  char *s;

  name = g_param_spec_get_name (pspec);
  if (T_IS_STR (gobject) && strcmp (name, "string") == 0) {
    s = t_str_get_string (T_STR (gobject));
    g_print ("String is set to %s.\n", s);
  }
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  TStr *str1, *str2, *str3;
  char *s1, *s2, *s3;

  g_print ("Test program for TStr and TStrNum.\n\n");

  str1 = t_str_new ();
  str2 = t_str_new ();

  g_signal_connect (G_OBJECT (str1), "notify::string", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (str2), "notify::string", G_CALLBACK (notify_cb), NULL);

  t_str_set_string (str1, one);
  t_str_set_string (str2, two);

  str3 = t_str_concat (str1, str2);
   s1 = t_str_get_string (str1);
   s2 = t_str_get_string (str2);
   s3 = t_str_get_string (str3);
  if (s1)
    g_print ("str1 is \"%s\".\n", s1);
  else
    g_print ("str1 is NULL.\n");
  if (s2)
    g_print ("str2 is \"%s\".\n", s2);
  else
    g_print ("str2 is NULL.\n");
  if (s3)
    g_print ("str3 is \"%s\".\n", s3);
  else
    g_print ("str3 is NULL.\n");

  g_object_unref (str1);
  g_object_unref (str2);
  g_object_unref (str3);
  if (s1) g_free (s1);
  if (s2) g_free (s2);
  if (s3) g_free (s3);

/* ---------- */

  TNumStr *numstr;
  TNumber *num;
  char *s;
  int a;
  double x;

  char *t_name[] = {
    "t_none",
    "t_int",
    "t_double"
  };
  struct {
    char *s;
    int type;
  } sample [10] = {
    {"123", t_int},
    {"-45", t_int},
    {"+0", t_int},
    {"123.456", t_double},
    {"+123.456", t_double},
    {"-123.456", t_double},
    {".456", t_double},
    {"123.", t_double},
    {"0.0", t_double},
    {"abc", t_none}
  };
  int j;

  for (j=0; j<10; ++j) {
    if ((a = t_num_str_is_numeric_string (sample[j].s)) != sample[j].type)
      g_print ("%s is %s, but t_num_str_is_numeric_string returns %s.", sample[j].s, t_name[sample[j].type], t_name[a]);
  }
  numstr = t_num_str_new ();
  for (j=0; j<9; ++j) {
    t_str_set_string (T_STR (numstr), sample[j].s);
    if ((a = t_num_str_get_num_type (numstr)) != sample[j].type)
      g_print ("%s is %s, but t_num_str_get_num_type returns %s.", sample[j].s, t_name[sample[j].type], t_name[a]);
  }
  g_object_unref (numstr);

  g_print ("\n");

  numstr = t_num_str_new_with_int (100);
  g_signal_connect (G_OBJECT (numstr), "notify::string", G_CALLBACK (notify_cb), NULL);
  if (!numstr)
    g_print ("Numstr is NULL.\n");
  if (t_num_str_get_num_type (numstr) != t_int)
    g_print ("Numstr is not an integer.\n");
  g_object_get (T_STR (numstr), "string", &s, NULL);
  if (strcmp (s, "100") != 0)
    g_print ("property of numstr is not \"100\", it is %s\n", s);
  g_free (s);
  num = t_num_str_get (numstr);
  if (! T_IS_INT (num))
    g_print ("t_num_str_get doesn't return TInt.\n");
  else {
    g_object_get (T_INT (num), "value", &a, NULL);
    if (a != 100) {
      g_print ("t_num_str_get returns TInt but its value isn't 100.\n");
      g_print ("It is %d\n", a);
    }
  }
  g_object_unref (num);

  g_print ("\n");

  t_str_set_string (T_STR (numstr), "123.456");
  if (t_num_str_get_num_type (numstr) != t_double)
    g_print ("Numstr is not a double.\n");
  g_object_get (T_STR (numstr), "string", &s, NULL);
  if (strcmp (s, "123.456") != 0)
    g_print ("property of numstr is not \"123.456\", it is %s\n", s);
  g_free (s);
  num = t_num_str_get (numstr);
  if (! T_IS_DOUBLE (num))
    g_print ("t_num_str_get doesn't return TDouble.\n");
  else {
    g_object_get (T_DOUBLE (num), "value", &x, NULL);
    if (x != 123.456) {
      g_print ("t_num_str_get returns TDouble but its value isn't 123.456.\n");
      g_print ("It is %f\n", x);
    }
  }

  g_print ("\n");

  t_str_set_string (T_STR (numstr), "abc.def"); /* This string is illegal, so it will be ignored. */
  /* Notify signal will be emitted though "abc.def" is ignored. */
  /* The signal handler prints "123.456". Not "abc.def". */

  g_print ("\n");

  t_str_set_string (T_STR (numstr), "-.789"); /* This string is illegal, so it will be ignored. */
  if (t_num_str_get_num_type (numstr) != t_double)
    g_print ("Numstr is not a double.\n");
  g_object_get (T_STR (numstr), "string", &s, NULL);
  if (strcmp (s, "-.789") != 0)
    g_print ("property of numstr is not \"-.789\", it is %s\n", s);
  g_free (s);
  num = t_num_str_get (numstr);
  if (! T_IS_DOUBLE (num))
    g_print ("t_num_str_get doesn't return TDouble.\n");
  else {
    g_object_get (T_DOUBLE (num), "value", &x, NULL);
    if (x != -0.789) {
      g_print ("t_num_str_get returns TDouble but its value isn't -0.789.\n");
      g_print ("It is %f\n", x);
    }
  }
  g_object_unref (numstr);
  g_object_unref (num);

  return 0;
}

