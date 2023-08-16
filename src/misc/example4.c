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

G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)

static void
t_double_class_init (TDoubleClass *class) {
}

static void
t_double_init (TDouble *self) {
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
  g_object_unref (d);

  return 0;
}

