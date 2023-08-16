Up: [Readme.md](../Readme.md),  Prev: [Section 5](sec5.md), Next: [Section 7](sec7.md)

# Derivable type and abstract type

## Derivable type

There are two kinds of types, final type and derivable type.
Final type doesn't have any child object.
Derivable type has child objects.

The main difference between the two objects is their classes.
Final type objects doesn't have its own class area.
The only member of the class is its parent class.

Derivable object has its own area in the class.
The class is open to its descendants.

`G_DECLARE_DERIVABLE_TYPE` is used to declare derivable type.
It is written in a header file like this:

~~~C
#define T_TYPE_NUMBER             (t_number_get_type ())
G_DECLARE_DERIVABLE_TYPE (TNumber, t_number, T, NUMBER, GObject)
~~~

## Abstract type

Abstract type doesn't have any instance.
This type of object is derivable and its children can use functions and signals of the abstract object.

The examples of this section are TNumber, TInt and TDouble object.
TInt and TDouble have already been made in the previous section.
They represent integer and floating point respectively.
Numbers are more abstract than integer and floating point.

TNumber is an abstract object which represents numbers.
TNumber is a parent object of TInt and TDouble.
TNumber isn't instantiated because its type is abstract.
When an instance has TInt or TDouble type, it is an instance of TNumber as well.

TInt and TDouble have five operations: addition, subtraction, multiplication, division and unary minus operation.
Those operations can be defined on TNumber object.

In this section we will define TNumber object and five functions above.
In addition, `to_s` function will be added.
It converts the value of TNumber into a string.
It is like sprintf function.
And we will rewrite TInt and TDouble to implement the functions.

## TNumber class

`tnumber.h` is a header file for the TNumber class.

~~~C
 1 #pragma once
 2 
 3 #include <glib-object.h>
 4 
 5 #define T_TYPE_NUMBER             (t_number_get_type ())
 6 G_DECLARE_DERIVABLE_TYPE (TNumber, t_number, T, NUMBER, GObject)
 7 
 8 struct _TNumberClass {
 9   GObjectClass parent_class;
10   TNumber* (*add) (TNumber *self, TNumber *other);
11   TNumber* (*sub) (TNumber *self, TNumber *other);
12   TNumber* (*mul) (TNumber *self, TNumber *other);
13   TNumber* (*div) (TNumber *self, TNumber *other);
14   TNumber* (*uminus) (TNumber *self);
15   char * (*to_s) (TNumber *self);
16   /* signal */
17   void (*div_by_zero) (TNumber *self);
18 };
19 
20 /* arithmetic operator */
21 /* These operators create a new instance and return a pointer to it. */
22 TNumber *
23 t_number_add (TNumber *self, TNumber *other);
24 
25 TNumber *
26 t_number_sub (TNumber *self, TNumber *other);
27 
28 TNumber *
29 t_number_mul (TNumber *self, TNumber *other);
30 
31 TNumber *
32 t_number_div (TNumber *self, TNumber *other);
33 
34 TNumber *
35 t_number_uminus (TNumber *self);
36 
37 char *
38 t_number_to_s (TNumber *self);
~~~

- 6: `G_DECLARE_DERIVABLE_TYPE` macro.
This is similar to `G_DECLARE_FINAL_TYPE` macro.
The difference is derivable or final.
`G_DECLARE_DERIVABLE_TYPE` is expanded to:
  - Declaration of `t_number_get_type ()` function. This function must be defined in `tnumber.c` file. The definition is usually done with `G_DEFINE_TYPE` or its family macros.
  - Definition of TNumber instance, whose member is its parent only.
  - Declaration of TNumberClass. It should be defined later in the header file.
  - Convenience macros `T_NUMBER` (cast to instance), `T_NUMBER_CLASS` (cast to class), `T_IS_NUMBER` (instance check), `T_IS_NUMBER_CLASS` (class check) and `T_NUMBER_GET_CLASS` are defined.
  - `g_autoptr()` support.
