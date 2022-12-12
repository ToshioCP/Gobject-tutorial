/* test for public methods and the notify signal for TStr and TNumStr */

#include <glib-object.h>
#include <string.h>
#include "tstr.h"
#include "tnumstr.h"
#include "../tnumber/tint.h"
#include "../tnumber/tdouble.h"

/* ----- Test for TStr ----- */

static int notify_signal = 0;

static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  char *s;

  name = g_param_spec_get_name (pspec);
  if (strcmp (name, "string") == 0
    && strcmp ((s = t_str_get_string(T_STR (gobject))), "hello") == 0)
      notify_signal = 1;
  if (s) g_free (s);
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  TStr *str1, *str2, *str3;
  char *s;

/* test for the notify signal and set/get property */
  str1 = t_str_new ();
  g_signal_connect (G_OBJECT (str1), "notify::string", G_CALLBACK (notify_cb), NULL);
  notify_signal = 0;
  g_object_set (G_OBJECT (str1), "string", "hello", NULL);
  if (notify_signal != 1)
    g_print ("Notify::string signal was NOT caught.\n");
  g_object_get(str1, "string", &s, NULL);
  if (strcmp(s, "hello") != 0)
    g_print ("g_object_get didn't work.\n");
  if (s) g_free (s);

/* test for t_str_set_string and t_str_get_string */

  t_str_set_string (str1, one);
  s = t_str_get_string (str1);
  if (strcmp (s, one) != 0)
    g_print ("t_str_set_string or t_str_get_string didn't work.\n");
  if (s) g_free (s);

/* test for t_str_concat */
  str2 = t_str_new ();
  t_str_set_string (str2, two);
  str3 = t_str_concat (str1, str2);
  s = t_str_get_string (str3);
  if (strcmp (s, "onetwo") != 0) {
    g_print ("t_str_concat didn't work.\n");
    g_print ("%s was expected, but actual string was %s.\n", "onetwo", s);
  }
  if (s) g_free (s);
  if (str1) g_object_unref (str1);
  if (str2) g_object_unref (str2);
  if (str3) g_object_unref (str3);

/* ----- Test for TNumStr ----- */

  TNumStr *numstr;
  TNumber *num;
  int a, i, max;

  char *t_name[] = {
    "t_none",
    "t_int",
    "t_double"
  };
  struct str_type {
    char *s;
    int type;
  } sample [] = {
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

  max = sizeof(sample)/sizeof(struct str_type);

/* test for t_num_str_get_string_type */

  numstr = t_num_str_new ();
  for (i=0; i<max; ++i) {
    t_str_set_string (T_STR (numstr), sample[i].s);
    if ((a = t_num_str_get_string_type (numstr)) != sample[i].type) {
      if (t_none <= a && a <= t_double)
        g_print ("t_num_str_get_string_type returns %s for %s.\n", t_name[a], sample[i].s);
      else
        g_print ("t_num_str_get_string_type returns unexpected value %d.\n", a);
    }
  }
 
/* test for set/get "string" property for TNumStr */

  for (i=0; i<max; ++i) {
    g_object_set (T_STR (numstr), "string", sample[i].s, NULL);
    g_object_get (T_STR (numstr), "string", &s, NULL);
    if (strcmp (s, sample[i].s) != 0 || (a = t_num_str_get_string_type (numstr)) != sample[i].type) {
      if (t_none <= a && a <= t_double)
        g_print ("g_object_set/get didn't work. %s was expected, but actual returned value was %s and the type was %s.\n", sample[i].s, s, t_name[a]);
      else
        g_print ("g_object_set/get didn't work. %s was expected, but actual returned value was %s and the type was %d (illegal).\n", sample[i].s, s, a);
    }
    if (s) g_free (s);
  }
 
 /* test for t_num_str_set_from_t_number and t_num_str_get_t_number */
 
  for (i=0; i<max; ++i) {
    if (sample[i].type == t_int)
      num = T_NUMBER (t_int_new_with_value (atoi(sample[i].s)));
    else if (sample[i].type == t_double)
      num = T_NUMBER (t_double_new_with_value (atof(sample[i].s)));
    else
      num = NULL;
    if (T_IS_NUMBER (num))
      t_num_str_set_from_t_number (numstr, num);
    else
      numstr = NULL;
    a = numstr ? t_num_str_get_string_type (numstr) : t_none;
    if (num) g_object_unref (num);
    num = numstr ? t_num_str_get_t_number (numstr) : NULL;
    s = T_IS_NUMBER (num) ? t_number_to_s (num) : NULL;
    if (a != sample[i].type) {
      if (t_none <= a && a <= t_double)
        g_print ("Expected string type was %s, but t_num_str_get_string_type returned %s.\n", t_name[sample[i].type], t_name[a]);
      else
        g_print ("Expected string type was %s, but t_num_str_get_string_typel returned %d (illegal).\n", t_name[sample[i].type], a);
    }
    else if (sample[i].type == t_int && s && atoi (s) != atoi (sample[i].s))
      g_print ("%s was expected, but actual returned value was %s.\n", sample[i].s, s);
    else if (sample[i].type == t_double && s && atof (s) != atof (sample[i].s))
      g_print ("%s was expected, but actual returned value was %s.\n", sample[i].s, s);
    if (s) g_free (s);
    if (num) g_object_unref (num);
  }
  if (numstr) g_object_unref (numstr);

/* test for t_num_str_new_with_t_number */

  for (i=0; i<max; ++i) {
    if (sample[i].type == t_int)
      num = T_NUMBER (t_int_new_with_value (atoi(sample[i].s)));
    else if (sample[i].type == t_double)
      num = T_NUMBER (t_double_new_with_value (atof(sample[i].s)));
    else
      num = NULL;
    numstr = num ? t_num_str_new_with_tnumber (num) : NULL;
    a = numstr ? t_num_str_get_string_type (numstr) : t_none;
    if (num) g_object_unref (num);
    num = numstr ? t_num_str_get_t_number (numstr) : NULL;
    s = T_IS_NUMBER (num) ? t_number_to_s (num) : NULL;
    if (a != sample[i].type) {
      if (t_none <= a && a <= t_double)
        g_print ("Expected string type was %s, but t_num_str_get_string_type returned %s.\n", t_name[sample[i].type], t_name[a]);
      else
        g_print ("Expected string type was %s, but t_num_str_get_string_typel returned %d (illegal).\n", t_name[sample[i].type], a);
    } else if (sample[i].type == t_int && s && atoi (s) != atoi (sample[i].s))
      g_print ("%s was expected, but actual returned value was %s.\n", sample[i].s, s);
    else if (sample[i].type == t_double && s && atof (s) != atof (sample[i].s))
      g_print ("%s was expected, but actual returned value was %s.\n", sample[i].s, s);
    if (s) g_free (s);
    if (num) g_object_unref (num);
    if (numstr) g_object_unref (numstr);
  }

  return 0;
}
