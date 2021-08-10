Up: [Readme.md](../Readme.md),  Prev: [Section 8](sec8.md)

# Interface

Interface is similar to abstract class.
Interface defines virtual functions which are expected to be overridden by a function in another instantiable object.

This section provides a simple example, TComparable.
TComparable is an interface.
It defines functions to compare.
They are:

- `t_comparable_cmp (self, other)` compares `self` and `other`.
`self` is an instance on which `t_comparable_cmp` runs.
`other` is another instance.
This function needs to be overridden in an object which implements the interface.
  - If `self` is equal to `other`, `t_comparable_cmp` returns 0.
  - If `self` is greater than `other`, `t_comparable_cmp` returns 1.
  - If `self` is less than `other`, `t_comparable_cmp` returns -1.
  - If an error happens, `t_comparable_cmp` returns -2.
- `t_comparable_eq (self, other)` returns TRUE if `self` is equal to `other`.
Otherwise it returns FALSE.
You need to be careful that FALSE is returned even if an error occurs.
- `t_comparable_gt (self, other)` returns TRUE if `self` is greater than `other`.
Otherwise it returns FALSE.
- `t_comparable_lt (self, other)` returns TRUE if `self` is less than `other`.
Otherwise it returns FALSE.
- `t_comparable_ge (self, other)` returns TRUE if `self` is greater than or equal to `other`.
Otherwise it returns FALSE.
- `t_comparable_le (self, other)` returns TRUE if `self` is less than or equal to `other`.
Otherwise it returns FALSE.

Numbers and strings are comparable.
TInt, TDouble, TStr and TNumStr implement TComparable interface so that they can use the functions above.
For example,

~~~C
TInt *i1 = t_int_new_with_value (10);
TInt *i2 = t_int_new_with_value (20);
t_comparable_eq (T_COMPARABLE (i1), T_COMPARABLE (i2)); /* It returns FALSE */
t_comparable_lt (T_COMPARABLE (i1), T_COMPARABLE (i2)); /* It returns TRUE */
~~~

What's the difference between interface and abstract class?
Virtual functions in an abstract class are overridden by a function in its descendant classes.
Virtual functions in an interface are overridden by a function in any classes.
Compare TNumber and TComparable.

- A function `t_number_add` is overridden in TIntClass and TDoubleClass.
It can't be overridden in TStrClass because TStr isn't a descendant of TNumber.
- A function `t_comparable_cmp` is overridden in TIntClass, TDoubleClass, TStrClass and TNumStrClass.

## TComparable interface

Defining interfaces is similar to define objects.

- Use `G_DECLARE_INTERFACE` instead of `G_DEFINE_FINAL_TYPE`.
- Use `G_DEFINE_INTERFACE` instead of `G_DEFINE_TYPE`.

Now let's see the header file.

~~~C
 1 #ifndef __T_COMPARABLE_H__
 2 #define __T_COMPARABLE_H__
 3 
 4 #include <glib-object.h>
 5 
 6 #define T_TYPE_COMPARABLE  (t_comparable_get_type ())
 7 G_DECLARE_INTERFACE (TComparable, t_comparable, T, COMPARABLE, GObject)
 8 
 9 struct _TComparableInterface {
10   GTypeInterface parent;
11   /* signal */
12   void (*arg_error) (TComparable *self);
13   /* virtual function */
14   int (*cmp) (TComparable *self, TComparable *other);
15 };
16 
17 /* t_comparable_cmp */
18 /* if self > other, then returns 1 */
19 /* if self = other, then returns 0 */
20 /* if self < other, then returns -1 */
21 /* if error happens, then returns -2 */
22 
23 int
24 t_comparable_cmp (TComparable *self, TComparable *other);
25 
26 gboolean
27 t_comparable_eq (TComparable *self, TComparable *other);
28 
29 gboolean
30 t_comparable_gt (TComparable *self, TComparable *other);
31 
32 gboolean
33 t_comparable_lt (TComparable *self, TComparable *other);
34 
35 gboolean
36 t_comparable_ge (TComparable *self, TComparable *other);
37 
38 gboolean
39 t_comparable_le (TComparable *self, TComparable *other);
40 
41 #endif /* __T_COMPARABLE_H__ */
42 
~~~

