Up: [Readme.md](../Readme.md),  Prev: [Section 7](sec7.md), Next: [Section 9](sec9.md)

# Overriding a property and chaining up the finalize process

The example in this section is TNumStr object.
TNumStr is a child of TStr object.
TNumStr holds a string and the string is a numeric string.

A numeric string is a string that expresses a number.
For example, "0", "-100" and "123.456" are numeric strings.
They are string and expresses numbers.

- "0" is a string.
It is a character array and its elements are '0' and '\0'.
It expresses 0, which is an integer zero.
- "-100" is a string that consists of '-', '1', '0', '0' and '\0'.
It expresses an integer -100.
- "123.456" consists of '1', '2', '3', '.', '4', '5', '6' and '\0'.
It expresses a real number (double type) 123.456.

A numeric string is such a specific string.

## Verification of a numeric string

Before defining TNumStr, we need a way to verify a numeric string.

Numeric string includes:

- Integer.
For example, "0", "100", "-10" and "+20".
- Double.
For example, "0.1", "-10.3", "+3.14", ".05" "1." and "0.0".

We need to be careful that "0" and "0.0" are different.
Because their type are different.
The type of "0" is integer and the type of "0.0" is double.
In the same way, "1" is an integer and "1." is a double.

".5" and "0.5" are the same.
Both are double and their value is 0.5.

Verification of a numeric string is a kind of lexical analysis.
A state diagram and state matrix is often used for lexical analysis.

A numeric string is a sequence of characters that satisfies:

1. '+' or '-' can be the first character. It can be left out.
2. a sequence of digits follows.
3. a period follows.
4. a sequence of digits follows.

The second part can be left out.
For example, ".56" or "-.56" are correct.

The third and fourth parts can be left out.
For example, "12" or "-23" are correct.

The fourth part can be left out.
For example, "100." is correct.

There are six state.

- 0 is the start point.
- 1 is the state after '+' or '-'.
- 2 is at the middle of the first sequence of digits (integer part).
- 3 is the state after the decimal point.
- 4 is the end of the string and the string is int.
- 5 is the end of the string and the string is double.
- 6 is error. The string doesn't express a number.

The input characters are:

- 0: '+' or '-'
- 1: digit ('0' - '9')
- 2: period '.'
- 3: end of string '\0'
- 4: other characters

The state diagram is as follows.

![state diagram of a numeric string](../image/state_diagram.png)

The state matrix is:

|state＼input|0 |1 |2 |3 |4 |
|:-----------|:-|:-|:-|:-|:-|
|0           |1 |2 |3 |6 |6 |
|1           |6 |2 |3 |6 |6 |
|2           |6 |2 |3 |4 |6 |
|3           |6 |3 |6 |5 |6 |

## Header file

The header file of TNumStr is [`tnumstr.h`](../src/tstr/tnumstr.h).
It is in the `src/tstr` directory.

~~~C
 1 #ifndef __T_NUM_STR_H__
 2 #define __T_NUM_STR_H__
 3 
 4 #include <glib-object.h>
 5 #include "tstr.h"
 6 #include "../tnumber/tnumber.h"
 7 
 8 #define T_TYPE_NUM_STR  (t_num_str_get_type ())
 9 G_DECLARE_FINAL_TYPE (TNumStr, t_num_str, T, NUM_STR, TStr)
10 
11 /* type of the numeric string */
12 enum {
13   t_none,
14   t_int,
15   t_double
16 };
17 
18 int
19 t_num_str_is_numeric_string (const char *string);
20 
21 /* setter and getter */
22 void
23 t_num_str_set (TNumStr *self, TNumber *num);
24 
25 TNumber *
26 t_num_str_get (TNumStr *self);
27 
28 int
29 t_num_str_get_num_type (TNumStr *self);
30 
31 /* create a new TNumStr instance */
32 TNumStr *
33 t_num_str_new_with_int (int i);
34 
35 TNumStr *
36 t_num_str_new_with_double (double d);
37 
38 TNumStr *
39 t_num_str_new_with_tnumber (TNumber *num);
40 
41 TNumStr *
42 t_num_str_new (void);
43 #endif /* __T_NUM_STR_H__ */
44 
~~~