- 8-18: Definition of the structure of TNumberClass.
- 10-15: These are pointers to functions.
They are called class methods or virtual functions.
They are expected to be overridden in the descendant object.
The methods are five arithmetic operators and `to_s` function.
`to_s` function is similar to sprintf function.
- 17: A pointer to the default signal handler of "div-by-zero" signal.
The offset of this pointer is given to `g_signal_new` as an argument.
- 22-38: Functions. They are public.

`tnumber.c` is as follows.

~~~C
 1 #include "tnumber.h"
 2 
 3 static guint t_number_signal;
 4 
 5 G_DEFINE_ABSTRACT_TYPE (TNumber, t_number, G_TYPE_OBJECT)
 6 
 7 static void
 8 div_by_zero_default_cb (TNumber *self) {
 9   g_printerr ("\nError: division by zero.\n\n");
10 }
11 
12 static void
13 t_number_class_init (TNumberClass *class) {
14 
15   /* virtual functions */
16   class->add = NULL;
17   class->sub = NULL;
18   class->mul = NULL;
19   class->div = NULL;
20   class->uminus = NULL;
21   class->to_s = NULL;
22   /* default signal handler */
23   class->div_by_zero = div_by_zero_default_cb;
24   /* signal */
25   t_number_signal =
26   g_signal_new ("div-by-zero",
27                 G_TYPE_FROM_CLASS (class),
28                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
29                 G_STRUCT_OFFSET (TNumberClass, div_by_zero),
30                 NULL /* accumulator */,
31                 NULL /* accumulator data */,
32                 NULL /* C marshaller */,
33                 G_TYPE_NONE /* return_type */,
34                 0     /* n_params */
35                 );
36 }
37 
38 static void
39 t_number_init (TNumber *self) {
40 }
41 
42 TNumber *
43 t_number_add (TNumber *self, TNumber *other) {
44   g_return_val_if_fail (T_IS_NUMBER (self), NULL);
45   g_return_val_if_fail (T_IS_NUMBER (other), NULL);
46 
47   TNumberClass *class = T_NUMBER_GET_CLASS(self);
48 
49   return class->add ? class->add (self, other) : NULL;
50 }
51 
52 TNumber *
53 t_number_sub (TNumber *self, TNumber *other) {
54   g_return_val_if_fail (T_IS_NUMBER (self), NULL);
55   g_return_val_if_fail (T_IS_NUMBER (other), NULL);
56 
57   TNumberClass *class = T_NUMBER_GET_CLASS(self);
58 
59   return class->sub ? class->sub (self, other) : NULL;
60 }
61 
62 TNumber *
63 t_number_mul (TNumber *self, TNumber *other) {
64   g_return_val_if_fail (T_IS_NUMBER (self), NULL);
65   g_return_val_if_fail (T_IS_NUMBER (other), NULL);
66 
67   TNumberClass *class = T_NUMBER_GET_CLASS(self);
68 
69   return class->mul ? class->mul (self, other) : NULL;
70 }
71 
72 TNumber *
73 t_number_div (TNumber *self, TNumber *other) {
74   g_return_val_if_fail (T_IS_NUMBER (self), NULL);
75   g_return_val_if_fail (T_IS_NUMBER (other), NULL);
76 
77   TNumberClass *class = T_NUMBER_GET_CLASS(self);
78 
79   return class->div ? class->div (self, other) : NULL;
80 }
81 
82 TNumber *
83 t_number_uminus (TNumber *self) {
84   g_return_val_if_fail (T_IS_NUMBER (self), NULL);
85 
86   TNumberClass *class = T_NUMBER_GET_CLASS(self);
87 
88   return class->uminus ? class->uminus (self) : NULL;
89 }
90 
91 char *
92 t_number_to_s (TNumber *self) {
93   g_return_val_if_fail (T_IS_NUMBER (self), NULL);
94 
95   TNumberClass *class = T_NUMBER_GET_CLASS(self);
96 
97   return class->to_s ? class->to_s (self) : NULL;
98 }
~~~

