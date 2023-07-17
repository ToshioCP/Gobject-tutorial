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
TInt, TDouble and TStr implement TComparable interface so that they can use the functions above.
In addition, TNumStr can use the functions because it is a child class of TStr.

For example,

~~~C
TInt *i1 = t_int_new_with_value (10);
TInt *i2 = t_int_new_with_value (20);
t_comparable_eq (T_COMPARABLE (i1), T_COMPARABLE (i2)); /* => FALSE */
t_comparable_lt (T_COMPARABLE (i1), T_COMPARABLE (i2)); /* => TRUE */
~~~

What's the difference between interface and abstract class?
Virtual functions in an abstract class are overridden by a function in its descendant classes.
Virtual functions in an interface are overridden by a function in any classes.
Compare TNumber and TComparable.

- A function `t_number_add` is overridden in TIntClass and TDoubleClass.
It can't be overridden in TStrClass because TStr isn't a descendant of TNumber.
- A function `t_comparable_cmp` is overridden in TIntClass, TDoubleClass and TStrClass.

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
So, any other object than the descendants of GObject, for example GVariant, can't implement TComparable.
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
(See `gobject.h`. Note that `GObjectClass` is the same as `struct _GObjectClass`.)
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
14   /* virtual function */
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
33   g_return_val_if_fail (T_IS_COMPARABLE (self), -2);
34   if (! T_IS_COMPARABLE (other))
35     g_signal_emit (self, t_comparable_signal, 0);
36   g_return_val_if_fail (T_IS_COMPARABLE (other), -2);
37 
38   TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);
39 
40   g_return_val_if_fail (iface->cmp, -2);
41   return iface->cmp (self, other);
42 }
43 
44 gboolean
45 t_comparable_eq (TComparable *self, TComparable *other) {
46   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
47   if (! T_IS_COMPARABLE (other))
48     g_signal_emit (self, t_comparable_signal, 0);
49   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
50 
51   return (t_comparable_cmp (self, other) == 0);
52 }
53 
54 gboolean
55 t_comparable_gt (TComparable *self, TComparable *other) {
56   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
57   if (! T_IS_COMPARABLE (other))
58     g_signal_emit (self, t_comparable_signal, 0);
59   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
60 
61   return (t_comparable_cmp (self, other) == 1);
62 }
63 
64 gboolean
65 t_comparable_lt (TComparable *self, TComparable *other) {
66   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
67   if (! T_IS_COMPARABLE (other))
68     g_signal_emit (self, t_comparable_signal, 0);
69   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
70 
71   return (t_comparable_cmp (self, other) == -1);
72 }
73 
74 gboolean
75 t_comparable_ge (TComparable *self, TComparable *other) {
76   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
77   if (! T_IS_COMPARABLE (other))
78     g_signal_emit (self, t_comparable_signal, 0);
79   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
80 
81 
82   int result = t_comparable_cmp (self, other);
83   return (result == 1 || result == 0);
84 }
85 
86 gboolean
87 t_comparable_le (TComparable *self, TComparable *other) {
88   g_return_val_if_fail (T_IS_COMPARABLE (self), FALSE);
89   if (! T_IS_COMPARABLE (other))
90     g_signal_emit (self, t_comparable_signal, 0);
91   g_return_val_if_fail (T_IS_COMPARABLE (other), FALSE);
92 
93 
94   int result = t_comparable_cmp (self, other);
95   return (result == -1 || result == 0);
96 }
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
- 31-42: `t_comparable_cmp` function.
First four lines (33-36) check the arguments.
If `self` is invalid, it logs the error message and returns -2 (error).
If the argument is invalid, it emits "arg-error" signal.
Then it outputs an error log, then returns -2.
If `iface->cmp` is NULL (it means the class method hasn't been overridden), then outputs an error log and returns -2.
Otherwise it calls the class method and returns directly the value returned by the class method.
- 44-96: Public functions.
These five functions are based on `t_comparable_cmp`.
Therefore, no overriding is necessary for the functions.
For example, `t_comparable_eq` just calls `t_comparable_cmp`.
And if the return value is zero, then it returns TRUE.
Otherwise it returns FALSE.

This program uses a signal to give the argument type error information to a user.
This error is usually a program error rather than a run-time error.
Using a signal to report a program error is not a good way.
The best way is using `g_return_if_fail`.
The reason why I made this signal is just I wanted to show how to implement a signal in interfaces.

## Implementing interface

TInt, TDouble and TStr implement TComparable.
First, look at TInt.
The header file is the same as before.
The implementation is written in C file.

`tint.c` is as follows.

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
 21   if (! T_IS_NUMBER (other))
 22     g_signal_emit_by_name (self, "arg-error");
 23   g_return_val_if_fail (T_IS_NUMBER (other), -2);
 24 
 25   int i;
 26   double s, o;
 27 
 28   s = (double) T_INT (self)->value;
 29   if (T_IS_INT (other)) {
 30     g_object_get (other, "value", &i, NULL);
 31     o = (double) i;
 32   } else
 33     g_object_get (other, "value", &o, NULL);
 34   if (s > o)
 35     return 1;
 36   else if (s == o)
 37     return 0;
 38   else if (s < o)
 39     return -1;
 40   else /* This can't happen. */
 41     return -2;
 42 }
 43 
 44 static void
 45 t_comparable_interface_init (TComparableInterface *iface) {
 46   iface->cmp = t_int_comparable_cmp;
 47 }
 48 
 49 static void
 50 t_int_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 51   TInt *self = T_INT (object);
 52 
 53   if (property_id == PROP_INT) {
 54     self->value = g_value_get_int (value);
 55   } else
 56     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 57 }
 58 
 59 static void
 60 t_int_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 61   TInt *self = T_INT (object);
 62 
 63   if (property_id == PROP_INT)
 64     g_value_set_int (value, self->value);
 65   else
 66     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 67 }
 68 
 69 static void
 70 t_int_init (TInt *i) {
 71 }
 72 
 73 /* arithmetic operator */
 74 /* These operators create a new instance and return a pointer to it. */
 75 #define t_int_binary_op(op) \
 76   int i; \
 77   double d; \
 78   if (T_IS_INT (other)) { \
 79     g_object_get (T_INT (other), "value", &i, NULL); \
 80     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op i)); \
 81   } else { \
 82     g_object_get (T_DOUBLE (other), "value", &d, NULL); \
 83     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op (int) d)); \
 84   }
 85 
 86 static TNumber *
 87 t_int_add (TNumber *self, TNumber *other) {
 88   g_return_val_if_fail (T_IS_INT (self), NULL);
 89 
 90   t_int_binary_op (+)
 91 }
 92 
 93 static TNumber *
 94 t_int_sub (TNumber *self, TNumber *other) {
 95   g_return_val_if_fail (T_IS_INT (self), NULL);
 96 
 97   t_int_binary_op (-)
 98 }
 99 
