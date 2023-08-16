Up: [Readme.md](../Readme.md),  Prev: [Section 8](sec8.md)

# Interface

Interface is similar to abstract class.
Interface defines virtual functions which are expected to be overridden by a function in another instantiable object.

This section provides a simple example, TComparable.
TComparable is an interface.
It defines functions to compare.
They are:

- `t_comparable_cmp (self, other)`: It compares `self` and `other`.
The first argument `self` is an instance on which `t_comparable_cmp` runs.
The second argument `other` is another instance.
This function needs to be overridden in an object which implements the interface.
  - If `self` is equal to `other`, `t_comparable_cmp` returns 0.
  - If `self` is greater than `other`, `t_comparable_cmp` returns 1.
  - If `self` is less than `other`, `t_comparable_cmp` returns -1.
  - If an error happens, `t_comparable_cmp` returns -2.
- `t_comparable_eq (self, other)`: It returns TRUE if `self` is equal to `other`.
Otherwise it returns FALSE.
You need to be careful that FALSE is returned even if an error occurs.
- `t_comparable_gt (self, other)`: It returns TRUE if `self` is greater than `other`.
Otherwise it returns FALSE.
- `t_comparable_lt (self, other)`: It returns TRUE if `self` is less than `other`.
Otherwise it returns FALSE.
- `t_comparable_ge (self, other)`: It returns TRUE if `self` is greater than or equal to `other`.
Otherwise it returns FALSE.
- `t_comparable_le (self, other)`: It returns TRUE if `self` is less than or equal to `other`.
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

Defining interfaces is similar to defining objects.

- Use `G_DECLARE_INTERFACE` instead of `G_DECLARE_FINAL_TYPE`.
- Use `G_DEFINE_INTERFACE` instead of `G_DEFINE_TYPE`.

Now let's see the header file.

~~~C
 1 #pragma once
 2 
 3 #include <glib-object.h>
 4 
 5 #define T_TYPE_COMPARABLE  (t_comparable_get_type ())
 6 G_DECLARE_INTERFACE (TComparable, t_comparable, T, COMPARABLE, GObject)
 7 
 8 struct _TComparableInterface {
 9   GTypeInterface parent;
10   /* signal */
11   void (*arg_error) (TComparable *self);
12   /* virtual function */
13   int (*cmp) (TComparable *self, TComparable *other);
14 };
15 
16 /* t_comparable_cmp */
17 /* if self > other, then returns 1 */
18 /* if self = other, then returns 0 */
19 /* if self < other, then returns -1 */
20 /* if error happens, then returns -2 */
21 
22 int
23 t_comparable_cmp (TComparable *self, TComparable *other);
24 
25 gboolean
26 t_comparable_eq (TComparable *self, TComparable *other);
27 
28 gboolean
29 t_comparable_gt (TComparable *self, TComparable *other);
30 
31 gboolean
32 t_comparable_lt (TComparable *self, TComparable *other);
33 
34 gboolean
35 t_comparable_ge (TComparable *self, TComparable *other);
36 
37 gboolean
38 t_comparable_le (TComparable *self, TComparable *other);
~~~