- 5: `G_DEFINE_ABSTRACT_TYPE` macro.
This macro is used to define an abstract type object.
Abstract type isn't instantiated.
This macro is expanded to:
  - Declaration of `t_number_init ()` function.
  - Declaration of `t_number_class_init ()` function.
  - Definition of `t_number_get_type ()` function.
  - Definition of `t_number_parent_class` static variable that points the parent class.
- 3, 7-10, 26-35: Defines division-by-zero signal.
The function `div_by_zero_default_cb` is a default handler of "div-by-zero" signal.
Default handler doesn't have user data parameter.
The function `g_signal_new` is used instead of `g_signal_new_class_handler`.
It specifies a handler as the offset from the top of the class to the pointer to the handler.
- 12-36: The class initialization function `t_number_class_init`.
- 16-21: These class methods are virtual functions.
They are expected to be overridden in the descendant object of TNumber.
NULL is assigned here so that nothing happens when the methods are called.
- 23: Assigns the address of the function `dev_by_zero_default_cb` to `class->div_by_zero`.
This is the default handler of "div-by-zero" signal.
- 38-40: `t_number_init` is a initialization function for an instance.
But abstract object isn't instantiated.
So, nothing is done in this function.
But you can't leave out the definition of this function.
- 42-98: Public functions.
These functions just call the corresponding class methods if the pointer to the class method is not NULL.

## TInt object.

`tint.h` is a header file of the TInt class.
TInt is a child class of TNumber.

~~~C
 1 #pragma once
 2 
 3 #include <glib-object.h>
 4 
 5 #define T_TYPE_INT  (t_int_get_type ())
 6 G_DECLARE_FINAL_TYPE (TInt, t_int, T, INT, TNumber)
 7 
 8 /* create a new TInt instance */
 9 TInt *
10 t_int_new_with_value (int value);
11 
12 TInt *
13 t_int_new (void);
~~~

- 9-13:Declares public functions.
Arithmetic functions and `to_s` are declared in TNumber, so TInt doesn't declare those functions.
Only instance creation functions are declared.

The C file `tint.c` implements virtual functions (class methods).
And the pointers of the methods in TNumberClass are rewritten here.