100 static TNumber *
101 t_int_mul (TNumber *self, TNumber *other) {
102   g_return_val_if_fail (T_IS_INT (self), NULL);
103 
104   t_int_binary_op (*)
105 }
106 
107 static TNumber *
108 t_int_div (TNumber *self, TNumber *other) {
109   g_return_val_if_fail (T_IS_INT (self), NULL);
110   int i;
111   double d;
112 
113   if (T_IS_INT (other)) {
114     g_object_get (T_INT (other), "value", &i, NULL);
115     if (i == 0) {
116       g_signal_emit_by_name (self, "div-by-zero");
117       return NULL;
118     } else
119       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / i));
120   } else {
121     g_object_get (T_DOUBLE (other), "value", &d, NULL);
122     if (d == 0) {
123       g_signal_emit_by_name (self, "div-by-zero");
124       return NULL;
125     } else
126       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / (int)  d));
127   }
128 }
129 
130 static TNumber *
131 t_int_uminus (TNumber *self) {
132   g_return_val_if_fail (T_IS_INT (self), NULL);
133 
134   return T_NUMBER (t_int_new_with_value (- T_INT(self)->value));
135 }
136 
137 static char *
138 t_int_to_s (TNumber *self) {
139   g_return_val_if_fail (T_IS_INT (self), NULL);
140   int i;
141 
142   g_object_get (T_INT (self), "value", &i, NULL); 
143   return g_strdup_printf ("%d", i);
144 }
145 
146 static void
147 t_int_class_init (TIntClass *class) {
148   TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
149   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
150 
151   /* override virtual functions */
152   tnumber_class->add = t_int_add;
153   tnumber_class->sub = t_int_sub;
154   tnumber_class->mul = t_int_mul;
155   tnumber_class->div = t_int_div;
156   tnumber_class->uminus = t_int_uminus;
157   tnumber_class->to_s = t_int_to_s;
158 
159   gobject_class->set_property = t_int_set_property;
160   gobject_class->get_property = t_int_get_property;
161   int_property = g_param_spec_int ("value", "val", "Integer value", G_MININT, G_MAXINT, 0, G_PARAM_READWRITE);
162   g_object_class_install_property (gobject_class, PROP_INT, int_property);
163 }
164 
165 TInt *
166 t_int_new_with_value (int value) {
167   TInt *i;
168 
169   i = g_object_new (T_TYPE_INT, "value", value, NULL);
170   return i;
171 }
172 
173 TInt *
174 t_int_new (void) {
175   TInt *i;
176 
177   i = g_object_new (T_TYPE_INT, NULL);
178   return i;
179 }
~~~