- 9: TNumStr is a child object of TStr.
It is a final type object.
- 12-16: These three integers define the type of TNumStr string.
  - `t_none`: No string is stored or the string isn't a numeric string.
  - `t_int`: Integer
  - `t_double`: Double
- 18-19: `t_num_str_is_numerc_string` returns the type of the string.
The returned value is `t_none`, `t_int` or `t_double`.
- 22-26: Setter and getter with TNumber object.
- 28-29: `t_num_str_get_num_type` returns the type of the numeric string.
- 31-42: Functions to create new TNumStr objects.

## C file

The C file of TNumStr is [`tnumstr.c`](../src/tstr/tnumstr.c).
It is in the `src/tstr` directory.

~~~C
  1 #include <stdlib.h>
  2 #include <ctype.h>
  3 #include "tnumstr.h"
  4 #include "../tnumber/tnumber.h"
  5 #include "../tnumber/tint.h"
  6 #include "../tnumber/tdouble.h"
  7 
  8 /* override string property */
  9 
 10 enum {
 11   PROP_0,
 12   PROP_STRING,
 13   N_PROPERTIES
 14 };
 15 
 16 struct _TNumStr {
 17   TStr parent;
 18   char *string;
 19   int type;
 20   int ivalue;
 21   double dvalue;
 22 };
 23 
 24 G_DEFINE_TYPE(TNumStr, t_num_str, T_TYPE_STR)
 25 
 26 int
 27 t_num_str_is_numeric_string (const char *string) {
 28   const char *t;
 29   int stat, input;
 30   /* state matrix */
 31   int m[4][5] = {
 32     {1, 2, 3, 6, 6},
 33     {6, 2, 3, 6, 6},
 34     {6, 2, 3, 4, 6},
 35     {6, 3, 6, 5, 6}
 36   };
 37 
 38   stat = 0;
 39   for (t = string; ; ++t) {
 40     if (*t == '+' || *t == '-')      
 41       input = 0;
 42     else if (isdigit (*t))
 43       input = 1;
 44     else if (*t == '.')
 45       input = 2;
 46     else if (*t == '\0')
 47       input = 3;
 48     else
 49       input = 4;
 50 
 51     stat = m[stat][input];
 52 
 53     if (stat >= 4 || *t == '\0')
 54       break;
 55   }
 56   if (stat == 4)
 57     return t_int;
 58   else if (stat == 5)
 59     return t_double;
 60   else
 61     return t_none;
 62 }
 63 
 64 static void
 65 t_num_str_set_string_property (TNumStr *self, const char *string) {
 66   int type;
 67 
 68   type = t_num_str_is_numeric_string (string);
 69   if (type == t_int) {
 70     if (self->string)
 71       g_free (self->string);
 72     self->string = g_strdup (string);
 73     self->type = t_int;
 74     self->ivalue = atoi (self->string);
 75   } else if (type == t_double) {
 76     if (self->string)
 77       g_free (self->string);
 78     self->string = g_strdup (string);
 79     self->type = t_double;
 80     self->dvalue = atof (self->string);
 81   } else /* type == NS_OTHER */
 82     /* The string is not a number. No update. */
 83     ;
 84 }
 85 
 86 static void
 87 t_num_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
 88   TNumStr *self = T_NUM_STR (object);
 89 
 90   if (property_id == PROP_STRING)
 91     t_num_str_set_string_property (self, g_value_get_string (value));
 92   else
 93     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
 94 }
 95 
 96 static void
 97 t_num_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
 98   TNumStr *self = T_NUM_STR (object);
 99 
