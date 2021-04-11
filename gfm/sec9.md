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
So, any other object than the descendants of GObject, for example GVarinat, can't implement TComparable.
A prerequisite is the GType of either an interface or a class.
This macro expands to:
  - Declaration of `t_comparable_get_type()`.
  - `Typedef struct _TComparableInterface TComparableInterface`
  - `T_COMPARABLE ()` macro. It casts an instance to TComparable type.
  - `T_IS_COMPARABLE ()` macro. It checks if the type of an instance is `T_TYPE_COMPARABLE`.
  - `T_COMPARABLE_GET_IFACE ()` macro. It gets the interface of the instance.
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
It is a virtual function and is expected to be overridden by a function in the class.
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
 27                 0     /* n_params */,
 28                 NULL  /* param_types */);
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
So, the method doesn't work before an implementation object rewrites it.
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

## Implementing interface

TInt, TDouble and TStr implement TComparable.
First, look at TInt.
The header file is the same as before.
The implementation is written in C file.

`tint.c' is as follows.

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
There is a good example in [GObject API reference](https://developer.gnome.org/gobject/stable/gtype-non-instantiatable-classed.html).
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
  4 struct _TStr {
  5   TPtr parent;
  6 };
  7 
  8 static void t_comparable_interface_init (TComparableInterface *iface);
  9 
 10 G_DEFINE_TYPE_WITH_CODE (TStr, t_str, T_TYPE_PTR,
 11                          G_IMPLEMENT_INTERFACE (T_TYPE_COMPARABLE, t_comparable_interface_init))
 12 
 13 static int
 14 t_str_comparable_cmp (TComparable *self, TComparable *other) {
 15   g_return_val_if_fail (T_IS_STR (self), -2);
 16   g_return_val_if_fail (T_IS_STR (other), -2);
 17 
 18   char *s, *o;
 19   int result;
 20 
 21   s = t_str_get_string (T_STR (self));
 22   o = t_str_get_string (T_STR (other));
 23 
 24   if (strcmp (s, o) > 0)
 25     result = 1;
 26   else if (strcmp (s, o) == 0)
 27     result = 0;
 28   else if (strcmp (s, o) < 0)
 29     result = -1;
 30   else /* This can't happen. */
 31     result = -2;
 32   g_free (s);
 33   g_free (o);
 34   return result;
 35 }
 36 
 37 static void
 38 t_comparable_interface_init (TComparableInterface *iface) {
 39   iface->cmp = t_str_comparable_cmp;
 40 }
 41 
 42 static void
 43 t_str_init (TStr *inst) {
 44 }
 45 
 46 static void
 47 t_str_finalize (GObject *object) {
 48   char *s = (char *) t_ptr_get_pointer (T_PTR (object));
 49 
 50   if (s)
 51     g_free (s);
 52   G_OBJECT_CLASS (t_str_parent_class)->finalize (object);
 53 }
 54 
 55 static void
 56 t_str_class_init (TStrClass *class) {
 57   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
 58 
 59   gobject_class->finalize = t_str_finalize;
 60 }
 61 
 62 /* setter and getter */
 63 void
 64 t_str_set_string (TStr *self, const char *s) {
 65   char *t = g_strdup (s);
 66 
 67   t_ptr_set_pointer (T_PTR (self), t);
 68 }
 69 
 70 char *
 71 t_str_get_string (TStr *self) {
 72   char *s;
 73 
 74   s = (char *) t_ptr_get_pointer (T_PTR (self));
 75   if (s)
 76     return g_strdup (s);
 77   else
 78     return s; /* NULL */
 79 }
 80 
 81 TStr *
 82 t_str_concat (TStr *self, TStr *other) {
 83   char *s1, *s2, *s3;
 84   TStr *str;
 85 
 86   s1 = t_str_get_string (self);
 87   s2 = t_str_get_string (other);
 88   if (s1 && s2)
 89     s3 = g_strconcat (s1, s2, NULL);
 90   else if (s1)
 91     s3 = g_strdup (s1);
 92   else if (s2)
 93     s3 = g_strdup (s2);
 94   else
 95     s3 = NULL;
 96   str = t_str_new_with_string (s3);
 97   if (s1) g_free (s1);
 98   if (s2) g_free (s2);
 99   if (s3) g_free (s3);
100   return str;
101 }
102 
103 TStr *
104 t_str_new_with_string (const char *s) {
105   TStr *str;
106 
107   str = g_object_new (T_TYPE_STR, NULL);
108   t_str_set_string (str,s);
109   return str;
110 }
111 
112 TStr *
113 t_str_new (void) {
114   TStr *str;
115 
116   str = g_object_new (T_TYPE_STR, NULL);
117   return str;
118 }
119 
~~~

- 13-35: `t_str_comparable_cmp`.
First, strings are got from TStr objects.
They are `s` and `o`.
It uses the C standard function `strcmp` to compare strings.
After that, `s` and `o` are freed.

TStr can be compared with TStr, but not with TInt nor TDouble.
Generally, comparison is available between two same type instances.
It is possible that TNumber would implement TComparable instead of TInt and TDouble so far.
But, if you make TComplex, which is an object of complex number, as a child of TNumber, it is impossible for TNumber to implement TComarable.
Because complex numbers cannot be ordered linearly.

`main.c` is a test program.

~~~C
  1 #include <glib-object.h>
  2 #include "tcomparable.h"
  3 #include "tnumber.h"
  4 #include "tint.h"
  5 #include "tdouble.h"
  6 #include "../tptr/tptr.h"
  7 #include "tstr.h"
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
 24   } else if (T_IS_STR (gobject) && strcmp (name, "pointer") == 0) {
 25     g_object_get (T_PTR (gobject), "pointer", &s, NULL);
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
 36   else if (T_IS_STR (T_STR (c1)))
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
 50   g_print ("%s %s %s.\n", s1, cmp, s2);
 51   g_free (s1);
 52   g_free (s2);
 53 }    
 54 
 55 static void
 56 compare (TComparable *c1, TComparable *c2) {
 57   if (t_comparable_eq (c1, c2))
 58     t_print ("equals", c1, c2);
 59   if (t_comparable_gt (c1, c2))
 60     t_print ("is greater than", c1, c2);
 61   if (t_comparable_lt (c1, c2))
 62     t_print ("is less than", c1, c2);
 63   if (t_comparable_ge (c1, c2))
 64     t_print ("is greater than or equal to", c1, c2);
 65   if (t_comparable_le (c1, c2))
 66     t_print ("is less than or equal to", c1, c2);
 67 }
 68 
 69 int
 70 main (int argc, char **argv) {
 71   const char *one = "one";
 72   const char *two = "two";
 73   TInt *i;
 74   TDouble *d;
 75   TStr *str1, *str2;
 76   gpointer obj;
 77 
 78   i = t_int_new ();
 79   d = t_double_new ();
 80   str1 = t_str_new ();
 81   str2 = t_str_new ();
 82   obj = g_object_new (G_TYPE_OBJECT, NULL);
 83 
 84   g_signal_connect (G_OBJECT (i), "notify::value", G_CALLBACK (notify_cb), NULL);
 85   g_signal_connect (G_OBJECT (d), "notify::value", G_CALLBACK (notify_cb), NULL);
 86   g_signal_connect (G_OBJECT (str1), "notify::pointer", G_CALLBACK (notify_cb), NULL);
 87   g_signal_connect (G_OBJECT (str2), "notify::pointer", G_CALLBACK (notify_cb), NULL);
 88 
 89   g_object_set (i, "value", 124, NULL);
 90   g_object_set (d, "value", 123.45, NULL);
 91   t_str_set_string (str1, one);
 92   t_str_set_string (str2, two);
 93 
 94   compare (T_COMPARABLE (i), T_COMPARABLE (d));
 95   compare (T_COMPARABLE (str1), T_COMPARABLE (str2));
 96   t_comparable_eq (T_COMPARABLE (d), obj);
 97 
 98   g_object_unref (i);
 99   g_object_unref (d);
100   g_object_unref (str1);
101   g_object_unref (str2);
102   g_object_unref (obj);
103 
104   return 0;
105 }
106 
~~~

- 10-28: "notify" signal handler is extended to support three types of objects: TInt, TDouble and TStr.
- 30-53: This function uses `to_s` function to convert TInt and/or TDouble to a string.
Then, displays the result of the comparison of two TComparable objects.
- 55-67: `compare` compares two TComparable objects and calls `t_print` to display the result.
- 69-105: `main` function.
- 78-92: Creates TInt, TDouble, two TStr and GObject.
Connects "notify" signal.
Set values and strings of TInt, TDouble and TStr.
- 94: Compares TInt and TDouble.
- 95: Compares two TStr.
- 96: Check equality between TDouble and GObject.
This makes an error because GObject doesn't implement TComparable.
- 98-102: Frees objects.

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
Property "pointer" is set to one.
Property "pointer" is set to two.
124 is greater than 123.450000.
124 is greater than or equal to 123.450000.
one is less than two.
one is less than or equal to two.

TComparable: argument error.

** (process:164961): CRITICAL **: 17:01:40.850: t_comparable_eq: assertion 'T_IS_COMPARABLE (other)' failed
~~~

The last two lines show that the comparison with TDouble and GObject fails.

## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the language are similar.
Learning such languages is very useful to know GObject system.


Up: [Readme.md](../Readme.md),  Prev: [Section 8](sec8.md)