- 6: `G_DECLARE_INTERFACE` macro.
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
- 8-14: `TComparableInterface` structure.
This is similar to a class structure.
The first member is the parent interface.
The parent of `TComparableInterface` is `GTypeInterface`.
`GTypeInterface` is a base of all interface types.
It is like a `GTypeClass` which is a base of all class types.
`GTypeClass` is the first member of the structure `GObjectClass`.
(See `gobject.h`. Note that `GObjectClass` is the same as `struct _GObjectClass`.)
The next member is a pointer `arg_error` to the default signal handler of "arg-error" signal.
This signal is emitted when the second argument of the comparison function is inappropriate.
For example, if `self` is TInt and `other` is TStr, both of them are Comparable instance.
But they are *not* able to compare.
This is because `other` isn't TNumber.
The last member `cmp` is a pointer to a comparison method.
It is a virtual function and is expected to be overridden by a function in the object which implements the interface.
- 22-38: Public functions.

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
34 
35   TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);
36   
37   return (iface->cmp == NULL ? -2 : iface->cmp (self, other));
38 }
39 
40 gboolean
41 t_comparable_eq (TComparable *self, TComparable *other) {
42   return (t_comparable_cmp (self, other) == 0);
43 }
44 
45 gboolean
46 t_comparable_gt (TComparable *self, TComparable *other) {
47   return (t_comparable_cmp (self, other) == 1);
48 }
49 
50 gboolean
51 t_comparable_lt (TComparable *self, TComparable *other) {
52   return (t_comparable_cmp (self, other) == -1);
53 }
54 
55 gboolean
56 t_comparable_ge (TComparable *self, TComparable *other) {
57   int result = t_comparable_cmp (self, other);
58   return (result == 1 || result == 0);
59 }
60 
61 gboolean
62 t_comparable_le (TComparable *self, TComparable *other) {
63   int result = t_comparable_cmp (self, other);
64   return (result == -1 || result == 0);
65 }
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
- 15: Assigns NULL to `cmp`.
So, the comparison method doesn't work before an implementation class overrides it.
- 17: Set the default signal handler of the signal "arg-error".
- 18-28: Creates a signal "arg-error".
- 31-38: The function `t_comparable_cmp`.
It checks the type of `self` on the first line.
If it isn't comparable, it logs the error message and returns -2 (error).
If `iface->cmp` is NULL (it means the class method hasn't been overridden), then it returns -2.
Otherwise it calls the class method and returns the value returned by the class method.
- 40-65: Public functions.
These five functions are based on `t_comparable_cmp`.
Therefore, no overriding is necessary for them.
For example, `t_comparable_eq` just calls `t_comparable_cmp`.
And it returns TRUE if `t_comparable_cmp` returns zero.
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
  1 #include "../tnumber/tnumber.h"
  2 #include "../tnumber/tint.h"
  3 #include "../tnumber/tdouble.h"
  4 #include "tcomparable.h"
  5 
  6 enum {
  7   PROP_0,
  8   PROP_INT,
  9   N_PROPERTIES
 10 };
 11 
 12 static GParamSpec *int_properties[N_PROPERTIES] = {NULL, };
 13 
 14 struct _TInt {
 15   TNumber parent;
 16   int value;
 17 };
 18 
 19 static void t_comparable_interface_init (TComparableInterface *iface);
 20 
 21 G_DEFINE_TYPE_WITH_CODE (TInt, t_int, T_TYPE_NUMBER,
 22                          G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))
 23 
 24 static int
 25 t_int_comparable_cmp (TComparable *self, TComparable *other) {
 26   if (! T_IS_NUMBER (other)) {
 27     g_signal_emit_by_name (self, "arg-error");
 28     return -2;
 29   }
 30 
 31   int i;
 32   double s, o;
 33 
 34   s = (double) T_INT (self)->value;
 35   if (T_IS_INT (other)) {
 36     g_object_get (other, "value", &i, NULL);
 37     o = (double) i;
 38   } else
 39     g_object_get (other, "value", &o, NULL);
 40   if (s > o)
 41     return 1;
 42   else if (s == o)
 43     return 0;
 44   else if (s < o)
 45     return -1;
 46   else /* This can't happen. */
 47     return -2;
 48 }
 49 
 50 static void
 51 t_comparable_interface_init (TComparableInterface *iface) {
 52   iface->cmp = t_int_comparable_cmp;
 53 }
 54 
 55 static void
 56 t_int_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 57   TInt *self = T_INT (object);
 58 
 59   if (property_id == PROP_INT)
 60     self->value = g_value_get_int (value);
 61   else
 62     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 63 }
 64 
 65 static void
 66 t_int_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 67   TInt *self = T_INT (object);
 68 
 69   if (property_id == PROP_INT)
 70     g_value_set_int (value, self->value);
 71   else
 72     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 73 }
 74 
 75 static void
 76 t_int_init (TInt *self) {
 77 }
 78 
 79 /* arithmetic operator */
 80 /* These operators create a new instance and return a pointer to it. */
 81 #define t_int_binary_op(op) \
 82   int i; \
 83   double d; \
 84   if (T_IS_INT (other)) { \
 85     g_object_get (T_INT (other), "value", &i, NULL); \
 86     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op i)); \
 87   } else { \
 88     g_object_get (T_DOUBLE (other), "value", &d, NULL); \
 89     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op (int) d)); \
 90   }
 91 
 92 static TNumber *
 93 t_int_add (TNumber *self, TNumber *other) {
 94   g_return_val_if_fail (T_IS_INT (self), NULL);
 95 
 96   t_int_binary_op (+)
 97 }
 98 
 99 static TNumber *
