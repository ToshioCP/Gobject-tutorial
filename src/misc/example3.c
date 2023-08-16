#include <glib-object.h>

#define T_TYPE_DOUBLE  (t_double_get_type ())

typedef struct _TDouble TDouble;
struct _TDouble {
  GObject parent;
  double value;
};

typedef struct _TDoubleClass TDoubleClass;
struct _TDoubleClass {
  GObjectClass parent_class;
};

static void
t_double_class_init (TDoubleClass *class) {
}

static void
t_double_init (TDouble *self) {
}

GType
t_double_get_type (void) {
  static GType type = 0;
  GTypeInfo info;

  if (type == 0) {
    info.class_size = sizeof (TDoubleClass);
    info.base_init = NULL;
    info.base_finalize = NULL;
    info.class_init = (GClassInitFunc)  t_double_class_init;
    info.class_finalize = NULL;
    info.class_data = NULL;
    info.instance_size = sizeof (TDouble);
    info.n_preallocs = 0;
    info.instance_init = (GInstanceInitFunc)  t_double_init;
    info.value_table = NULL;
    type = g_type_register_static (G_TYPE_OBJECT, "TDouble", &info, 0);
  }
  return type;
}

int
main (int argc, char **argv) {
  GType dtype;
  TDouble *d;

  dtype = t_double_get_type (); /* or dtype = T_TYPE_DOUBLE */
  if (dtype)
    g_print ("Registration was a success. The type is %lx.\n", dtype);
  else
    g_print ("Registration failed.\n");

  d = g_object_new (T_TYPE_DOUBLE, NULL);
  if (d)
    g_print ("Instantiation was a success. The instance address is %p.\n", d);
  else
    g_print ("Instantiation failed.\n");
  g_object_unref (d); /* Releases the object d. */

  return 0;
}