- 3: It needs to include the header file of TComparable.
- 13: Declaration of `t_comparable_interface_init ()` function.
This declaration is done before `G_DEFINE_TYPE_WITH_CODE` macro.
- 15-16: `G_DEFINE_TYPE_WITH_CODE` macro.
The last parameter is `G_IMPLEMENT_INTERFACE` macro.
The third parameter of `G_IMPLEMENT_INTERFACE` is `t_comparable_interface_init`.
These two macros expands to:
  - Declares `t_int_class_init ()` function.
  - Declares `t_int_init ()` function.
  - Defines `t_int_parent_class` static variable which points to the parent's class.
  - Defines `t_int_get_type ()` function.
This function includes `g_type_register_static_simple ()` and `g_type_add_interface_static ()`.
`g_type_register_static_simple ()` is a convenient version of `g_type_register_static ()`.
It registers TInt type to the type system.
`g_type_add_interface_static ()` adds an interface type to an instance type.
There is a good example in [GObject Reference Manual, Interfaces](https://docs.gtk.org/gobject/concepts.html#non-instantiatable-classed-types-interfaces).
If you want to know how to write codes without the macros, see [`tint_without_macro.c`](../src/tcomparble/tint_without_macro.c).
- 18-42: `t_int_comparable_cmp` is a function to compare TInt instances.
- 20-23: checks `self` and the argument `other`.
If the argument type is not TNumber, it emits "arg-error" signal with `g_signal_emit_by_name`.
- 28: converts `self` into double.
- 29-33: Gets the value of `other` and if it is TInt then the value is casted to double.
- 34-41: compares `s` and `o` and returns 1, 0, -1 and -2.
- 44-47: `t_comparable_interface_init`.
This function is called in the initialization process of TInt.
`t_int_comparable_cmp` overrides `cmp` method.
- 49-179: The same as before.

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
 25 
 26   if (property_id == PROP_STRING) {
 27     t_str_set_string(self, g_value_get_string (value));
 28   } else
 29     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 30 }
 31 
 32 static void
 33 t_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 34   TStr *self = T_STR (object);
 35 
 36   if (property_id == PROP_STRING)
 37     g_value_set_string (value, t_str_get_string(self));
 38   else
 39     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 40 }
 41 
 42 static void
 43 t_str_real_set_string (TStr *self, const char *s) {
 44   TStrPrivate *priv = t_str_get_instance_private (self);
 45 
 46   if (priv->string)
 47     g_free (priv->string);
 48   priv->string = g_strdup (s);
 49 }
 50 
 51 static void
 52 t_str_finalize (GObject *object) {
 53   TStr *self = T_STR (object);
 54   TStrPrivate *priv = t_str_get_instance_private (self);
 55 
 56   if (priv->string)
 57     g_free (priv->string);
 58   G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
 59 }
 60 
 61 static int
 62 t_str_comparable_cmp (TComparable *self, TComparable *other) {
 63   g_return_val_if_fail (T_IS_STR (self), -2);
 64   if (! T_IS_STR (other))
 65     g_signal_emit_by_name (self, "arg-error");
 66   g_return_val_if_fail (T_IS_STR (other), -2);
 67 
 68   char *s, *o;
 69   int result;
 70 
 71   s = t_str_get_string (T_STR (self));
 72   o = t_str_get_string (T_STR (other));
 73 
 74   if (strcmp (s, o) > 0)
 75     result = 1;
 76   else if (strcmp (s, o) == 0)
 77     result = 0;
 78   else if (strcmp (s, o) < 0)
 79     result = -1;
 80   else /* This can't happen. */
 81     result = -2;
 82   g_free (s);
 83   g_free (o);
 84   return result;
 85 }
 86 
 87 static void
 88 t_comparable_interface_init (TComparableInterface *iface) {
 89   iface->cmp = t_str_comparable_cmp;
 90 }
 91 
 92 static void
 93 t_str_init (TStr *self) {
 94   TStrPrivate *priv = t_str_get_instance_private (self);
 95 
 96   priv->string = NULL;
 97 }
 98 
 99 static void