~~~C
  1 #include "tnumber.h"
  2 #include "tint.h"
  3 #include "tdouble.h"
  4 
  5 #define PROP_INT 1
  6 static GParamSpec *int_property = NULL;
  7 
  8 struct _TInt {
  9   TNumber parent;
 10   int value;
 11 };
 12 
 13 G_DEFINE_TYPE (TInt, t_int, T_TYPE_NUMBER)
 14 
 15 static void
 16 t_int_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 17   TInt *self = T_INT (object);
 18 
 19   if (property_id == PROP_INT)
 20     self->value = g_value_get_int (value);
 21   else
 22     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 23 }
 24 
 25 static void
 26 t_int_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 27   TInt *self = T_INT (object);
 28 
 29   if (property_id == PROP_INT)
 30     g_value_set_int (value, self->value);
 31   else
 32     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 33 }
 34 
 35 static void
 36 t_int_init (TInt *self) {
 37 }
 38 
 39 /* arithmetic operator */
 40 /* These operators create a new instance and return a pointer to it. */
 41 #define t_int_binary_op(op) \
 42   int i; \
 43   double d; \
 44   if (T_IS_INT (other)) { \
 45     g_object_get (T_INT (other), "value", &i, NULL); \
 46     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op i)); \
 47   } else { \
 48     g_object_get (T_DOUBLE (other), "value", &d, NULL); \
 49     return  T_NUMBER (t_int_new_with_value (T_INT(self)->value op (int) d)); \
 50   }
 51 
 52 static TNumber *
 53 t_int_add (TNumber *self, TNumber *other) {
 54   g_return_val_if_fail (T_IS_INT (self), NULL);
 55 
 56   t_int_binary_op (+)
 57 }
 58 
 59 static TNumber *
 60 t_int_sub (TNumber *self, TNumber *other) {
 61   g_return_val_if_fail (T_IS_INT (self), NULL);
 62 
 63   t_int_binary_op (-)
 64 }
 65 
 66 static TNumber *
 67 t_int_mul (TNumber *self, TNumber *other) {
 68   g_return_val_if_fail (T_IS_INT (self), NULL);
 69 
 70   t_int_binary_op (*)
 71 }
 72 
 73 static TNumber *
 74 t_int_div (TNumber *self, TNumber *other) {
 75   g_return_val_if_fail (T_IS_INT (self), NULL);
 76 
 77   int i;
 78   double d;
 79 
 80   if (T_IS_INT (other)) {
 81     g_object_get (T_INT (other), "value", &i, NULL);
 82     if (i == 0) {
 83       g_signal_emit_by_name (self, "div-by-zero");
 84       return NULL;
 85     } else
 86       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / i));
 87   } else {
 88     g_object_get (T_DOUBLE (other), "value", &d, NULL);
 89     if (d == 0) {
 90       g_signal_emit_by_name (self, "div-by-zero");
 91       return NULL;
 92     } else
 93       return  T_NUMBER (t_int_new_with_value (T_INT(self)->value / (int)  d));
 94   }
 95 }
 96 
 97 static TNumber *
 98 t_int_uminus (TNumber *self) {
 99   g_return_val_if_fail (T_IS_INT (self), NULL);
100 
101   return T_NUMBER (t_int_new_with_value (- T_INT(self)->value));
102 }
103 
104 static char *
105 t_int_to_s (TNumber *self) {
106   g_return_val_if_fail (T_IS_INT (self), NULL);
107 
108   int i;
109 
110   g_object_get (T_INT (self), "value", &i, NULL); 
111   return g_strdup_printf ("%d", i);
112 }
113 
114 static void
115 t_int_class_init (TIntClass *class) {
116   TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
117   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
118 
119   /* override virtual functions */
120   tnumber_class->add = t_int_add;
121   tnumber_class->sub = t_int_sub;
122   tnumber_class->mul = t_int_mul;
123   tnumber_class->div = t_int_div;
124   tnumber_class->uminus = t_int_uminus;
125   tnumber_class->to_s = t_int_to_s;
126 
127   gobject_class->set_property = t_int_set_property;
128   gobject_class->get_property = t_int_get_property;
129   int_property = g_param_spec_int ("value", "val", "Integer value", G_MININT, G_MAXINT, 0, G_PARAM_READWRITE);
130   g_object_class_install_property (gobject_class, PROP_INT, int_property);
131 }
132 
133 TInt *
134 t_int_new_with_value (int value) {
135   TInt *i;
136 
137   i = g_object_new (T_TYPE_INT, "value", value, NULL);
138   return i;
139 }
140 
141 TInt *
142 t_int_new (void) {
143   TInt *i;
144 
145   i = g_object_new (T_TYPE_INT, NULL);
146   return i;
147 }
~~~

- 5-6, 15-33, 127-130: Definition of the property "value".
This is the same as before.
- 8-11: Definition of the structure of TInt.
This must be defined before `G_DEFINE_TYPE`.
- 13: `G_DEFINE_TYPE` macro.
This macro expands to:
  - Declaration of `t_int_init ()` function.
  - Definition of `t_int_get_type ()` function.
  - Definition of `t_int_parent_class` static variable which points the parent class.
- 35-37: `t_int_init`.
- 41-112: These functions are connected to the class method pointers in TIntClass.
They are the implementation of the virtual functions defined in `tnumber.c`.
- 41-50: Defines a macro used in `t_int_add`, `t_int_sub` and `t_int_mul`.
This macro is similar to `t_int_div` function.
Refer to the explanation below for `t_int_div`.
- 52-71: The functions `t_int_add`, `t_int_sub` and `t_int_mul`.
The macro `t_int_binary_op` is used.
- 73-95: The function `t_int_div`.
The first argument `self` is the object on which the function is called.
The second argument `other` is another TNumber object.
It can be TInt or TDouble.
If it is TDouble, its value is casted to int before the division is performed.
If the divisor (`other`) is zero, "div-by-zero" signal is emitted.
The signal is defined in TNumber, so TInt doesn't know the signal id.
Therefore, the emission is done with `g_signal_emit_by_name` instead of `g_signal_emit`.
The return value of `t_int_div` is TNumber type object
However, because TNumber is abstract, the actual type of the object is TInt.
- 97-102: A function for unary minus operator.
- 104-112: The function `to_s`. This function converts int to string.
For example, if the value of the object is 123, then the result is a string "123".
The caller should free the string if it becomes useless.
- 114- 131: The class initialization function `t_int_class_init`.
- 120-125: The class methods are overridden.
For example, if `t_number_add` is called on a TInt object, then the function calls the class method `*tnumber_class->add`.
The pointer points `t_int_add` function.
Therefore, `t_int_add` is finally called.
- 133-147: Instance creation functions are the same as before.

