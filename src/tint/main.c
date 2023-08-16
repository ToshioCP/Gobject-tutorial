#include <glib-object.h>
#include "tint.h"

static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  int v;

  name = g_param_spec_get_name (pspec);
  if (T_IS_INT (gobject) && strcmp (name, "value") == 0) {
    g_object_get (T_INT (gobject), "value", &v, NULL);
    g_print ("Property \"%s\" is set to %d.\n", name, v);
  }
}

static int
get_value (TInt *self) {
  int v;
  g_object_get (self, "value", &v, NULL);
  return v;
}

static void
t_print (char *op, TInt *i1, TInt *i2, TInt *i3) {
  g_print ("%d %s %d = %d\n", get_value(i1), op, get_value(i2), get_value(i3));
}

int
main (int argc, char **argv) {
  TInt *i1, *i2, *i3;

  i1 = t_int_new_with_value (10);
  i2 = t_int_new_with_value (20);

  g_signal_connect (G_OBJECT (i1), "notify::value", G_CALLBACK (notify_cb), NULL);
  if ((i3 = t_int_add (i1, i2)) != NULL) {
    t_print ("+", i1, i2, i3);
    g_object_unref (i3);
  }

  if ((i3 = t_int_sub (i1, i2)) != NULL) {
    t_print ("-", i1, i2, i3);
    g_object_unref (i3);
  }

  if ((i3 = t_int_mul (i1, i2)) != NULL) {
    t_print ("*", i1, i2, i3);
    g_object_unref (i3);
  }

  if ((i3 = t_int_div (i1, i2)) != NULL) {
    t_print ("/", i1, i2, i3);
    g_object_unref (i3);
  }

  g_object_set (i2, "value", 0, NULL);
  if ((i3 = t_int_div (i1, i2)) != NULL) {
    t_print ("/", i1, i2, i3);
    g_object_unref (i3);
  }

  if ((i3 = t_int_uminus (i1)) != NULL) {
    g_print ("-(%d) = %d\n", get_value(i1), get_value(i3));
    g_object_unref (i3);
  }

  g_object_set (i1, "value", 100, NULL);

  g_object_unref (i1);
  g_object_unref (i2);

  return 0;
}
