#include <glib-object.h>
#include "tdouble.h"

int
main (int argc, char **argv) {
  TDouble *d;
  double value;

  d = t_double_new (10.0);
  if (t_double_get_value (d, &value))
    g_print ("t_double_get_value succesfully assigned %lf to value.\n", value);
  else
    g_print ("t_double_get_value failed.\n");

  t_double_set_value (d, -20.0);
  g_print ("Now, set d (tDouble object) with %lf.\n", -20.0);
  if (t_double_get_value (d, &value))
    g_print ("t_double_get_value succesfully assigned %lf to value.\n", value);
  else
    g_print ("t_double_get_value failed.\n");
  g_object_unref (d);

  return 0;
}