- 7: `G_DECLARE_INTERFACE` macro.
The last parameter is a prerequisite of the interface.
The prerequisite of TComparable is GObject.
So, any other object than the descendants of GObject, for example GVarinat, can't implement TComparable.
A prerequisite is the GType of either an interface or a class.
This macro expands to:
  - Declaration of `t_comparable_get_type()`.
  - `Typedef struct _TComparableInterface TComparableInterface`
  - `T_COMPARABLE ()` macro. It casts an instance to TComparable type.
  - `T_IS_COMPARABLE ()` macro. It checks if the type of an instance is `T_TYPE_COMPARABLE`.
  - `T_COMPARABLE_GET_IFACE ()` macro. It gets the interface of the instance which is given as an argument.
- 9-15: `TComparableInterface` structure.
This is similar to a class structure.
The first member is the parent interface.
The parent of `TComparableInterface` is `GTypeInterface`.
`GTypeInterface` is a base of all interface types.
It is like a `GTypeClass` which is a base of all class types.
`GTypeClass` is the first member of the structure `GObjectClass`.
(See `gobject.h`. Note that `GObjecClass` is the same as `struct _GObjectClass`.)
The next member is a pointer `arg_error` to the default signal handler of "arg-error" signal.
This signal is emitted when an argument of the public functions isn't TComparable.
The last member is a pointer to a method `cmp`.
It is a virtual function and is expected to be overridden by a function in the object which implements the interface.
- 23-39: Public functions.

C file `tcomparable.c` is as follows.

~~~C
  1 #include "tcomparable.h"
  2 
  3 static guint t_comparable_signal;
  4 
  5 G_DEFINE_INTERFACE (TComparable, t_comparable, G_TYPE_OBJECT)
  6 
  7 static void
  8 arg_error_default_cb (TComparable *self) {
  9   g_printerr ("\nTComparable: argument error.\n");
 10 }
 11 
 12 static void
 13 t_comparable_default_init (TComparableInterface *iface) {
 14   /* virtual functio */
 15   iface->cmp = NULL;
 16   /* argument error signal */
 17   iface->arg_error = arg_error_default_cb;
 18   t_comparable_signal =
 19   g_signal_new ("arg-error",
 20                 T_TYPE_COMPARABLE,
 21                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
 22                 G_STRUCT_OFFSET (TComparableInterface, arg_error),
 23                 NULL /* accumulator */,
 24                 NULL /* accumulator data */,
 25                 NULL /* C marshaller */,
 26                 G_TYPE_NONE /* return_type */,
 27                 0     /* n_params */
 28                 );
 29 }
 30 
 31 int
 32 t_comparable_cmp (TComparable *self, TComparable *other) {
 33   if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
 34     g_signal_emit (self, t_comparable_signal, 0);
 35   g_return_val_if_fail (T_IS_COMPARABLE (self), -2);
 36   g_return_val_if_fail (T_IS_COMPARABLE (other), -2);
 37 
 38   int result;
 39   TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);
 40 
 41   g_return_val_if_fail (iface->cmp, -2);
 42   if ((result = iface->cmp (self, other)) == -2)
 43     g_signal_emit (self, t_comparable_signal, 0);
 44   return result;
 45 }
 46 
 47 gboolean
 48 t_comparable_eq (TComparable *self, TComparable *other) {
 49   if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
 50     g_signal_emit (self, t_comparable_signal, 0);
 51   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
 52   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
 53 
 54   if (t_comparable_cmp (self, other) == 0)
 55     return TRUE;
 56   else
 57     return FALSE;
 58 }
 59 
 60 gboolean
 61 t_comparable_gt (TComparable *self, TComparable *other) {
 62   if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
 63     g_signal_emit (self, t_comparable_signal, 0);
 64   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
 65   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
 66 
 67   if (t_comparable_cmp (self, other) == 1)
 68     return TRUE;
 69   else
 70     return FALSE;
 71 }
 72 
 73 gboolean
 74 t_comparable_lt (TComparable *self, TComparable *other) {
 75   if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
 76     g_signal_emit (self, t_comparable_signal, 0);
 77   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
 78   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
 79 
 80   if (t_comparable_cmp (self, other) == -1)
 81     return TRUE;
 82   else
 83     return FALSE;
 84 }
 85 
 86 gboolean
 87 t_comparable_ge (TComparable *self, TComparable *other) {
 88   if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
 89     g_signal_emit (self, t_comparable_signal, 0);
 90   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
 91   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
 92 
 93 
 94   int result = t_comparable_cmp (self, other);
 95   if (result == 1 || result == 0)
 96     return TRUE;
 97   else
 98     return FALSE;
 99 }
