#include <glib-object.h>
#include "tstr.h"


static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  char *s;

  name = g_param_spec_get_name (pspec);
  if (T_IS_STR (gobject) && strcmp (name, "string") == 0) {
    g_object_get (T_STR (gobject), "string", &s, NULL);
    g_print ("Property \"%s\" is set to %s.\n", name, s);
  }
}

int
main (int argc, char **argv) {
  const char *one = "one";
  const char *two = "two";
  const char *three = "three";
  const char *four = "four";
  TStr *s1, *s2, *s3;

  s1 = t_str_new_with_string (one);
  s2 = t_str_new_with_string (two);

  g_signal_connect (G_OBJECT (s1), "notify::string", G_CALLBACK (notify_cb), NULL);
  g_signal_connect (G_OBJECT (s2), "notify::string", G_CALLBACK (notify_cb), NULL);

  g_object_set (s1, "string", three, NULL);
  g_object_set (s2, "string", four, NULL);

  s3 = t_str_concat (s1, s2);
  if (s1)
    g_print ("s1 is \"%s\".\n", t_str_get_string (s1));
  else
    g_print ("s1 is NULL.\n");
  if (s2)
    g_print ("s2 is \"%s\".\n", t_str_get_string (s2));
  else
    g_print ("s2 is NULL.\n");
  if (s3)
    g_print ("s3 is \"%s\".\n", t_str_get_string (s3));
  else
    g_print ("s3 is NULL.\n");

  g_object_unref (s1);
  g_object_unref (s2);
  g_object_unref (s3);

  return 0;
}

