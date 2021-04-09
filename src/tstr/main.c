#include <glib-object.h>
#include "tstr.h"


static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  char *s;

  name = g_param_spec_get_name (pspec);
  if (T_IS_STR (gobject) && strcmp (name, "pointer") == 0) {
    g_object_get (T_PTR (gobject), "pointer", &s, NULL);
    g_print ("Property \"%s\" is set to %s.\n", name, s);
  }
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  TStr *str1, *str2, *str3;
  char *s1, *s2, *s3;

  str1 = t_str_new ();
  str2 = t_str_new ();

  g_signal_connect (G_OBJECT (str1), "notify::pointer", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (str2), "notify::pointer", G_CALLBACK (notify_cb), NULL);

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
  g_free (s1);
  g_free (s2);
  g_free (s3);

  return 0;
}