100 
101 gboolean
102 t_comparable_le (TComparable *self, TComparable *other) {
103   if ((! T_IS_COMPARABLE (self)) || (! T_IS_COMPARABLE (other)))
104     g_signal_emit (self, t_comparable_signal, 0);
105   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
106   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
107 
108 
109   int result = t_comparable_cmp (self, other);
110   if (result == -1 || result == 0)
111     return TRUE;
112   else
113     return FALSE;
114 }
115 
~~~

- 5: `G_DEFINE_INTERFACE` macro.
The third parameter is the type of the prerequisite.
This macro expands to:
  - Declaration of `t_comparable_default_init ()`.
  - Definition of `t_comparable_get_type ()`.
- 7-10: `arg_error_default_cb` is a default signal handler of "arg-error" signal.
- 12- 29: `t_comparable_default_init` function.
This function is similar to class initialization function.
It initializes `TComparableInterface` structure.
- 15: Assigns NULL to the pointer to `cmp` method.
So, the method doesn't work before an implementation object overrides it.
- 17: Set the default signal handler of the signal "arg-error".
- 18-28: Creates a signal "arg-error".
This signal is emitted when an argument for a public function is invalid.
- 31-45: `t_comparable_cmp` function.
First four lines (33-36) check the arguments.
If one of them is invalid, it emits "arg-error" signal and outputs an error log, then returns -2. (-2 indicates an error.)
If `iface->cmp` is NULL (it means the class method hasn't been overridden), then outputs an error log and returns -2.
Otherwise it calls the class method.
If `result` equals -2, an error has occurred.
Then, it emits "arg-error" signal.
Returns `result`.
- 47-114: Public functions.
These five functions are based on `t_comparable_cmp`.
Therefore, no overriding is necessary for the functions.
For example, `t_comparable_eq` just calls `t_comparable_cmp`.
And if the return value is zero, then it returns TRUE.
Otherwise it returns FALSE.

This program uses a signal to give the argument type error information to a user.
This error is usually a program error rather than a run-time error.
Using a signal to report a program error is not a good way.
The best way is using `g_return_if_fail`.
The reason why I made this signal is just I wanted to show how to implement a signal in an interface.

## Implementing interface

TInt, TDouble, TStr and TNumStr implement TComparable.
First, look at TInt.
The header file is the same as before.
The implementation is written in C file.

`tint.c'`is as follows.

~~~C
  1 #include "tint.h"
  2 #include "tdouble.h"
  3 #include "tcomparable.h"
  4 
  5 #define PROP_INT 1
  6 static GParamSpec *int_property = NULL;
  7 
  8 struct _TInt {
  9   TNumber parent;
 10   int value;
 11 };
 12 
 13 static void t_comparable_interface_init (TComparableInterface *iface);
 14 
 15 G_DEFINE_TYPE_WITH_CODE (TInt, t_int, T_TYPE_NUMBER,
 16                          G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))
 17 
 18 static int
 19 t_int_comparable_cmp (TComparable *self, TComparable *other) {
 20   g_return_val_if_fail (T_IS_INT (self), -2);
 21   g_return_val_if_fail (T_IS_INT (other) || T_IS_DOUBLE (other), -2);
 22 
 23   int i;
 24   double s, o;
 25 
 26   g_object_get (self, "value", &i, NULL);
 27   s = (double) i;
 28   if (T_IS_INT (other)) {
 29     g_object_get (other, "value", &i, NULL);
 30     o = (double) i;
 31   } else
 32     g_object_get (other, "value", &o, NULL);
 33   if (s > o)
 34     return 1;
 35   else if (s == o)
 36     return 0;
 37   else if (s < o)
 38     return -1;
 39   else /* This can't happen. */
 40     return -2;
 41 }
 42 
 43 static void
 44 t_comparable_interface_init (TComparableInterface *iface) {
 45   iface->cmp = t_int_comparable_cmp;
 46 }
 47 
 48 static void
 49 t_int_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 50   TInt *self = T_INT (object);
 51 
 52   if (property_id == PROP_INT) {
 53     self->value = g_value_get_int (value);
 54   } else
 55     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 56 }
 57 
 58 static void
 59 t_int_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 60   TInt *self = T_INT (object);
 61 
 62   if (property_id == PROP_INT)
 63     g_value_set_int (value, self->value);
 64   else
 65     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 66 }
 67 
 68 static void
 69 t_int_init (TInt *d) {
 70 }
 71 
 72 /* arithmetic operator */
 73 /* These operators create a new instance and return a pointer to it. */
 74 #define t_int_binary_op(op) \
 75   int i; \
 76   double d; \
 77   if (T_IS_INT (other)) { \
 78     g_object_get (T_INT (other), "value", &i, NULL); \
 79     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op i)); \
 80   } else { \
 81     g_object_get (T_DOUBLE (other), "value", &d, NULL); \
 82     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op (int) d)); \
 83   }
 84 
 85 static TNumber *
 86 t_int_add (TNumber *self, TNumber *other) {
 87   g_return_val_if_fail (T_IS_INT (self), NULL);
 88 
 89   t_int_binary_op (+)
 90 }
 91 
 92 static TNumber *
 93 t_int_sub (TNumber *self, TNumber *other) {
 94   g_return_val_if_fail (T_IS_INT (self), NULL);
 95 
 96   t_int_binary_op (-)
 97 }
 98 
 99 static TNumber *