100 t_str_class_init (TStrClass *class) {
101   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
102 
103   gobject_class->finalize = t_str_finalize;
104   gobject_class->set_property = t_str_set_property;
105   gobject_class->get_property = t_str_get_property;
106   str_properties[PROP_STRING] = g_param_spec_string ("string", "str", "string", "", G_PARAM_READWRITE);
107   g_object_class_install_properties (gobject_class, N_PROPERTIES, str_properties);
108 
109   class->set_string = t_str_real_set_string;
110 }
111 
112 /* setter and getter */
113 void
114 t_str_set_string (TStr *self, const char *s) {
115   g_return_if_fail (T_IS_STR (self));
116   TStrClass *class = T_STR_GET_CLASS (self);
117 
118   class->set_string (self, s);
119 }
120 
121 char *
122 t_str_get_string (TStr *self) {
123   g_return_val_if_fail (T_IS_STR (self), NULL);
124   TStrPrivate *priv = t_str_get_instance_private (self);
125 
126   return g_strdup (priv->string);
127 }
128 
129 TStr *
130 t_str_concat (TStr *self, TStr *other) {
131   g_return_val_if_fail (T_IS_STR (self), NULL);
132   g_return_val_if_fail (T_IS_STR (other), NULL);
133 
134   char *s1, *s2, *s3;
135   TStr *str;
136 
137   s1 = t_str_get_string (self);
138   s2 = t_str_get_string (other);
139   if (s1 && s2)
140     s3 = g_strconcat (s1, s2, NULL);
141   else if (s1)
142     s3 = g_strdup (s1);
143   else if (s2)
144     s3 = g_strdup (s2);
145   else
146     s3 = NULL;
147   str = t_str_new_with_string (s3);
148   if (s1) g_free (s1);
149   if (s2) g_free (s2);
150   if (s3) g_free (s3);
151   return str;
152 }
153 
154 /* create a new TStr instance */
155 TStr *
156 t_str_new_with_string (const char *s) {
157   return T_STR (g_object_new (T_TYPE_STR, "string", s, NULL));
158 }
159 
160 TStr *
161 t_str_new (void) {
162   return T_STR (g_object_new (T_TYPE_STR, NULL));
163 }
164 
~~~

