#include <glib-object.h>
#include "tcomparable.h"
#include "../tnumber/tnumber.h"
#include "../tnumber/tint.h"
#include "../tnumber/tdouble.h"
#include "../tstr/tstr.h"

static void
t_print (const char *cmp, TComparable *c1, TComparable *c2) {
  char *s1, *s2;
  TStr *ts1, *ts2, *ts3;

  ts1 = t_str_new_with_string("\"");
  if (T_IS_NUMBER (c1))
    s1 = t_number_to_s (T_NUMBER (c1));
  else if (T_IS_STR (c1)) {
    ts2 = t_str_concat (ts1, T_STR (c1));
    ts3 = t_str_concat (ts2, ts1);
    s1 = t_str_get_string (T_STR (ts3));
    g_object_unref (ts2);
    g_object_unref (ts3);
  } else {
    g_print ("c1 isn't TInt, TDouble nor TStr.\n");
    return;
  }
  if (T_IS_NUMBER (c2))
    s2 = t_number_to_s (T_NUMBER (c2));
  else if (T_IS_STR (c2)) {
    ts2 = t_str_concat (ts1, T_STR (c2));
    ts3 = t_str_concat (ts2, ts1);
    s2 = t_str_get_string (T_STR (ts3));
    g_object_unref (ts2);
    g_object_unref (ts3);
  } else {
    g_print ("c2 isn't TInt, TDouble nor TStr.\n");
    return;
  }
  g_print ("%s %s %s.\n", s1, cmp, s2);
  g_object_unref (ts1);
  g_free (s1);
  g_free (s2);
}    

static void
compare (TComparable *c1, TComparable *c2) {
  if (t_comparable_eq (c1, c2))
    t_print ("equals", c1, c2);
  else if (t_comparable_gt (c1, c2))
    t_print ("is greater than", c1, c2);
  else if (t_comparable_lt (c1, c2))
    t_print ("is less than", c1, c2);
  else if (t_comparable_ge (c1, c2))
    t_print ("is greater than or equal to", c1, c2);
  else if (t_comparable_le (c1, c2))
    t_print ("is less than or equal to", c1, c2);
  else
    t_print ("can't compare to", c1, c2);
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  const char *three = "three";
  TInt *i;
  TDouble *d;
  TStr *str1, *str2, *str3;

  i = t_int_new_with_value (124);
  d = t_double_new_with_value (123.45);
  str1 = t_str_new_with_string (one);
  str2 = t_str_new_with_string (two);
  str3 = t_str_new_with_string (three);

  compare (T_COMPARABLE (i), T_COMPARABLE (d));
  compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
  compare (T_COMPARABLE (str2), T_COMPARABLE (str3));
  compare (T_COMPARABLE (i), T_COMPARABLE (str1));

  g_object_unref (i);
  g_object_unref (d);
  g_object_unref (str1);
  g_object_unref (str2);
  g_object_unref (str3);

  return 0;
}
