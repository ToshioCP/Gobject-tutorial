#include "tptr.h"

#define PROP_PTR 1
static GParamSpec *ptr_property = NULL;

typedef struct {
  gpointer pointer;
} TPtrPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TPtr, t_ptr, G_TYPE_OBJECT)

static void
t_ptr_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  TPtr *self = T_PTR (object);
  TPtrPrivate *priv = t_ptr_get_instance_private (self);

  if (property_id == PROP_PTR) {
    priv->pointer = g_value_get_pointer (value);
  } else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_ptr_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  TPtr *self = T_PTR (object);
  TPtrPrivate *priv = t_ptr_get_instance_private (self);

  if (property_id == PROP_PTR)
    g_value_set_pointer (value, priv->pointer);
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
t_ptr_class_init (TPtrClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->set_property = t_ptr_set_property;
  gobject_class->get_property = t_ptr_get_property;
  ptr_property = g_param_spec_pointer ("pointer", "ptr", "pointer to anything", G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_PTR, ptr_property);
}

static void
t_ptr_init (TPtr *inst) {
  TPtrPrivate *priv = t_ptr_get_instance_private (inst);
  priv->pointer = NULL;
}

/* setter and getter */
void
t_ptr_set_pointer (TPtr *self, const gpointer p) {
  g_object_set (self, "pointer", p, NULL);
}

/* t_ptr_get_pointer doesn't use g_object_get */
/* Because this function is possibly used in finalize method of its child. */
/* g_object_get cannot be used in the finalization process. */
gpointer
t_ptr_get_pointer (TPtr *self) {
  TPtrPrivate *priv = t_ptr_get_instance_private (self);

  return priv->pointer;
}

TPtr *
t_ptr_new_with_pointer (gpointer p) {
  TPtr *ptr;

  ptr = g_object_new (T_TYPE_PTR, "pointer", p, NULL);
  return ptr;
}

TPtr *
t_ptr_new (void) {
  TPtr *p;

  p = g_object_new (T_TYPE_PTR, NULL);
  return p;
}