100   if (property_id == PROP_STRING)
101     g_value_set_string (value, g_strdup (self->string));
102   else
103     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
104 }
105 
106 static void
107 t_num_str_finalize (GObject *object) {
108   TNumStr *self = T_NUM_STR (object);
109 
110   if (self->string)
111     g_free (self->string);
112   G_OBJECT_CLASS (t_num_str_parent_class)->finalize (object);
113 }
114 
115 static void
116 t_num_str_init (TNumStr *self) {
117   self->type = t_none;
118   self->string = NULL;
119 }
120 
121 static void
122 t_num_str_class_init (TNumStrClass *class) {
123   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
124 
125   gobject_class->finalize = t_num_str_finalize;
126   gobject_class->set_property = t_num_str_set_property;
127   gobject_class->get_property = t_num_str_get_property;
128   g_object_class_override_property (gobject_class, PROP_STRING, "string");
129 }
130 
131 /* setter and getter */
132 void
133 t_num_str_set (TNumStr *self, TNumber *num) {
134   g_return_if_fail (T_IS_NUM_STR (self));
135   g_return_if_fail (T_IS_NUMBER (num));
136 
137   char *s;
138 
139   if (T_IS_INT (num)) {
140     s = t_number_to_s (T_NUMBER (num));
141     t_str_set_string (T_STR (self), s);
142     g_free (s);
143   } else {
144     s = t_number_to_s (T_NUMBER (num));
145     t_str_set_string (T_STR (self), s);
146     g_free (s);
147   }
148 }
149 
150 TNumber *
151 t_num_str_get (TNumStr *self) {
152   g_return_val_if_fail (T_IS_NUM_STR (self), NULL);
153 
154   if (self->type == t_int)
155     return T_NUMBER (t_int_new_with_value (self->ivalue));
156   else if (self->type == t_double)
157     return T_NUMBER (t_double_new_with_value (self->dvalue));
158   else
159     return NULL;
160 }
161 
162 int
163 t_num_str_get_num_type (TNumStr *self) {
164   g_return_val_if_fail (T_IS_NUM_STR (self), G_TYPE_INVALID);
165 
166   return self->type;
167 }
168 
169 /* create a new TNumStr instance */
170 TNumStr *
171 t_num_str_new_with_int (int i) {
172   char *s;
173   TNumStr *numstr;
174 
175   numstr = t_num_str_new ();
176   s = g_strdup_printf ("%d", i);
177   t_str_set_string (T_STR (numstr), s);
178   g_free (s);
179   return numstr;
180 }
181 
182 TNumStr *
183 t_num_str_new_with_double (double d) {
184   char *s;
185   TNumStr *numstr;
186 
187   numstr = t_num_str_new ();
188   s = g_strdup_printf ("%f", d);
189   t_str_set_string (T_STR(numstr), s);
190   g_free (s);
191   return numstr;
192 }
193 
194 TNumStr *
195 t_num_str_new_with_tnumber (TNumber *num) {
196   g_return_val_if_fail (T_IS_NUMBER (num), NULL);
197 
198   TNumStr *numstr;
199 
200   numstr = t_num_str_new ();
201   t_num_str_set (numstr, num);
202   return numstr;
203 }
204 
205 TNumStr *
206 t_num_str_new (void) {
207   return T_NUM_STR (g_object_new (T_TYPE_NUM_STR, NULL));
208 }
209 
~~~

- 10-14: Defines a property id `PROP_STRING`.
This id is used for a overriding property "string".
It will be explained in the next subsection.
- 16-22: Definition of TNumStr.
  - string: the numeric string held by TNumStr instance.
  - type: the type of the numeric string. `t_int` or `t_double`.