100 t_int_sub (TNumber *self, TNumber *other) {
101   g_return_val_if_fail (T_IS_INT (self), NULL);
102 
103   t_int_binary_op (-)
104 }
105 
106 static TNumber *
107 t_int_mul (TNumber *self, TNumber *other) {
108   g_return_val_if_fail (T_IS_INT (self), NULL);
109 
110   t_int_binary_op (*)
111 }
112 
113 static TNumber *
114 t_int_div (TNumber *self, TNumber *other) {
115   g_return_val_if_fail (T_IS_INT (self), NULL);
116 
117   int i;
118   double d;
119 
120   if (T_IS_INT (other)) {
121     g_object_get (T_INT (other), "value", &i, NULL);
122     if (i == 0) {
123       g_signal_emit_by_name (self, "div-by-zero");
124       return NULL;
125     } else
126       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / i));
127   } else {
128     g_object_get (T_DOUBLE (other), "value", &d, NULL);
129     if (d == 0) {
130       g_signal_emit_by_name (self, "div-by-zero");
131       return NULL;
132     } else
133       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / (int)  d));
134   }
135 }
136 
137 static TNumber *
138 t_int_uminus (TNumber *self) {
139   g_return_val_if_fail (T_IS_INT (self), NULL);
140 
141   return T_NUMBER (t_int_new_with_value (- T_INT(self)->value));
142 }
143 
144 static char *
145 t_int_to_s (TNumber *self) {
146   g_return_val_if_fail (T_IS_INT (self), NULL);
147 
148   int i;
149 
150   g_object_get (T_INT (self), "value", &i, NULL); 
151   return g_strdup_printf ("%d", i);
152 }
153 
154 static void
155 t_int_class_init (TIntClass *class) {
156   TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
157   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
158 
159   /* override virtual functions */
160   tnumber_class->add = t_int_add;
161   tnumber_class->sub = t_int_sub;
162   tnumber_class->mul = t_int_mul;
163   tnumber_class->div = t_int_div;
164   tnumber_class->uminus = t_int_uminus;
165   tnumber_class->to_s = t_int_to_s;
166 
167   gobject_class->set_property = t_int_set_property;
168   gobject_class->get_property = t_int_get_property;
169   int_properties[PROP_INT] = g_param_spec_int ("value", "val", "Integer value", G_MININT, G_MAXINT, 0, G_PARAM_READWRITE);
170   g_object_class_install_properties (gobject_class, N_PROPERTIES, int_properties);
171 }
172 
173 TInt *
174 t_int_new_with_value (int value) {
175   TInt *i;
176 
177   i = g_object_new (T_TYPE_INT, "value", value, NULL);
178   return i;
179 }
180 
181 TInt *
182 t_int_new (void) {
183   TInt *i;
184 
185   i = g_object_new (T_TYPE_INT, NULL);
186   return i;
187 }
~~~

- 4: It needs to include the header file of TComparable.
- 19: Declaration of `t_comparable_interface_init ()` function.
This declaration must be done before `G_DEFINE_TYPE_WITH_CODE` macro.
- 21-22: `G_DEFINE_TYPE_WITH_CODE` macro.
The last parameter is `G_IMPLEMENT_INTERFACE` macro.
The second parameter of `G_IMPLEMENT_INTERFACE` is `t_comparable_interface_init`.
These two macros expands to:
  - Declaration of `t_int_class_init ()`.
  - Declaration of `t_int_init ()`.
  - Definition of `t_int_parent_class` static variable which points to the parent's class.
  - Definition of `t_int_get_type ()`.
