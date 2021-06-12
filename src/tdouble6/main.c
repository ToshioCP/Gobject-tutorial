#include <glib-object.h>
#include "tdouble.h"

static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  double v;

  name = g_param_spec_get_name (pspec);
  if (T_IS_DOUBLE (gobject) && strcmp (name, "value") == 0) {
    g_object_get (T_DOUBLE (gobject), "value", &v, NULL);
    g_print ("Property \"%s\" is set to %lf.\n", name, v);
  }
}

static double
get_value (TDouble *d) {
  double v;
  g_object_get (d, "value", &v, NULL);
  return v;
}

static void
t_print (char *op, TDouble *d1, TDouble *d2, TDouble *d3) {
  g_print ("%lf %s %lf = %lf\n", get_value(d1), op, get_value(d2), get_value(d3));
}

int
main (int argc, char **argv) {
  TDouble *d1, *d2, *d3;

  d1 = t_double_new_with_value (10.0);
  d2 = t_double_new_with_value (20.0);
  g_signal_connect (G_OBJECT (d1), "notify::value", G_CALLBACK (notify_cb), NULL);
  if ((d3 = t_double_add (d1, d2)) != NULL) {
    t_print ("+", d1, d2, d3);
    g_object_unref (d3);
  }

  if ((d3 = t_double_sub (d1, d2)) != NULL) {
    t_print ("-", d1, d2, d3);
    g_object_unref (d3);
  }

  if ((d3 = t_double_mul (d1, d2)) != NULL) {
    t_print ("*", d1, d2, d3);
    g_object_unref (d3);
  }

  if ((d3 = t_double_div (d1, d2)) != NULL) {
    t_print ("/", d1, d2, d3);
    g_object_unref (d3);
  }

  g_object_set (d2, "value", 0.0, NULL);
  if ((d3 = t_double_div (d1, d2)) != NULL) {
    t_print ("/", d1, d2, d3);
    g_object_unref (d3);
  }

  if ((d3 = t_double_uminus (d1)) != NULL) {
    g_print ("-(%lf) = %lf\n", get_value(d1), get_value(d3));
    g_object_unref (d3);
  }

  g_object_set (d1, "value", 100.0, NULL);

  g_object_unref (d1);
  g_object_unref (d2);

  return 0;
}