If It is `t_none`, the string member is NULL or non-numeric string (it isn't expected though).
  ivalue: If the type is `t_int`, the value is assigned to it.
  dvalue: If the type is `t_double`, the value is assigned to it.
- 24: `G_DEFINE_TYPE` macro. the parent object is TStr.
- 26- 62: `t_num_str_is_numeric_string` function checks the given string and returns `t_int`, `t_double` or `t_none`.
If the string isn't a numeric string, `t_none` will be returned.
The check algorithm is explained in the first subsection "Verification of a numeric string".
- 64-104: Implements overriding "string" property.
This will be explained in the next subsection.
- 106-113: Finalize method.
This method and the "chain up to its parent" process will be explained later.
- 115-119: Initializes an instance.
- 121-129: Initializes the class.
First, the finalize method is overridden by `t_new_str_finalize`.
The next three lines overrides the "string" property.
It will be explained in the next subsection.
- 132-160: Setter and getter.
The setter sets the numeric string with a TNumber object.
And the getter returns a TNumber object.
- 162-167: `t_num_str_get_num_type` returns the type of the numeric string held by the TNumStr instance.
It is `t_int`, `t_double` or `t_none`.
- 170- 208: Four functions for creating a new TNumStr instance.

## Overriding a property

TStr has a "string" property.
Because TNumStr is a child of TStr, TNumStr also has the property.
It is natural to think that TNumStr uses the property for its numeric string.
But TStr's string is a general string.
It allows any string even if it is not a numeric string.
Therefore, it is not appropriate to use the "string" property as it is.

TNumStr overrides the property and it restricts it to a numeric string.
See the class initialization function again.

~~~C
1 static void
2 t_num_str_class_init (TNumStrClass *class) {
3   GObjectClass *gobject_class = G_OBJECT_CLASS (class);
4 
5   gobject_class->finalize = t_num_str_finalize;
6   gobject_class->set_property = t_num_str_set_property;
7   gobject_class->get_property = t_num_str_get_property;
8   g_object_class_override_property (gobject_class, PROP_STRING, "string");
9 }
~~~

- 6-7: set\_property and get\_property methods are overridden by TNumStr's set/get functions.
- 8: `g_object_class_override_property` overrides the "string" property.
The arguments are TNumStr class, new property id and the property name.
Now, the property can be set/get with the TNumStr's own functions.

The set/get functions are shown below.

~~~C
 1 static void
 2 t_num_str_set_string_property (TNumStr *self, const char *string) {
 3   int type;
 4 
 5   type = t_num_str_is_numeric_string (string);
 6   if (type == t_int) {
 7     if (self->string)
 8       g_free (self->string);
 9     self->string = g_strdup (string);
10     self->type = t_int;
11     self->ivalue = atoi (self->string);
12   } else if (type == t_double) {
13     if (self->string)
14       g_free (self->string);
15     self->string = g_strdup (string);
16     self->type = t_double;
17     self->dvalue = atof (self->string);
18   } else /* type == NS_OTHER */
19     /* The string is not a number. No update. */
20     ;
21 }
22 
23 static void
24 t_num_str_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
25   TNumStr *self = T_NUM_STR (object);
26 
27   if (property_id == PROP_STRING)
28     t_num_str_set_string_property (self, g_value_get_string (value));
29   else
30     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
31 }
32 
33 static void
34 t_num_str_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
35   TNumStr *self = T_NUM_STR (object);
36 
37   if (property_id == PROP_STRING)
38     g_value_set_string (value, g_strdup (self->string));
39   else
40     G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
41 }
~~~

- 23-31: `t_num_str_set_property` overrides the set\_property method.
If the property id is the same as the id set with `g_object_class_override_property`, then `t_num_str_set_string_property` is called.
- 1-21: `t_num_str_set_string_property` function.
If the string type is `t_int`, the string expresses an integer.
The previous string has been stored in `self->string`.
If it isn't NULL, free the string, first.
Then, duplicates the new string and stores it in`self->string`.
Updates `self->type` and `self->ivalue`.
If the string type is `t_double` the previous string is freed and the new string is duplicated and stored in `self->string`.
Updates `self->type` and `self->dvalue`.
If the string type is neither `t_int` nor `t_double`, then nothing happens.
Notice that even if the string isn't updated, notify signal will be emitted.
Therefore, if you want to know the change of the numeric string, you need to check the string is really changed.
- 33-41: `t_num_str_get_property` overrides the get\_property method.
It just retrieve the numeric string from `self->string` and set the GValue with the copy of the string.

The functions above uses `self->string` to save the numeric string.
It is a member of TNumStr structure.
Therefore, a `string` member of TStr (parent instance member) isn't used to save the property value.

## Chaining up to its parent

The "chain up to its parent" process is illustrated with the diagram below.
There are three classes, GObjectCLass, TStrClass and TNumStrClass.
Each class has a finalize method pointer which points finalize functions in the corresponding program (GObject, TStr or TNumStr program).
The finalize method of TNumStrClass finalizes its own part of the TNumStr instance.
At the end of the function, it calls its parent's finalize method.
It is the finalize method of TStrClass.
It calls its own finalize function and finalizes the TStr private data (TStrPrivate) of the TNumStr instance.
And at the end of the function, it calls its parent's finalize method.
At last, GObject finalize method finalizes the GObject part of the TNumStr instance.

![Chaining up process](../image/chainup.png)

## Compilation and execution

There's `main.c` in `src/tstr` directory.
It just tests `tstr.c` and `tnumstr.c` program.

Compilation is done by usual way.
First, change your current directory to `src/tstr`.

~~~
$ meson _build
$ ninja -C _build
$ _build/tstr
~~~

Up: [Readme.md](../Readme.md),  Prev: [Section 7](sec7.md), Next: [Section 9](sec9.md)