- 16: Declares `t_comparable_interface_init` function.
It needs to be declared before `G_DEFINE_TYPE_WITH_CODE` macro.
- 18-20: `G_DEFINE_TYPE_WITH_CODE` macro.
Because TStr is derivable type, its private area (TStrPrivate) is needed.
`G_ADD_PRIVATE` macro makes the private area.
Be careful that there's no comma after `G_ADD_PRIVATE` macro.
- 61-85: `t_str_comparable_cmp`.
- 63-66: Checks `self` and the argument `other`.
If the argument is invalid, "arg-error" signal is emitted.
- 71-72: Gets strings `s` and `o` from TStr objects `self` and `other`.
- 74-81: Compares `s` and `o` with the C standard function `strcmp`.
- 82-83: Frees `s` and `o`.
- 84: Returns the result.
- 87-90: `t_comparable_interface_init` function.
It overrides `iface->comp` with `t_str_comparable_cmp`.

TStr can be compared with TStr, but not with TInt nor TDouble.
Generally, comparison is available between two same type instances.

TNumStr itself doesn't implement TComparable.
But it is a child of TStr, so it is comparable.
The comparison is based on the alphabetical order.
So, "a" is bigger than "b" and "three" is bigger than "two".

## Test program.

`main.c` is a test program.

~~~C
  1 #include <glib-object.h>
  2 #include "tcomparable.h"
  3 #include "tnumber.h"
  4 #include "tint.h"
  5 #include "tdouble.h"
  6 #include "tstr.h"
  7
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
 33   TStr *ts1, *ts2, *ts3;
 34 
 35   ts1 = t_str_new_with_string("\"");
 36   if (T_IS_NUMBER (c1))
 37     s1 = t_number_to_s (T_NUMBER (c1));
 38   else if (T_IS_STR (c1)) {
 39     ts2 = t_str_concat (ts1, T_STR (c1));
 40     ts3 = t_str_concat (ts2, ts1);
 41     s1 = t_str_get_string (T_STR (ts3));
 42     g_object_unref (ts2);
 43     g_object_unref (ts3);
 44   } else {
 45     g_print ("c1 isn't TInt, TDouble nor TStr.\n");
 46     return;
 47   }
 48   if (T_IS_NUMBER (c2))
 49     s2 = t_number_to_s (T_NUMBER (c2));
 50   else if (T_IS_STR (c2)) {
 51     ts2 = t_str_concat (ts1, T_STR (c2));
 52     ts3 = t_str_concat (ts2, ts1);
 53     s2 = t_str_get_string (T_STR (ts3));
 54     g_object_unref (ts2);
 55     g_object_unref (ts3);
 56   } else {
 57     g_print ("c2 isn't TInt, TDouble nor TStr.\n");
 58     return;
 59   }
 60   g_print ("%s %s %s.\n", s1, cmp, s2);
 61   g_object_unref (ts1);
 62   g_free (s1);
 63   g_free (s2);
 64 }    
 65 
 66 static void
 67 compare (TComparable *c1, TComparable *c2) {
 68   if (t_comparable_eq (c1, c2))
 69     t_print ("equals", c1, c2);
 70   else if (t_comparable_gt (c1, c2))
 71     t_print ("is greater than", c1, c2);
 72   else if (t_comparable_lt (c1, c2))
 73     t_print ("is less than", c1, c2);
 74   else if (t_comparable_ge (c1, c2))
 75     t_print ("is greater than or equal to", c1, c2);
 76   else if (t_comparable_le (c1, c2))
 77     t_print ("is less than or equal to", c1, c2);
 78   else
 79     t_print ("can't compare to", c1, c2);
 80 }
 81 
 82 int
 83 main (int argc, char **argv) {
 84   const char *one = "one";
 85   const char *two = "two";
 86   const char *three = "three";
 87   TInt *i;
 88   TDouble *d;
 89   TStr *str1, *str2, *str3;
 90   gpointer obj;
 91 
 92   i = t_int_new_with_value (124);
 93   d = t_double_new_with_value (123.45);
 94   str1 = t_str_new_with_string (one);
 95   str2 = t_str_new_with_string (two);
 96   str3 = t_str_new_with_string (three);
 97   obj = g_object_new (G_TYPE_OBJECT, NULL);
 98 
 99   g_signal_connect (G_OBJECT (i), "notify::value", G_CALLBACK (notify_cb), NULL);
100   g_signal_connect (G_OBJECT (d), "notify::value", G_CALLBACK (notify_cb), NULL);
101   g_signal_connect (G_OBJECT (str1), "notify::string", G_CALLBACK (notify_cb), NULL);
102   g_signal_connect (G_OBJECT (str2), "notify::string", G_CALLBACK (notify_cb), NULL);
103   g_signal_connect (G_OBJECT (str3), "notify::string", G_CALLBACK (notify_cb), NULL);
104 
105   compare (T_COMPARABLE (i), T_COMPARABLE (d));
106   compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
107   compare (T_COMPARABLE (str2), T_COMPARABLE (str3));
108   t_comparable_eq (T_COMPARABLE (d), obj);
109 
110 
111   g_object_unref (i);
112   g_object_unref (d);
113   g_object_unref (str1);
114   g_object_unref (str2);
115   g_object_unref (str3);
116   g_object_unref (obj);
117 
118   return 0;
119 }
120 
~~~

