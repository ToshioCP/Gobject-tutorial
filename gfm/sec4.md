Up: [Readme.md](../Readme.md),  Prev: [Section 3](sec3.md), Next: [Section 5](sec5.md)

# Signals

## Signals

Signals provide a means of communication between objects.
Signals are emitted when something happens or completes.

The steps to program a signal is shown below. 

1. Register a signal.
A signal belongs to an object, so the registration is done in the class initialization function of the object.
2. Write a signal handler.
A signal handler is a function that is invoked when the signal is emitted.
3. Connect the signal and handler.
Signals are connected to handlers with `g_connect_signal` or its family functions.
4. Emit the signal.

Step one and Four are done on the object to which the signal belongs.
Step three is usually done outside the object.

The process of signals is complicated and it takes long to explain all the features.
The contents of this section is limited to the minimum things to write a simple signal and not necessarily accurate.
If you need an accurate information, refer to GObject API reference.
There are three parts which describe signals.

- [Type System Concepts, signals](https://docs.gtk.org/gobject/concepts.html#signals)
- [Old GObject Reference Manual, Signals](https://developer-old.gnome.org/gobject/stable/gobject-Signals.html)
- [GObject Tutorial, How to create and use signals](https://docs.gtk.org/gobject/tutorial.html#how-to-create-and-use-signals)

## Signal registration

An example in this section is a signal emitted when division-by-zero happens.
First, we need to determine the name of the signal.
Signal name consists of letters, digits, dash (`-`) and underscore (`_`).
The first character of the name must be a letter.
So, a string "div-by-zero"is appropriate for the signal name.

There are four functions to register a signal.
We will use `g_signal_new` for "div-by-zero" signal.

~~~C
guint
g_signal_new (const gchar *signal_name,
              GType itype,
              GSignalFlags signal_flags,
              guint class_offset,
              GSignalAccumulator accumulator,
              gpointer accu_data,
              GSignalCMarshaller c_marshaller,
              GType return_type,
              guint n_params,
              ...);
~~~

It needs a lot to explain each parameter.
At present I just show you `g_signal_new` function call extracted from `tdouble.c`.

~~~C
t_double_signal =
g_signal_new ("div-by-zero",
              G_TYPE_FROM_CLASS (class),
              G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
              0 /* class offset.Subclass cannot override the class handler (default handler). */,
              NULL /* accumulator */,
              NULL /* accumulator data */,
              NULL /* C marshaller. g_cclosure_marshal_generic() will be used */,
              G_TYPE_NONE /* return_type */,
              0     /* n_params */
              );
~~~

- `t_double_signal` is a static guint variable.
Guint type is the same as unsigned int.
It is set the signal id.
- The second parameter is the type (GType) of the object the signal belongs to.
`G_TYPE_FROM_CLASS (class)` returns the type corresponds to the class (`class` is a pointer to the class of the object).
- The third parameter is a signal flag.
Lots of pages are necessary to explain this flag.
So, I want leave them out now.
The argument above can be used in many cases.
The definition is described in the [GObject API Reference, SignalFlags](https://docs.gtk.org/gobject/flags.SignalFlags.html).
- The return type is G_TYPE_NONE which means no value is returned by the signal handler.
- `n_params` is a number of parameters.
This signal doesn't have parameters, so it is zero.

This function call is in the class initialization function.

You can use other functions such as `g_signal_newv`.
See [GObject API Reference](https://docs.gtk.org/gobject/func.signal_newv.html) for details.

## Signal handler

Signal handler is a function that is called when the signal is emitted.
The handler has two parameters.

- The instance to which the signal belongs
- A pointer to a user data which is given in the signal connection.

The "div-by-zero" signal doesn't need user data.

~~~C
void dev_by_zero_cb (TDouble *d, gpointer user_data) { ... ... ...};
~~~

Or, you can leave out the second parameter.

~~~C
void dev_by_zero_cb (TDouble *d) { ... ... ...};
~~~

If a signal has parameters, the parameters are between the instance and user data.
For example, the handler of "response" signal on GtkDialog is:

~~~C
void user_function (GtkDialog *dialog, int response_id, gpointer user_data);
~~~

`resoponse_id` is the parameter of the signal.
The "response" signal is emitted when a user clicks on a button like "OK" or "Cancel".
The argument of `response_id` is a value that shows what button has been clicked.

The handler of "div-by-zero" signal just shows an error message.

~~~C
static void
div_by_zero_cb (TDouble *d, gpointer user_data) {
  g_print ("\nError: division by zero.\n\n");
}
~~~

## Signal connection

A signal and a handler are connected with the function `g_signal_connect`.

~~~C
g_signal_connect (d1, "div-by-zero", G_CALLBACK (div_by_zero_cb), NULL);
~~~

- `d1` is an instance the signal belongs.
- The second argument is the signal name.
- The third argument is the signal handler.
It must be casted by `G_CALLBACK`.
- The last argument is an user data.
The signal doesn't need a user data, so NULL is assigned.

## Signal emission

Signals are emitted on the object.
The following is a part of `tdouble.c`.

~~~C
TDouble *
t_double_div (TDouble *self, TDouble *other) {
... ... ...
  if ((! t_double_get_value (other, &value)))
    return NULL;
  else if (value == 0) {
    g_signal_emit (self, t_double_signal, 0);
    return NULL;
  }
  return t_double_new (self->value / value);
}
~~~

If the divisor is zero, the signal is emitted.
`g_signal_emit` has three parameters.

- The first parameter is the instance that emits the signal.
- The second parameter is the signal id.
Signal id has been set with `g_signal_new` function.
- The third parameter is detail.
"dev-by-zero" signal doesn't have a detail, so the argument is zero.
Detail isn't explained in this section.
If you want to know details, refer to [GObject API Reference, Signal Detail](https://docs.gtk.org/gobject/concepts.html#the-detail-argument).

If a signal has parameters, they are fourth and subsequent arguments.

## Example code

A sample program is in [src/tdouble3](../src/tdouble3).

tdouble.c

~~~C
  1 #include "tdouble.h"
  2 
  3 static guint t_double_signal;
  4 
  5 struct _TDouble {
  6   GObject parent;
  7   double value;
  8 };
  9 
 10 G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)
 11 
 12 static void
 13 t_double_class_init (TDoubleClass *class) {
 14   t_double_signal = g_signal_new ("div-by-zero",
 15                                  G_TYPE_FROM_CLASS (class),
 16                                  G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
 17                                  0 /* class offset.Subclass cannot override the class handler (default handler). */,
 18                                  NULL /* accumulator */,
 19                                  NULL /* accumulator data */,
 20                                  NULL /* C marshaller. g_cclosure_marshal_generic() will be used */,
 21                                  G_TYPE_NONE /* return_type */,
 22                                  0     /* n_params */
 23                                  );
 24 }
 25 
 26 static void
 27 t_double_init (TDouble *d) {
 28 }
 29 
 30 /* getter and setter */
 31 gboolean
 32 t_double_get_value (TDouble *d, double *value) {
 33   g_return_val_if_fail (T_IS_DOUBLE (d), FALSE);
 34 
 35   *value = d->value;
 36   return TRUE;
 37 }
 38 
 39 void
 40 t_double_set_value (TDouble *d, double value) {
 41   g_return_if_fail (T_IS_DOUBLE (d));
 42 
 43   d->value = value;
 44 }
 45 
 46 /* arithmetic operator */
 47 /* These operators create a new instance and return a pointer to it. */
 48 #define t_double_binary_op(op) \
 49   if (! t_double_get_value (other, &value)) \
 50     return NULL; \
 51   return t_double_new (self->value op value);
 52 
 53 TDouble *
 54 t_double_add (TDouble *self, TDouble *other) {
 55   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 56   g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
 57   double value;
 58 
 59   t_double_binary_op (+)
 60 }
 61 
 62 TDouble *
 63 t_double_sub (TDouble *self, TDouble *other) {
 64   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 65   g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
 66   double value;
 67 
 68   t_double_binary_op (-)
 69 }
 70 
 71 TDouble *
 72 t_double_mul (TDouble *self, TDouble *other) {
 73   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 74   g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
 75   double value;
 76 
 77   t_double_binary_op (*)
 78 }
 79 
 80 TDouble *
 81 t_double_div (TDouble *self, TDouble *other) {
 82   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 83   g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
 84   double value;
 85 
 86   if ((! t_double_get_value (other, &value)))
 87     return NULL;
 88   else if (value == 0) {
 89     g_signal_emit (self, t_double_signal, 0);
 90     return NULL;
 91   }
 92   return t_double_new (self->value / value);
 93 }
 94 TDouble *
 95 t_double_uminus (TDouble *self) {
 96   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 97 
 98   return t_double_new (-self->value);
 99 }
100 
101 TDouble *
102 t_double_new (double value) {
103   TDouble *d;
104 
105   d = g_object_new (T_TYPE_DOUBLE, NULL);
106   d->value = value;
107   return d;
108 }
109 
~~~

main.c

~~~C
 1 #include <glib-object.h>
 2 #include "tdouble.h"
 3 
 4 static void
 5 div_by_zero_cb (TDouble *d, gpointer user_data) {
 6   g_print ("\nError: division by zero.\n\n");
 7 }
 8 
 9 static void
10 t_print (char *op, TDouble *d1, TDouble *d2, TDouble *d3) {
11   double v1, v2, v3;
12 
13   if (! t_double_get_value (d1, &v1))
14     return;
15   if (! t_double_get_value (d2, &v2))
16     return;
17   if (! t_double_get_value (d3, &v3))
18     return;
19 
20   g_print ("%lf %s %lf = %lf\n", v1, op, v2, v3);
21 }
22 
23 int
24 main (int argc, char **argv) {
25   TDouble *d1, *d2, *d3;
26   double v1, v3;
27 
28   d1 = t_double_new (10.0);
29   d2 = t_double_new (20.0);
30   if ((d3 = t_double_add (d1, d2)) != NULL) {
31     t_print ("+", d1, d2, d3);
32     g_object_unref (d3);
33   }
34 
35   if ((d3 = t_double_sub (d1, d2)) != NULL) {
36     t_print ("-", d1, d2, d3);
37     g_object_unref (d3);
38   }
39 
40   if ((d3 = t_double_mul (d1, d2)) != NULL) {
41     t_print ("*", d1, d2, d3);
42     g_object_unref (d3);
43   }
44 
45   if ((d3 = t_double_div (d1, d2)) != NULL) {
46     t_print ("/", d1, d2, d3);
47     g_object_unref (d3);
48   }
49 
50   g_signal_connect (d1, "div-by-zero", G_CALLBACK (div_by_zero_cb), NULL);
51   t_double_set_value (d2, 0.0);
52   if ((d3 = t_double_div (d1, d2)) != NULL) {
53     t_print ("/", d1, d2, d3);
54     g_object_unref (d3);
55   }
56 
57   if ((d3 = t_double_uminus (d1)) != NULL && (t_double_get_value (d1, &v1)) && (t_double_get_value (d3, &v3))) {
58     g_print ("-%lf = %lf\n", v1, v3);
59     g_object_unref (d3);
60   }
61 
62   g_object_unref (d1);
63   g_object_unref (d2);
64 
65   return 0;
66 }
67 
~~~

To compile, change your current directory to src/tdouble3 and type as follows.

~~~
$ meson _build
$ ninja -C _build
~~~

Then, Executable file `tdouble` is created in `_build` directory.
Execute it.

~~~
$ cd tdouble3; _build/tdouble
10.000000 + 20.000000 = 30.000000
10.000000 - 20.000000 = -10.000000
10.000000 * 20.000000 = 200.000000
10.000000 / 20.000000 = 0.500000

Error: division by zero.

-10.000000 = -10.000000
~~~

## Default signal handler

You may have thought that it was strange that the error message was set in `main.c`.
Indeed, the error happens in `tdouble.c` so the message should been managed by `tdouble.c` itself.
GObject system has a default signal handler that is set in the object itself.
A default signal handler is also called "default handler" or "object method handler".

You can set a default handler with `g_signal_new_class_handler`.

~~~c
guint
g_signal_new_class_handler (const gchar *signal_name,
                            GType itype,
                            GSignalFlags signal_flags,
                            GCallback class_handler, /*default signal handler */
                            GSignalAccumulator accumulator,
                            gpointer accu_data,
                            GSignalCMarshaller c_marshaller,
                            GType return_type,
                            guint n_params,
                            ...);
~~~

The difference from `g_signal_new` is the fourth parameter.
`g_signal_new` sets a default handler with the offset of the function pointer in the class structure.
If an object is derivable, it has its own class area, so you can set a default handler with `g_signal_new`.
But a final type object doesn't have its own class area, so it's impossible to set a default handler with `g_signal_new`.
That's the reason why we use `g_signal_new_class_handler`.

`tdouble.c` is changed.
`div_by_zero_default_cb` function is added and `g_signal_new_class_handler` replaces `g_signal_new`.
Default signal handler doesn't have `user_data` parameter.
A `user_data` parameter is set with `g_signal_connect` family functions when user provided signal handler is connected to the signal.
Because default signal handler isn't connected with `g_signal_connect` family function, no user data is given as an argument.

~~~C
 1 static void
 2 div_by_zero_default_cb (TDouble *d) {
 3   g_print ("\nError: division by zero.\n\n");
 4 }
 5 
 6 static void
 7 t_double_class_init (TDoubleClass *class) {
 8   t_double_signal =
 9   g_signal_new_class_handler ("div-by-zero",
10                               G_TYPE_FROM_CLASS (class),
11                               G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
12                               G_CALLBACK (div_by_zero_default_cb),
13                               NULL /* accumulator */,
14                               NULL /* accumulator data */,
15                               NULL /* C marshaller */,
16                               G_TYPE_NONE /* return_type */,
17                               0     /* n_params */
18                               );
19 }
~~~

`g_signal_connect` and `div_by_zero_cb` are removed from `main.c`.

Compile and execute it.

~~~
$ cd tdouble4; _build/tdouble
10.000000 + 20.000000 = 30.000000
10.000000 - 20.000000 = -10.000000
10.000000 * 20.000000 = 200.000000
10.000000 / 20.000000 = 0.500000

Error: division by zero.

-10.000000 = -10.000000
~~~

The source file is in the directory [src/tdouble4](../src/tdouble4).

If you want to connect your handler (user-provided handler) to the signal, you can still use `g_signal_connect`.
Add the following in `main.c`.

~~~C
static void
div_by_zero_cb (TDouble *d, gpointer user_data) {
  g_print ("\nError happens in main.c.\n");
}

int
main (int argc, char **argv) {
... ... ...
  g_signal_connect (d1, "div-by-zero", G_CALLBACK (div_by_zero_cb), NULL);
... ... ...
}
~~~

Then, both the user-provided handler and default handler are called when the signal is emitted.
Compile and execute it, then the following is shown on your display.

~~~
10.000000 + 20.000000 = 30.000000
10.000000 - 20.000000 = -10.000000
10.000000 * 20.000000 = 200.000000
10.000000 / 20.000000 = 0.500000

Error happens in main.c.

Error: division by zero.

-10.000000 = -10.000000
~~~

This tells us that the user-provided handler is called first, then the default handler is called.
If you want your handler called after the default handler, then you can use `g_signal_connect_after`.
Add the lines below to `main.c` again.

~~~C
static void
div_by_zero_after_cb (TDouble *d, gpointer user_data) {
  g_print ("\nError has happened in main.c and an error message has been displayed.\n");
}

int
main (int argc, char **argv) {
... ... ...
  g_signal_connect_after (d1, "div-by-zero", G_CALLBACK (div_by_zero_after_cb), NULL);
... ... ...
}
~~~

Compile and execute it, then:

~~~
10.000000 + 20.000000 = 30.000000
10.000000 - 20.000000 = -10.000000
10.000000 * 20.000000 = 200.000000
10.000000 / 20.000000 = 0.500000

Error happens in main.c.

Error: division by zero.

Error has happened in main.c and an error message has been displayed.

-10.000000 = -10.000000
~~~

The source files are in [src/tdouble5](../src/tdouble5).

## Signal flag

The order that handlers are called is described in [GObject API Reference, Sigmal emission](https://docs.gtk.org/gobject/concepts.html#signal-emission).

The order depends on the signal flag which is set in `g_signal_new` or `g_signal_new_class_handler`.
There are three flags which relate to the order of handlers' invocation.

- `G_SIGNAL_RUN_FIRST`: the default handler is called before any user provided handler.
- `G_SIGNAL_RUN_LAST`: the default handler is called after the normal user provided handler (not connected with `g_signal_connect_after`).
- `G_SIGNAL_RUN_CLEANUP`: the default handler is called after any user provided handler.

`G_SIGNAL_RUN_LAST` is the most appropriate in many cases.

Other signal flags are described in [GObject API Rreference](https://docs.gtk.org/gobject/flags.SignalFlags.html).


Up: [Readme.md](../Readme.md),  Prev: [Section 3](sec3.md), Next: [Section 5](sec5.md)