This function includes `g_type_register_static_simple ()` and `g_type_add_interface_static ()`.
The function `g_type_register_static_simple ()` is a convenient version of `g_type_register_static ()`.
It registers TInt type to the type system.
The function `g_type_add_interface_static ()` adds an interface type to an instance type.
There is a good example in [GObject Reference Manual, Interfaces](https://docs.gtk.org/gobject/concepts.html#non-instantiatable-classed-types-interfaces).
If you want to know how to write codes without the macros, see [`tint_without_macro.c`](../src/tcomparble/tint_without_macro.c).
- 24-48: `t_int_comparable_cmp` is a function to compare TInt instance to TNumber instance.
- 26-29: Checks the type of `other`.
If the argument type is not TNumber, it emits "arg-error" signal with `g_signal_emit_by_name`.
- 34: Converts `self` into double.
- 35-39: Gets the value of `other` and if it is TInt then the value is casted to double.
- 40-47: compares `s` and `o` and returns 1, 0, -1 and -2.
- 50-53: `t_comparable_interface_init`.
This function is called in the initialization process of TInt.
The function `t_int_comparable_cmp` is assigned to `iface->cmp`.

`tdouble.c` is almost same as `tint.c`.
These two objects can be compared because int is casted to double before the comparison.

`tstr.c` is as follows.

~~~C
  1 #include "../tstr/tstr.h"
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
 26 /* The returned value of the function g_value_get_string can be NULL. */
 27 /* The function t_str_set_string calls a class method, */
 28 /* which is expected to rewrite in the descendant object. */
 29   if (property_id == PROP_STRING)
 30     t_str_set_string (self, g_value_get_string (value));
 31   else
 32     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 33 }
 34 
 35 static void
 36 t_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 37   TStr *self = T_STR (object);
 38   TStrPrivate *priv = t_str_get_instance_private (self);
 39 
 40 /* The second argument of the function g_value_set_string can be NULL. */
 41   if (property_id == PROP_STRING)
 42     g_value_set_string (value, priv->string);
 43   else
 44     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 45 }
 46 
 47 /* This function just set the string. */
 48 /* So, no notify signal is emitted. */
 49 static void
 50 t_str_real_set_string (TStr *self, const char *s) {
 51   TStrPrivate *priv = t_str_get_instance_private (self);
 52 
 53   if (priv->string)
 54     g_free (priv->string);
 55   priv->string = g_strdup (s);
 56 }
 57 
 58 static void
 59 t_str_finalize (GObject *object) {
 60   TStr *self = T_STR (object);
 61   TStrPrivate *priv = t_str_get_instance_private (self);
 62 
 63   if (priv->string)
 64     g_free (priv->string);
 65   G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
 66 }
 67 
 68 static int
 69 t_str_comparable_cmp (TComparable *self, TComparable *other) {
 70   if (! T_IS_STR (other)) {
 71     g_signal_emit_by_name (self, "arg-error");
 72     return -2;
 73   }
 74 
 75   char *s, *o;
 76   int result;
 77 
 78   s = t_str_get_string (T_STR (self));
 79   o = t_str_get_string (T_STR (other));
 80 
 81   if (strcmp (s, o) > 0)
 82     result = 1;
 83   else if (strcmp (s, o) == 0)
 84     result = 0;
 85   else if (strcmp (s, o) < 0)
 86     result = -1;
 87   else /* This can't happen. */
 88     result = -2;
 89   g_free (s);
 90   g_free (o);
 91   return result;
 92 }
 93 
 94 static void
 95 t_comparable_interface_init (TComparableInterface *iface) {
 96   iface->cmp = t_str_comparable_cmp;
 97 }
 98 
 99 static void