## TDouble object.

TDouble object is defined with `tdouble.h` and `tdouble.c`.
The definition is very similar to TInt.
So, this subsection just shows the contents of the files.

tdouble.h

~~~C
 1 #pragma once
 2 
 3 #include <glib-object.h>
 4 
 5 #define T_TYPE_DOUBLE  (t_double_get_type ())
 6 G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, TNumber)
 7 
 8 /* create a new TDouble instance */
 9 TDouble *
10 t_double_new_with_value (double value);
11 
12 TDouble *
13 t_double_new (void);
~~~

tdouble.c

~~~C
  1 #include "tnumber.h"
  2 #include "tdouble.h"
  3 #include "tint.h"
  4 
  5 #define PROP_DOUBLE 1
  6 static GParamSpec *double_property = NULL;
  7 
  8 struct _TDouble {
  9   TNumber parent;
 10   double value;
 11 };
 12 
 13 G_DEFINE_TYPE (TDouble, t_double, T_TYPE_NUMBER)
 14 
 15 static void
 16 t_double_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 17   TDouble *self = T_DOUBLE (object);
 18   if (property_id == PROP_DOUBLE) {
 19     self->value = g_value_get_double (value);
 20   } else
 21     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 22 }
 23 
 24 static void
 25 t_double_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 26   TDouble *self = T_DOUBLE (object);
 27 
 28   if (property_id == PROP_DOUBLE)
 29     g_value_set_double (value, self->value);
 30   else
 31     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 32 }
 33 
 34 static void
 35 t_double_init (TDouble *self) {
 36 }
 37 
 38 /* arithmetic operator */
 39 /* These operators create a new instance and return a pointer to it. */
 40 #define t_double_binary_op(op) \
 41   int i; \
 42   double d; \
 43   if (T_IS_INT (other)) { \
 44     g_object_get (T_INT (other), "value", &i, NULL); \
 45     return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value op (double) i)); \
 46   } else { \
 47     g_object_get (T_DOUBLE (other), "value", &d, NULL); \
 48     return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value op d)); \
 49   }
 50 
 51 static TNumber *
 52 t_double_add (TNumber *self, TNumber *other) {
 53   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 54 
 55   t_double_binary_op (+)
 56 }
 57 
 58 static TNumber *
 59 t_double_sub (TNumber *self, TNumber *other) {
 60   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 61 
 62   t_double_binary_op (-)
 63 }
 64 
 65 static TNumber *
 66 t_double_mul (TNumber *self, TNumber *other) {
 67   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 68 
 69   t_double_binary_op (*)
 70 }
 71 
 72 static TNumber *
 73 t_double_div (TNumber *self, TNumber *other) {
 74   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 75 
 76   int i;
 77   double d;
 78 
 79   if (T_IS_INT (other)) {
 80     g_object_get (T_INT (other), "value", &i, NULL);
 81     if (i == 0) {
 82       g_signal_emit_by_name (self, "div-by-zero");
 83       return NULL;
 84     } else
 85       return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value / (double) i));
 86   } else {
 87     g_object_get (T_DOUBLE (other), "value", &d, NULL);
 88     if (d == 0) {
 89       g_signal_emit_by_name (self, "div-by-zero");
 90       return NULL;
 91     } else
 92       return  T_NUMBER (t_double_new_with_value (T_DOUBLE(self)->value / d));
 93   }
 94 }
 95 
 96 static TNumber *
 97 t_double_uminus (TNumber *self) {
 98   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
 99 
100   return T_NUMBER (t_double_new_with_value (- T_DOUBLE(self)->value));
101 }
102 
103 static char *
104 t_double_to_s (TNumber *self) {
105   g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
106 
107   double d;
108 
109   g_object_get (T_DOUBLE (self), "value", &d, NULL);
110   return g_strdup_printf ("%lf", d);
111 }
112 
113 static void
114 t_double_class_init (TDoubleClass *class) {
115   TNumberClass *tnumber_class = T_NUMBER_CLASS (class);
116   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
117 
118   /* override virtual functions */
119   tnumber_class->add = t_double_add;
120   tnumber_class->sub = t_double_sub;
121   tnumber_class->mul = t_double_mul;
122   tnumber_class->div = t_double_div;
123   tnumber_class->uminus = t_double_uminus;
124   tnumber_class->to_s = t_double_to_s;
125 
126   gobject_class->set_property = t_double_set_property;
127   gobject_class->get_property = t_double_get_property;
128   double_property = g_param_spec_double ("value", "val", "Double value", -G_MAXDOUBLE, G_MAXDOUBLE, 0, G_PARAM_READWRITE);
129   g_object_class_install_property (gobject_class, PROP_DOUBLE, double_property);
130 }
131 
132 TDouble *
133 t_double_new_with_value (double value) {
134   TDouble *d;
135 
136   d = g_object_new (T_TYPE_DOUBLE, "value", value, NULL);
137   return d;
138 }
139 
140 TDouble *
141 t_double_new (void) {
142   TDouble *d;
143 
144   d = g_object_new (T_TYPE_DOUBLE, NULL);
145   return d;
146 }
~~~

