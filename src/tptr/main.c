#include <glib-object.h>
#include "tptr.h"

static void
notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
  const char *name;
  gpointer p;

  name = g_param_spec_get_name (pspec);
  if (T_IS_PTR (gobject) && strcmp (name, "pointer") == 0) {
    g_object_get (T_PTR (gobject), "pointer", &p, NULL);
    g_print ("Property \"%s\" is set to %p.\n", name, p);
  }
}

int
main (int argc, char **argv) {
  TPtr *ptr;

  ptr = t_ptr_new ();

  g_signal_connect (G_OBJECT (ptr), "notify::pointer", G_CALLBACK (notify_cb), NULL);

  t_ptr_set_pointer (ptr, NULL);
  t_ptr_set_pointer (ptr, ptr);
  g_print ("Get a pointer %p.\n", t_ptr_get_pointer (ptr));

  g_object_unref (ptr);

  return 0;
}

