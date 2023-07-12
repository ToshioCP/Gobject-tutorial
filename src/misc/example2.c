#include <glib-object.h>

static void
show_ref_count (GObject *instance) {
  if (G_IS_OBJECT (instance))
    /* Users should not use ref_count member in their program. */
    /* This is only for demonstration. */
    g_print ("Reference count is %d.\n", instance->ref_count);
  else
    g_print ("Instance is not GObject.\n");
}

int
main (int argc, char **argv) {
  GObject *instance;
  
  instance = g_object_new (G_TYPE_OBJECT, NULL);
  g_print ("Call g_object_new.\n");
  show_ref_count (instance);
  g_object_ref (instance);
  g_print ("Call g_object_ref.\n");
  show_ref_count (instance);
  g_object_unref (instance);
  g_print ("Call g_object_unref.\n");
  show_ref_count (instance);
  g_object_unref (instance);
  g_print ("Call g_object_unref.\n");
  g_print ("Now the reference count is zero and the instance is destroyed.\n");
  g_print ("The instance memories are possibly returned to the system.\n");
  g_print ("Therefore, the access to the same address may cause a segmentation error.\n");
  
  return 0;
}