100 t_str_init (TStr *self) {
101   TStrPrivate *priv = t_str_get_instance_private (self);
102 
103   priv->string = NULL;
104 }
105 
106 static void
107 t_str_class_init (TStrClass *class) {
108   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
109 
110   gobject_class->finalize = t_str_finalize;
111   gobject_class->set_property = t_str_set_property;
112   gobject_class->get_property = t_str_get_property;
113   str_properties[PROP_STRING] = g_param_spec_string ("string", "str", "string", "", G_PARAM_READWRITE);
114   g_object_class_install_properties (gobject_class, N_PROPERTIES, str_properties);
115 
116   class->set_string = t_str_real_set_string;
117 }
118 
119 /* setter and getter */
120 void
121 t_str_set_string (TStr *self, const char *s) {
122   g_return_if_fail (T_IS_STR (self));
123   TStrClass *class = T_STR_GET_CLASS (self);
124 
125 /* The setter calls the class method 'set_string', */
126 /* which is expected to be overridden by the descendant TNumStr. */
127 /* Therefore, the behavior of the setter is different between TStr and TNumStr. */
128   class->set_string (self, s);
129 }
130 
131 char *
132 t_str_get_string (TStr *self) {
133   g_return_val_if_fail (T_IS_STR (self), NULL);
134   TStrPrivate *priv = t_str_get_instance_private (self);
135 
136   return g_strdup (priv->string);
137 }
138 
139 TStr *
140 t_str_concat (TStr *self, TStr *other) {
141   g_return_val_if_fail (T_IS_STR (self), NULL);
142   g_return_val_if_fail (T_IS_STR (other), NULL);
143 
144   char *s1, *s2, *s3;
145   TStr *str;
146 
147   s1 = t_str_get_string (self);
148   s2 = t_str_get_string (other);
149   if (s1 && s2)
150     s3 = g_strconcat (s1, s2, NULL);
151   else if (s1)
152     s3 = g_strdup (s1);
153   else if (s2)
154     s3 = g_strdup (s2);
155   else
156     s3 = NULL;
157   str = t_str_new_with_string (s3);
158   if (s1) g_free (s1);
159   if (s2) g_free (s2);
160   if (s3) g_free (s3);
161   return str;
162 }
163 
164 /* create a new TStr instance */
165 TStr *
166 t_str_new_with_string (const char *s) {
167   return T_STR (g_object_new (T_TYPE_STR, "string", s, NULL));
168 }
169 
170 TStr *
171 t_str_new (void) {
172   return T_STR (g_object_new (T_TYPE_STR, NULL));
173 }
~~~