- 10-28: "notify" signal handler is extended to support three types of objects: TInt, TDouble and TStr.
Because TNumStr is a child of TStr, TNumStr instance is also supported.
- 30-64: `t_print` function.
It has three parameters and builds an output string, then shows it to the display.
When it builds the output, strings are surrounded with double quotes.
- 66-80: `compare` compares two TComparable objects and calls `t_print` to display the result.
- 82-118: `main` function.
- 92-97: Creates TInt, TDouble, three TStr and GObject instances.
They are given values.
- 99-103: Connects "notify" signals.
- 105: Compares TInt and TDouble.
- 105-106: Compares two TStr.
- 108: Check the equality between TDouble and GObject.
This makes an error because GObject doesn't implement TComparable.
- 111-116 Frees objects.

## Compilation and execution

Change your current directory to [src/tcomparable](../src/comparable).

~~~
$ meson _build
$ ninja -C _build
~~~

Then execute it.

~~~
$ cd tcomparable; _build/tcomparable 2>&1
124 is greater than 123.450000.
"one" is less than "two".
"two" is greater than "three".

TComparable: argument error.

** (process:81639): CRITICAL **: 21:53:14.414: t_comparable_eq: assertion 'T_IS_COMPARABLE (other)' failed
~~~

The lines from `TComparable: argument error.` to `** (process:XXXXX)...` show that the comparison with TDouble and GObject fails.

## Build an interface without macros

We used macros such as `G_DECLARE_INTERFACE`, `G_DEFINE_INTERFACE` to build an interface.
And `G_DEFINE_TYPE_WITH_CODE` to implement the interface.
We can also build it without macros.
There are three files in the `tcomparable` directory.

- `tcomparable_without_macro.h`
- `tcomparable_without_macro.c`
- `tint_without_macro.c`

They don't use macros.
Instead, they register the interface or implement the interface to the type system directly.
If you want to know that, see the source files in [src/tcomparable](../src/tcomparable).

## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the language are similar.
Learning such languages is very useful to know GObject system.


Up: [Readme.md](../Readme.md),  Prev: [Section 8](sec8.md)