100 t_int_mul (TNumber *self, TNumber *other) {
101   g_return_val_if_fail (T_IS_INT (self), NULL);
102 
103   t_int_binary_op (*)
104 }
105 
106 static TNumber *
107 t_int_div (TNumber *self, TNumber *other) {
108   g_return_val_if_fail (T_IS_INT (self), NULL);
109   int i;
110   double d;
111 
112   if (T_IS_INT (other)) {
113     g_object_get (T_INT (other), "value", &i, NULL);
114     if (i == 0) {
115       g_signal_emit_by_name (self, "div-by-zero");
116       return NULL;
117     } else
118       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / i));
119   } else {
120     g_object_get (T_DOUBLE (other), "value", &d, NULL);
121     if (d == 0) {
122       g_signal_emit_by_name (self, "div-by-zero");
123       return NULL;
124     } else
125       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / (int)  d));
126   }
127 }
128 
129 static TNumber *
130 t_int_uminus (TNumber *self) {
131   g_return_val_if_fail (T_IS_INT (self), NULL);
132 
133   return T_NUMBER (t_int_new_with_value (- T_INT(self)->value));
134 }
135 
136 static char *
137 t_int_to_s (TNumber *self) {
138   g_return_val_if_fail (T_IS_INT (self), NULL);
139   int i;
140 
141   g_object_get (T_INT (self), "value", &i, NULL); 
142   return g_strdup_printf ("%d", i);
143 }
144 
145 static void
146 t_int_class_init (TIntClass *class) {
147   TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
148   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
149 
150   /* override virtual functions */
151   tnumber_class->add = t_int_add;
152   tnumber_class->sub = t_int_sub;
153   tnumber_class->mul = t_int_mul;
154   tnumber_class->div = t_int_div;
155   tnumber_class->uminus = t_int_uminus;
156   tnumber_class->to_s = t_int_to_s;
157 
158   gobject_class->set_property = t_int_set_property;
159   gobject_class->get_property = t_int_get_property;
160   int_property = g_param_spec_int ("value", "val", "Integer value", G_MININT, G_MAXINT, 0, G_PARAM_READWRITE);
161   g_object_class_install_property (gobject_class, PROP_INT, int_property);
162 }
163 
164 TInt *
165 t_int_new_with_value (int value) {
166   TInt *d;
167 
168   d = g_object_new (T_TYPE_INT, "value", value, NULL);
169   return d;
170 }
171 
172 TInt *
173 t_int_new (void) {
174   TInt *d;
175 
176   d = g_object_new (T_TYPE_INT, NULL);
177   return d;
178 }
179 
~~~