## main.c

`main.c` is a simple program to test the objects.

~~~C
 1 #include <glib-object.h>
 2 #include "tnumber.h"
 3 #include "tint.h"
 4 #include "tdouble.h"
 5 
 6 static void
 7 notify_cb (GObject *gobject, GParamSpec *pspec, gpointer user_data) {
 8   const char *name;
 9   int i;
10   double d;
11 
12   name = g_param_spec_get_name (pspec);
13   if (T_IS_INT (gobject) && strcmp (name, "value") == 0) {
14     g_object_get (T_INT (gobject), "value", &i, NULL);
15     g_print ("Property \"%s\" is set to %d.\n", name, i);
16   } else if (T_IS_DOUBLE (gobject) && strcmp (name, "value") == 0) {
17     g_object_get (T_DOUBLE (gobject), "value", &d, NULL);
18     g_print ("Property \"%s\" is set to %lf.\n", name, d);
19   }
20 }
21 
22 int
23 main (int argc, char **argv) {
24   TNumber *i, *d, *num;
25   char *si, *sd, *snum;
26 
27   i = T_NUMBER (t_int_new ());
28   d = T_NUMBER (t_double_new ());
29 
30   g_signal_connect (G_OBJECT (i), "notify::value", G_CALLBACK (notify_cb), NULL);
31   g_signal_connect (G_OBJECT (d), "notify::value", G_CALLBACK (notify_cb), NULL);
32 
33   g_object_set (T_INT (i), "value", 100, NULL);
34   g_object_set (T_DOUBLE (d), "value", 12.345, NULL);
35 
36   num = t_number_add (i, d);
37 
38   si = t_number_to_s (i);
39   sd = t_number_to_s (d);
40   snum = t_number_to_s (num);
41 
42   g_print ("%s + %s is %s.\n", si, sd, snum);
43 
44   g_object_unref (num);
45   g_free (snum);
46 
47   num = t_number_add (d, i);
48   snum = t_number_to_s (num);
49 
50   g_print ("%s + %s is %s.\n", sd, si, snum);
51 
52   g_object_unref (num);
53   g_free (sd);
54   g_free (snum);
55 
56   g_object_set (T_DOUBLE (d), "value", 0.0, NULL);
57   sd = t_number_to_s (d);
58   if ((num = t_number_div(i, d)) != NULL) {
59     snum = t_number_to_s (num);
60     g_print ("%s / %s is %s.\n", si, sd, snum);
61     g_object_unref (num);
62     g_free (snum);
63   }
64 
65   g_object_unref (i);
66   g_object_unref (d);
67   g_free (si);
68   g_free (sd);
69 
70   return 0;
71 }
~~~

