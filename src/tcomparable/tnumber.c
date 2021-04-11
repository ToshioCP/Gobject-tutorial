#include "tnumber.h"

static guint t_number_signal;

G_DEFINE_ABSTRACT_TYPE (TNumber, t_number, G_TYPE_OBJECT)

static void
div_by_zero_default_cb (TNumber *self) {
  g_print ("\nError: division by zero.\n\n");
}

static void
t_number_class_init (TNumberClass *class) {

  /* virtual functions */
  class->add = NULL;
  class->sub = NULL;
  class->mul = NULL;
  class->div = NULL;
  class->uminus = NULL;
  class->to_s = NULL;
  /* default signal handler */
  class->div_by_zero = div_by_zero_default_cb;
  /* signal */
  t_number_signal =
  g_signal_new ("div-by-zero",
                G_TYPE_FROM_CLASS (class),
                G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                G_STRUCT_OFFSET (TNumberClass, div_by_zero),
                NULL /* accumulator */,
                NULL /* accumulator data */,
                NULL /* C marshaller */,
                G_TYPE_NONE /* return_type */,
                0     /* n_params */,
                NULL  /* param_types */);
}

static void
t_number_init (TNumber *inst) {
}

TNumber *
t_number_add (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_NUMBER (self), NULL);
  g_return_val_if_fail (T_IS_NUMBER (other), NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->add ? class->add (self, other) : NULL;
}

TNumber *
t_number_sub (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_NUMBER (self), NULL);
  g_return_val_if_fail (T_IS_NUMBER (other), NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->sub ? class->sub (self, other) : NULL;
}

TNumber *
t_number_mul (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_NUMBER (self), NULL);
  g_return_val_if_fail (T_IS_NUMBER (other), NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->mul ? class->mul (self, other) : NULL;
}

TNumber *
t_number_div (TNumber *self, TNumber *other) {
  g_return_val_if_fail (T_IS_NUMBER (self), NULL);
  g_return_val_if_fail (T_IS_NUMBER (other), NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->div ? class->div (self, other) : NULL;
}

TNumber *
t_number_uminus (TNumber *self) {
  g_return_val_if_fail (T_IS_NUMBER (self), NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->uminus ? class->uminus (self) : NULL;
}

char *
t_number_to_s (TNumber *self) {
  g_return_val_if_fail (T_IS_NUMBER (self), NULL);

  TNumberClass *class = T_NUMBER_GET_CLASS(self);
  return class->to_s ? class->to_s (self) : NULL;
}

