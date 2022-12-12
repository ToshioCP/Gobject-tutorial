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
  if (strcmp (name, "string") == 0) {
    g_print ("String property is set to %s.\n", (s = t_str_get_string(T_STR (gobject))));
    if (s) g_free (s);
  }
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  TStr *str1, *str2, *str3;
  char *s;

  str1 = t_str_new ();
  g_signal_connect (G_OBJECT (str1), "notify::string", G_CALLBACK (notify_cb), NULL);
  g_object_set (G_OBJECT (str1), "string", one, NULL);
  str2 = t_str_new ();
  g_object_set (G_OBJECT (str2), "string", two, NULL);
  str3 = t_str_concat (str1, str2);
  s = t_str_get_string (str3);
  g_print ("\"%s\" and \"%s\" is \"%s\".\n", one, two, s);
  if (s) g_free (s);
  if (str1) g_object_unref (str1);
  if (str2) g_object_unref (str2);
  if (str3) g_object_unref (str3);

  TNumStr *numstr[3];
  TNumber *num[3], *sum, *temp;
  const char *str[] = {"123", "456", "789"};
  int i;

  for (i=0; i<3; ++i) {
    numstr[i] = t_num_str_new ();
    t_str_set_string (T_STR (numstr[i]), str[i]);
    num[i] = t_num_str_get_t_number (numstr[i]);
  }
  temp = t_number_add (num[0], num[1]);
  sum = t_number_add (temp, num[2]);
  s = t_number_to_s (sum);
  g_print ("%s + %s + %s = %s\n", str[0], str[1], str[2], s);
  if (temp) g_object_unref (temp);
  if (sum) g_object_unref (sum);
  if (s) g_free (s);
  for (i=0; i<3; ++i) {
    if (numstr[i])
      g_object_unref (numstr[i]);
    if (num[i])
      g_object_unref (num[i]);
  }

  char *string[] = {"123", "-45", "+0", "123.456", "+123.456", "-123.456", ".456", "123.", "0.0", "123.4567890123456789", "abc", NULL};
  int max = sizeof(string)/sizeof(char*);
  TNumStr *ns = t_num_str_new();
  TNumber *n;
  char *s1;

  g_print ("TNumStr => TNumber => TNumStr\n");
  for (i=0; i<max; ++i) {
    t_str_set_string (T_STR (ns), string[i]);
    n = t_num_str_get_t_number (ns);
    if (n) {
      t_num_str_set_from_t_number (ns, n);
      s = t_number_to_s (n);
    } else
      s = NULL;
    s1 = t_str_get_string (T_STR (ns));
    g_print ("%s => %s => %s\n", string[i], s, s1);
    if (n) g_object_unref (n);
    if (s) g_free (s);
    if (s1) g_free (s1);
  }
  g_object_unref (ns);

  return 0;
}