- 6-20: "notify" handler.
This handler is upgraded to support both TInt and TDouble.
- 22-71: The function `main`.
- 30-31: Connects the notify signals on `i` (TInt) and `d` (TDouble).
- 33-34: Set "value" properties on `i` and `d`.
- 36: Add `d` to `i`.
The answer is TInt object.
- 47: Add `i` to `d`.
The answer is TDouble object.
The addition of two TNumber objects isn't commutative because the type of the result will be different if the two objects are exchanged.
- 56-63: Tests division by zero signal.

## Compilation and execution

The source files are located under [src/tnumber](../src/tnumber).
The file `meson.buld`, which controls the compilation process, is as follows.

~~~meson
1 project ('tnumber', 'c')
2 
3 gobjdep = dependency('gobject-2.0')
4 
5 sourcefiles = files('main.c', 'tnumber.c', 'tint.c', 'tdouble.c')
6 
7 executable('tnumber', sourcefiles, dependencies: gobjdep, install: false)
8 
~~~

Compilation and execution is done by the usual way.

~~~
$ cd src/tnumber
$ meson setup _build
$ ninja -C _build
$ _build/tnumber
~~~

Then the following is shown on the display.

~~~
Property "value" is set to 100.
Property "value" is set to 12.345000.
100 + 12.345000 is 112.
12.345000 + 100 is 112.345000.
Property "value" is set to 0.000000.

Error: division by zero.
~~~

The two answers are different because of the different types.

This section has shown a simple example of derivable and abstract class.
You can define your derivable object like this.
If your object isn't abstract, use `G_DEFINE_TYPE` instead of `G_DEFINE_ABSTRACT_TYPE`.
And you need one more thing, how to manage private data in your derivable object.
There is a tutorial in [GObject API Reference](https://docs.gtk.org/gobject/tutorial.html#gobject-tutorial).
See the tutorial for learning derivable object.

It is also good to see source files in GTK.

## Class initialization process

### Initialization process of TNumberClass

Because TNumber is an abstract object, you cannot instantiate it directly.
And you cannot create the TNumber class as well.
But when you create its descendant instance, TNumber class is made and initialized.
First call for `g_object_new (T_TYPE_INT, ...)` or `g_object_new (T_TYPE_DOUBLE, ...)` creates and initializes TNumberClass if the class doesn't exist.
After that, TIntClass or TDoubleClass is created and followed by the creation for TInt or TDouble instance respectively.

And the initialization process for the TNumber class is as follows.

1. GObjectClass has been initialized before the function `main` starts.
2. Memory is allocated for TNumberClass.
3. The parent (GObjectClass) part of the class is copied from GObjectClass.
4. The class initialization function `t_number_class_init` is called.
It initializes class methods (Pointers to the class methods) and a default signal handler.

The diagram below shows the process.

![TNumberClass initialization](../image/tnumberclass_init.png)

### Initialization process of TIntClass

1. TNumberClass has been initialized before the initialization of TIntClass starts.
2. First call for `g_object_new (T_TYPE_INT, ...)` initializes TIntClass.
And the initialization process is as follows.
3. Memory is allocated for TIntClass.
TIntClass doesn't have its own area.
Therefore its structure is the same as its parent class (TNumberClass).
4. The parent (TNumberClass) part of the class (This is the same as whole TIntClass) is copied from TNumberClass.
5. The class initialization function `t_int_class_init` is called.
It overrides class methods from TNumber, `set_property` and `get_property`.

The diagram below shows the process.

![TIntClass initialization](../image/tintclass_init.png)

Up: [Readme.md](../Readme.md),  Prev: [Section 5](sec5.md), Next: [Section 7](sec7.md)