- 16: Declares `t_comparable_interface_init` function.
It needs to be declared before `G_DEFINE_TYPE_WITH_CODE` macro.
- 18-20: `G_DEFINE_TYPE_WITH_CODE` macro.
Because TStr is derivable type, its private area (TStrPrivate) is needed.
`G_ADD_PRIVATE` macro makes the private area.
Be careful that there's no comma after `G_ADD_PRIVATE` macro.
- 68-92: `t_str_comparable_cmp`.
- 70-73: Checks the type of `other`.
If it is not TStr, "arg-error" signal is emitted.
- 78-79: Gets strings `s` and `o` from TStr objects `self` and `other`.
- 81-88: Compares `s` and `o` with the C standard function `strcmp`.
- 89-90: Frees `s` and `o`.
- 91: Returns the result.
- 94-97: `t_comparable_interface_init` function.
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
 3 #include "../tnumber/tnumber.h"
 4 #include "../tnumber/tint.h"
 5 #include "../tnumber/tdouble.h"
 6 #include "../tstr/tstr.h"
 7 
 8 static void
 9 t_print (const char *cmp, TComparable *c1, TComparable *c2) {
10   char *s1, *s2;
11   TStr *ts1, *ts2, *ts3;
12 
13   ts1 = t_str_new_with_string("\"");
14   if (T_IS_NUMBER (c1))
15     s1 = t_number_to_s (T_NUMBER (c1));
16   else if (T_IS_STR (c1)) {
17     ts2 = t_str_concat (ts1, T_STR (c1));
18     ts3 = t_str_concat (ts2, ts1);
19     s1 = t_str_get_string (T_STR (ts3));
20     g_object_unref (ts2);
21     g_object_unref (ts3);
22   } else {
23     g_print ("c1 isn't TInt, TDouble nor TStr.\n");
24     return;
25   }
26   if (T_IS_NUMBER (c2))
27     s2 = t_number_to_s (T_NUMBER (c2));
28   else if (T_IS_STR (c2)) {
29     ts2 = t_str_concat (ts1, T_STR (c2));
30     ts3 = t_str_concat (ts2, ts1);
31     s2 = t_str_get_string (T_STR (ts3));
32     g_object_unref (ts2);
33     g_object_unref (ts3);
34   } else {
35     g_print ("c2 isn't TInt, TDouble nor TStr.\n");
36     return;
37   }
38   g_print ("%s %s %s.\n", s1, cmp, s2);
39   g_object_unref (ts1);
40   g_free (s1);
41   g_free (s2);
42 }    
43 
44 static void
45 compare (TComparable *c1, TComparable *c2) {
46   if (t_comparable_eq (c1, c2))
47     t_print ("equals", c1, c2);
48   else if (t_comparable_gt (c1, c2))
49     t_print ("is greater than", c1, c2);
50   else if (t_comparable_lt (c1, c2))
51     t_print ("is less than", c1, c2);
52   else if (t_comparable_ge (c1, c2))
53     t_print ("is greater than or equal to", c1, c2);
54   else if (t_comparable_le (c1, c2))
55     t_print ("is less than or equal to", c1, c2);
56   else
57     t_print ("can't compare to", c1, c2);
58 }
59 
60 int
61 main (int argc, char **argv) {
62   const char *one = "one";
63   const char *two = "two";
64   const char *three = "three";
65   TInt *i;
66   TDouble *d;
67   TStr *str1, *str2, *str3;
68 
69   i = t_int_new_with_value (124);
70   d = t_double_new_with_value (123.45);
71   str1 = t_str_new_with_string (one);
72   str2 = t_str_new_with_string (two);
73   str3 = t_str_new_with_string (three);
74 
75   compare (T_COMPARABLE (i), T_COMPARABLE (d));
76   compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
77   compare (T_COMPARABLE (str2), T_COMPARABLE (str3));
78   compare (T_COMPARABLE (i), T_COMPARABLE (str1));
79 
80   g_object_unref (i);
81   g_object_unref (d);
82   g_object_unref (str1);
83   g_object_unref (str2);
84   g_object_unref (str3);
85 
86   return 0;
87 }
~~~

- 8-42: The function `t_print` has three parameters and builds an output string, then shows it to the display.
When it builds the output, strings are surrounded with double quotes.
- 44-58: The function `compare` compares two TComparable objects and calls `t_print` to display the result.
- 60-87: `main` function.
- 69-73: Creates TInt, TDouble and three TStr instances.
They are given values.
- 75: Compares TInt and TDouble.
- 76-77: Compares two TStr.
- 78: Compare TInt to TStr.
This makes an "arg-error".
- 80-84 Frees objects.

## Compilation and execution

Change your current directory to [src/tcomparable](../src/comparable).

~~~
$ cd src/tcomparable
$ meson setup _build
$ ninja -C _build
~~~

Then execute it.

~~~
$ _build/tcomparable
124 is greater than 123.450000.
"one" is less than "two".
"two" is greater than "three".

TComparable: argument error.

TComparable: argument error.

TComparable: argument error.

TComparable: argument error.

TComparable: argument error.
124 can't compare to "one".
~~~

## Build an interface without macros

We used macros such as `G_DECLARE_INTERFACE`, `G_DEFINE_INTERFACE` to build an interface.
And `G_DEFINE_TYPE_WITH_CODE` to implement the interface.
We can also build it without macros.
There are three files in the `tcomparable` directory.

- `tcomparable_without_macro.h`
- `tcomparable_without_macro.c`
- `tint_without_macro.c`

They don't use macros.
Instead, they register the interface or implementation of the interface to the type system directly.
If you want to know that, see the source files in [src/tcomparable](../src/tcomparable).

## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the languages are similar.
Learning such languages is very useful to know GObject system.

Up: [Readme.md](../Readme.md),  Prev: [Section 8](sec8.md)