- 3: It needs to include the header file of TComparable.
- 13: Declaration of `t_comparable_interface_init ()` function.
This declaration is done before `G_DEFINE_TYPE_WITH_CODE` macro.
- 15-16: `G_DEFINE_TYPE_WITH_CODE` macro.
The last parameter is `G_IMPLEMENT_INTERFACE` macro.
The third parameter of `G_IMPLEMENT_INTERFACE` is `t_comparabele_interface_init`.
These two macros expands to:
  - Declares `t_int_class_init ()` function.
  - Declares `t_int_init ()` function.
  - Defines `t_int_parent_class` static variable which points to the parent's class.
  - Defines `t_int_get_type ()` function.
This function includes `g_type_register_static_simple ()` and `g_type_add_interface_static ()`.
`g_type_register_static_simple ()` is a convenient version of `g_type_register_static ()`.
It registers TInt type to the type system.
`g_type_add_interface_static ()` adds an interface type to an instance type.
There is a good example in [GObject manual, Interfaces](https://developer-old.gnome.org/gobject/stable/gtype-non-instantiatable-classed.html).
- 18-41: `t_int_comparable_cmp` is a function to compare TInt instances.
- 26: Gets the value of `self` and cast it to double.
- 27-32: Gets the value of `other` and if it is TInt then the value is casted to double.
If `other` is TDouble, no cast is needed.
- 33-40: compares `s` and `o` and returns 1, 0, -1 and -2.
- 43-46: `t_comparable_interface_init`.
This function is called in the initialization process of TInt.
`t_int_comparable_cmp` overrides `cmp` method.
- 48-178: The same as before.

`tdouble.c` is almost same as `tint.c`.
These two objects can be compared because int is casted to double before the comparison.

`tstr.c` is as follows.

~~~C
  1 #include "tstr.h"
  2 #include "tcomparable.h"
  3 
  4 enum {
  5   PROP_0,
  6   PROP_STRING,
  7   N_PROPERTIES
  8 };
  9 
 10 static GParamSpec *str_properties[N_PROPERTIES] = {NULL, };
 11 
 12 typedef struct {
 13   char *string;
 14 } TStrPrivate;
 15 
 16 static void t_comparable_interface_init (TComparableInterface *iface);
 17 
 18 G_DEFINE_TYPE_WITH_CODE (TStr, t_str, G_TYPE_OBJECT,
 19                          G_ADD_PRIVATE (TStr)
 20                          G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))
 21 
 22 static void
 23 t_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 24   TStr *self = T_STR (object);
 25   TStrPrivate *priv = t_str_get_instance_private (self);
 26 
 27 
 28   if (property_id == PROP_STRING) {
 29     if (priv->string)
 30       g_free (priv->string);
 31     priv->string = g_strdup (g_value_get_string (value));
 32   } else
 33     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 34 }
 35 
 36 static void
 37 t_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 38   TStr *self = T_STR (object);
 39   TStrPrivate *priv = t_str_get_instance_private (self);
 40 
 41   if (property_id == PROP_STRING)
 42     g_value_set_string (value, g_strdup (priv->string));
 43   else
 44     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 45 }
 46 
 47 static void
 48 t_str_finalize (GObject *object) {
 49   TStr *self = T_STR (object);
 50   TStrPrivate *priv = t_str_get_instance_private (self);
 51 
 52   if (priv->string)
 53     g_free (priv->string);
 54   G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
 55 }
 56 
 57 static int
 58 t_str_comparable_cmp (TComparable *self, TComparable *other) {
 59   g_return_val_if_fail (T_IS_STR (self), -2);
 60   g_return_val_if_fail (T_IS_STR (other), -2);
 61 
 62   char *s, *o;
 63   int result;
 64 
 65   s = t_str_get_string (T_STR (self));
 66   o = t_str_get_string (T_STR (other));
 67 
 68   if (strcmp (s, o) > 0)
 69     result = 1;
 70   else if (strcmp (s, o) == 0)
 71     result = 0;
 72   else if (strcmp (s, o) < 0)
 73     result = -1;
 74   else /* This can't happen. */
 75     result = -2;
 76   g_free (s);
 77   g_free (o);
 78   return result;
 79 }
 80 
 81 static void
 82 t_comparable_interface_init (TComparableInterface *iface) {
 83   iface->cmp = t_str_comparable_cmp;
 84 }
 85 
 86 static void
 87 t_str_init (TStr *self) {
 88   TStrPrivate *priv = t_str_get_instance_private (self);
 89 
 90   priv->string = NULL;
 91 }
 92 
 93 static void
 94 t_str_class_init (TStrClass *class) {
 95   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
 96 
 97   gobject_class->finalize = t_str_finalize;
 98   gobject_class->set_property = t_str_set_property;
 99   gobject_class->get_property = t_str_get_property;
100   str_properties[PROP_STRING] = g_param_spec_string ("string", "str", "string", "", G_PARAM_READWRITE);
101   g_object_class_install_properties (gobject_class, N_PROPERTIES, str_properties);
102 }
103 
104 /* setter and getter */
105 void
106 t_str_set_string (TStr *self, const char *s) {
107   g_return_if_fail (T_IS_STR (self));
108 
109   g_object_set (self, "string", s, NULL);
110 }
111 
112 char *
113 t_str_get_string (TStr *self) {
114   g_return_val_if_fail (T_IS_STR (self), NULL);
115 
116   char *s;
117 
118   g_object_get (self, "string", &s, NULL);
119   return s;
120 }
121 
122 TStr *
123 t_str_concat (TStr *self, TStr *other) {
124   g_return_val_if_fail (T_IS_STR (self), NULL);
125   g_return_val_if_fail (T_IS_STR (other), NULL);
126 
127   char *s1, *s2, *s3;
128   TStr *str;
129 
130   s1 = t_str_get_string (self);
131   s2 = t_str_get_string (other);
132   if (s1 && s2)
133     s3 = g_strconcat (s1, s2, NULL);
134   else if (s1)
135     s3 = g_strdup (s1);
136   else if (s2)
137     s3 = g_strdup (s2);
138   else
139     s3 = NULL;
140   str = t_str_new_with_string (s3);
141   if (s1) g_free (s1);
142   if (s2) g_free (s2);
143   if (s3) g_free (s3);
144   return str;
145 }
146 
147 /* create a new TStr instance */
148 TStr *
149 t_str_new_with_string (const char *s) {
150   return T_STR (g_object_new (T_TYPE_STR, "string", s, NULL));
151 }
152 
153 TStr *
154 t_str_new (void) {
155   return T_STR (g_object_new (T_TYPE_STR, NULL));
156 }
157 
~~~

- 16: Declares `t_comparable_interface_init` function.
This needs to be declared before `G_DEFINE_TYPE_WITH_CODE` macro.
- 18-20: `G_DEFINE_TYPE_WITH_CODE` macro.
Because TStr is derivable type, its private area (TStrPrivate) is needed.
`G_ADD_PRIVATE` macro makes the private area.
Be careful that there's no comma after `G_ADD_PRIVATE` macro.
- 57-79: `t_str_comparable_cmp`.
First, strings `s` and `o` are got from TStr objects `self` and `other`.
Then, the C standard function `strcmp` is used to compare strings.
After that, `s` and `o` are freed.

TStr can be compared with TStr, but not with TInt nor TDouble.
Generally, comparison is available between two same type instances.

TNumStr implements TComparable.
The program `tnumstr.c` is similar to `tint.c`.
Its `cmp` function compares two numeric strings based on their numbers.
Therefore, "100" is bigger than "50".
(If you compare them as TStr, "100" is smaller than "50".)
The source file is in the `src/tcomparable` directory.
If you want to know the details, see [`src/tcomparable/tnumstr.c](../src/tcomparable/tnumstr.c).

## Test program.

`main.c` is a test program.

~~~C
  1 #include <glib-object.h>
  2 #include "tcomparable.h"
  3 #include "tnumber.h"
  4 #include "tint.h"
  5 #include "tdouble.h"
  6 #include "tstr.h"
  7 #include "tnumstr.h"
  8 
  9 
 10 static void
 11 notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
 12   const char *name;
 13   int i;
 14   double d;
 15   char *s;
 16 
 17   name = g_param_spec_get_name (pspec);
 18   if (T_IS_INT (gobject) && strcmp (name, "value") == 0) {
 19     g_object_get (T_INT (gobject), "value", &i, NULL);
 20     g_print ("Property \"%s\" is set to %d.\n", name, i);
 21   } else if (T_IS_DOUBLE (gobject) && strcmp (name, "value") == 0) {
 22     g_object_get (T_DOUBLE (gobject), "value", &d, NULL);
 23     g_print ("Property \"%s\" is set to %lf.\n", name, d);
 24   } else if (T_IS_STR (gobject) && strcmp (name, "string") == 0) {
 25     s = t_str_get_string (T_STR (gobject));
 26     g_print ("Property \"%s\" is set to %s.\n", name, s);
 27   }
 28 }
 29 
 30 static void
 31 t_print (const char *cmp, TComparable *c1, TComparable *c2) {
 32   char *s1, *s2;
 33 
 34   if (T_IS_NUMBER (c1))
 35     s1 = t_number_to_s (T_NUMBER (c1));
 36   else if (T_IS_STR (c1))
 37     s1 = t_str_get_string (T_STR (c1));
 38   else {
 39     g_print ("c1 isn't TInt, TDouble nor TStr.\n");
 40     return;
 41   }
 42   if (T_IS_NUMBER (c2))
 43     s2 = t_number_to_s (T_NUMBER (c2));
 44   else if (T_IS_STR (c2))
 45     s2 = t_str_get_string (T_STR (c2));
 46   else {
 47     g_print ("c2 isn't TInt, TDouble nor TStr.\n");
 48     return;
 49   }
 50   if (T_IS_STR (c1))
 51     g_print ("\"%s\" %s \"%s\".\n", s1, cmp, s2);
 52   else
 53     g_print ("%s %s %s.\n", s1, cmp, s2);
 54   g_free (s1);
 55   g_free (s2);
 56 }    
 57 
 58 static void
 59 compare (TComparable *c1, TComparable *c2) {
 60   if (t_comparable_eq (c1, c2))
 61     t_print ("equals", c1, c2);
 62   if (t_comparable_gt (c1, c2))
 63     t_print ("is greater than", c1, c2);
 64   if (t_comparable_lt (c1, c2))
 65     t_print ("is less than", c1, c2);
 66   if (t_comparable_ge (c1, c2))
 67     t_print ("is greater than or equal to", c1, c2);
 68   if (t_comparable_le (c1, c2))
 69     t_print ("is less than or equal to", c1, c2);
 70 }
 71 
 72 int
 73 main (int argc, char **argv) {
 74   const char *one = "one";
 75   const char *two = "two";
 76   TInt *i;
 77   TDouble *d;
 78   TStr *str1, *str2;
 79   TNumStr *numstr1, *numstr2;
 80   gpointer obj;
 81 
 82   i = t_int_new ();
 83   d = t_double_new ();
 84   str1 = t_str_new ();
 85   str2 = t_str_new ();
 86   numstr1 = t_num_str_new ();
 87   numstr2 = t_num_str_new ();
 88   obj = g_object_new (G_TYPE_OBJECT, NULL);
 89 
 90   g_signal_connect (G_OBJECT (i), "notify::value", G_CALLBACK (notify_cb), NULL);
 91   g_signal_connect (G_OBJECT (d), "notify::value", G_CALLBACK (notify_cb), NULL);
 92   g_signal_connect (G_OBJECT (str1), "notify::string", G_CALLBACK (notify_cb), NULL);
 93   g_signal_connect (G_OBJECT (str2), "notify::string", G_CALLBACK (notify_cb), NULL);
 94   g_signal_connect (G_OBJECT (numstr1), "notify::string", G_CALLBACK (notify_cb), NULL);
 95   g_signal_connect (G_OBJECT (numstr2), "notify::string", G_CALLBACK (notify_cb), NULL);
 96 
 97   g_object_set (i, "value", 124, NULL);
 98   g_object_set (d, "value", 123.45, NULL);
 99   t_str_set_string (str1, one);
100   t_str_set_string (str2, two);
101   t_num_str_set (numstr1, T_NUMBER (i));
102   t_num_str_set (numstr2, T_NUMBER (d));
103 
104   compare (T_COMPARABLE (i), T_COMPARABLE (d));
105   compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
106   compare (T_COMPARABLE (numstr1), T_COMPARABLE (numstr2));
107   t_comparable_eq (T_COMPARABLE (d), obj);
108 
109   g_print ("\n");
110   t_str_set_string (str1, "100");
111   t_str_set_string (str2, "50");
112   t_str_set_string (T_STR (numstr1), "100");
113   t_str_set_string (T_STR (numstr2), "50");
114   compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
115   compare (T_COMPARABLE (numstr1), T_COMPARABLE (numstr2));
116 
117   g_object_unref (i);
118   g_object_unref (d);
119   g_object_unref (str1);
120   g_object_unref (str2);
121   g_object_unref (numstr1);
122   g_object_unref (numstr2);
123   g_object_unref (obj);
124 
125   return 0;
126 }
127 
~~~

- 10-28: "notify" signal handler is extended to support three types of objects: TInt, TDouble and TStr.
Because TNumStr is a child of TStr, TNumStr instance is also supported.
- 30-56: This function `t_print` uses `to_s` function to convert TInt and/or TDouble to a string.
Then, displays the result of the comparison of two TComparable objects.
- 58-70: `compare` compares two TComparable objects and calls `t_print` to display the result.
- 72-125: `main` function.
- 82-88: Creates TInt, TDouble, two TStr, two TNumStr and GObject instances.
- 90-95: Connects "notify" signal.
- 97-102: Set values and strings of TInt, TDouble, TStr and TNumStr.
- 104: Compares TInt and TDouble.
- 105: Compares two TStr.
- 106: Compares two TNumStr.
- 107: Check equality between TDouble and GObject.
This makes an error because GObject doesn't implement TComparable.
- 109-115: Compare "100" and "50" as TStr first, then as TNumStr.
"100" is smaller than "50" if they are TStr.
"100" is bigger than "50" if they are TNumStr.
- 117-123: Frees objects.

## Compilation and execution

Change your current directory to [src/tcomparable](../src/comparable).

~~~
$ meson _build
$ ninja -C _build
~~~

Then execute it.

~~~
$ cd tcomparable; _build/tcomparable
Property "value" is set to 124.
Property "value" is set to 123.450000.
Property "string" is set to one.
Property "string" is set to two.
Property "string" is set to 124.
Property "string" is set to 123.450000.
124 is greater than 123.450000.
124 is greater than or equal to 123.450000.
"one" is less than "two".
"one" is less than or equal to "two".
"124" is greater than "123.450000".
"124" is greater than or equal to "123.450000".

TComparable: argument error.

** (process:7737): CRITICAL **: 13:08:42.673: t_comparable_eq: assertion 'T_IS_COMPARABLE (other)' failed

Property "string" is set to 100.
Property "string" is set to 50.
Property "string" is set to 100.
Property "string" is set to 50.
"100" is less than "50".
"100" is less than or equal to "50".
"100" is greater than "50".
"100" is greater than or equal to "50".
~~~

The lines from `TComparable: argument error.` to `** (process:7737)...` show that the comparison with TDouble and GObject fails.

## Build an interface without macros

We used macros such as `G_DECLARE_INTERFACE`, `G_DEFINE_INTERFACE` to build an interface.
We can also build it without macros.
There are two files in the `tcomparable` directory.

- `tcomparable_without_macro.h`
- `tcomparable_without_macro.c`

These two files don't use the macros.
Instead, they register the interface to the type system directly.
If you want to know that, see the source files in [src/tcomparable](../src/tcomparable).


## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the language are similar.
Learning such languages is very useful to know GObject system.


Up: [Readme.md](../Readme.md),  Prev: [Section 8](sec8.md)
