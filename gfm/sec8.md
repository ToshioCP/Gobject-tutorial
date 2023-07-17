Up: [Readme.md](../Readme.md),  Prev: [Section 7](sec7.md), Next: [Section 9](sec9.md)

# Child class extends parent's function

The example in this section is TNumStr object.
TNumStr is a child of TStr object.
TNumStr holds a string and the string type, which is one of `t_int`, `t_double` or `t_none`.

- t\_int: the string expresses an integer
- t\_double: the string expresses a double (floating point)
- t\_none: the string doesn't express the two numbers above.

A t\_int or t\_double type string is called a numeric string.
It is not a common terminology and it is used only in this tutorial.
In short, a numeric string is a string that expresses a number.
For example, "0", "-100" and "123.456" are numeric strings.
They are strings and express numbers.

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
Both are double and their values are 0.5.

Verification of a numeric string is a kind of lexical analysis.
A state diagram and state matrix is often used for lexical analysis.

A numeric string is a sequence of characters that satisfies:

1. '+' or '-' can be the first character. It can be left out.
2. followed by a sequence of digits.
3. followed by a period.
4. followed by a sequence of digits.

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

This state diagram doesn't support "1.23e5" style double (decimal floating point).
If it is supported, the state diagram will be more complicated.
(However, it will be a good practice for your programming skill.)

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
18 /* get the type of the TNumStr object */
19 int
20 t_num_str_get_string_type (TNumStr *self);
21 
22 /* setter and getter */
23 void
24 t_num_str_set_from_t_number (TNumStr *self, TNumber *num);
25 
26 // TNumStr can have any string, which is t_none, t_int or t_double type.
27 // If the type is t_none, t_num_str_get_t_number returns NULL.
28 // It is good idea to call t_num_str_get_string_type and check the type in advance. 
29 
30 TNumber *
31 t_num_str_get_t_number (TNumStr *self);
32 
33 /* create a new TNumStr instance */
34 TNumStr *
35 t_num_str_new_with_tnumber (TNumber *num);
36 
37 TNumStr *
38 t_num_str_new (void);
39 #endif /* __T_NUM_STR_H__ */
40 
~~~

- 9: TNumStr is a child object of TStr.
It is a final type object.
- 12-16: These three enum data define the type of TNumStr string.
  - `t_none`: No string is stored or the string isn't a numeric string.
  - `t_int`: Integer
  - `t_double`: Double
- 19-20: `t_num_str_get_string_type` returns the type of the string TStrNum object has.
The returned value is `t_none`, `t_int` or `t_double`.
- 23-31: Setter and getter from/to a TNumber object.
- 34-38: Functions to create new TNumStr objects.

## C file

The C file of TNumStr is [`tnumstr.c`](../src/tstr/tnumstr.c).
It is in the `src/tstr` directory.

~~~C
  1 #include <ctype.h>
  2 #include "tnumstr.h"
  3 #include "tstr.h"
  4 #include "../tnumber/tnumber.h"
  5 #include "../tnumber/tint.h"
  6 #include "../tnumber/tdouble.h"
  7
  8 
  9 
 10 struct _TNumStr {
 11   TStr parent;
 12   int type;
 13 };
 14 
 15 G_DEFINE_TYPE(TNumStr, t_num_str, T_TYPE_STR)
 16 
 17 static int
 18 t_num_str_string_type (const char *string) {
 19   const char *t;
 20   int stat, input;
 21   /* state matrix */
 22   int m[4][5] = {
 23     {1, 2, 3, 6, 6},
 24     {6, 2, 3, 6, 6},
 25     {6, 2, 3, 4, 6},
 26     {6, 3, 6, 5, 6}
 27   };
 28 
 29   if (string == NULL)
 30     return t_none;
 31   stat = 0;
 32   for (t = string; ; ++t) {
 33     if (*t == '+' || *t == '-')      
 34       input = 0;
 35     else if (isdigit (*t))
 36       input = 1;
 37     else if (*t == '.')
 38       input = 2;
 39     else if (*t == '\0')
 40       input = 3;
 41     else
 42       input = 4;
 43 
 44     stat = m[stat][input];
 45 
 46     if (stat >= 4 || *t == '\0')
 47       break;
 48   }
 49   if (stat == 4)
 50     return t_int;
 51   else if (stat == 5)
 52     return t_double;
 53   else
 54     return t_none;
 55 }
 56 
 57 static void
 58 t_num_str_real_set_string (TStr *self, const char *s) {
 59   T_STR_CLASS (t_num_str_parent_class)->set_string (self, s);
 60   T_NUM_STR (self)->type = t_num_str_string_type(s);
 61 }
 62 
 63 static void
 64 t_num_str_init (TNumStr *self) {
 65   self->type = t_none;
 66 }
 67 
 68 static void
 69 t_num_str_class_init (TNumStrClass *class) {
 70   TStrClass *t_str_class = T_STR_CLASS (class);
 71 
 72   t_str_class->set_string = t_num_str_real_set_string;
 73 }
 74 
 75 int
 76 t_num_str_get_string_type (TNumStr *self) {
 77   g_return_val_if_fail (T_IS_NUM_STR (self), -1);
 78 
 79   return self->type;
 80 }
 81 
 82 /* setter and getter */
 83 void
 84 t_num_str_set_from_t_number (TNumStr *self, TNumber *num) {
 85   g_return_if_fail (T_IS_NUM_STR (self));
 86   g_return_if_fail (T_IS_NUMBER (num));
 87 
 88   char *s;
 89 
 90   s = t_number_to_s (T_NUMBER (num));
 91   t_str_set_string (T_STR (self), s);
 92   g_free (s);
 93 }
 94 
 95 TNumber *
 96 t_num_str_get_t_number (TNumStr *self) {
 97   g_return_val_if_fail (T_IS_NUM_STR (self), NULL);
 98 
 99   char *s = t_str_get_string(T_STR (self));
100   TNumber *tnum;
101 
102   if (self->type == t_int)
103     tnum = T_NUMBER (t_int_new_with_value (atoi (s)));
104   else if (self->type == t_double)
105     tnum = T_NUMBER (t_double_new_with_value (atof (s)));
106   else
107     tnum = NULL;
108   g_free (s);
109   return tnum;
110 }
111 
112 /* create a new TNumStr instance */
113 
114 TNumStr *
115 t_num_str_new_with_tnumber (TNumber *num) {
116   g_return_val_if_fail (T_IS_NUMBER (num), NULL);
117 
118   TNumStr *numstr;
119 
120   numstr = t_num_str_new ();
121   t_num_str_set_from_t_number (numstr, num);
122   return numstr;
123 }
124 
125 TNumStr *
126 t_num_str_new (void) {
127   return T_NUM_STR (g_object_new (T_TYPE_NUM_STR, NULL));
128 }
~~~

- 10-13: Definition of TNumStr type C structure.
TNumStr instance holds a string and type.
The string is placed in the parent's (TStr) private area, so the structure doesn't have it.
The type is stored in the `type` element of the structure.
- 15: `G_DEFINE_TYPE` macro.
- 17- 55: `t_num_str_string_type` function checks the given string and returns `t_int`, `t_double` or `t_none`.
If the string is NULL or an non-numeric string, `t_none` will be returned.
The check algorithm is explained in the first subsection "Verification of a numeric string".
- 57-61: `t_num_str_real_set_string` function.
It sets TNumStr's string and type with the parameter `s`.
- 63-66: Initializes an instance.
When the TNumStr instance is created, it has no string (NULL).
So, the type is set with t\_none.
- 68-73: Initializes the class.
The class method `set_string` is replaced by `t_num_str_real_set_string`.
So `t_str_set_string` and `t_str_set_property` (in tstr.c) sets not only the string but also the type.
-75-80: `t_num_str_get_string_type` returns the type.
- 83-110: Setter and getter.
The setter sets the numeric string with a TNumber object.
And the getter returns a TNumber object.
- 114-128: TNumStr instance creating functions.

## Child class extends parent's function.

TNumStr is a child class of TStr, so it has all the TStr's public funftions.

- `TStr *t_str_concat (TStr *self, TStr *other)`
- `void t_str_set_string (TStr *self, const char *s)`
- `char *t_str_get_string (TStr *self)`

When you want to set a string to a TNumStr instance, you can use `t_str_set_string` function.

```c
TNumStr *ns = t_num_str_new ();
t_str_set_string (T_STR (ns), "123.456");
```

TNumStr extends the function of `t_str_set_string`.
It sets not only a string but also its type for a TNumStr instance.

```c
int t;
t = t_num_str_get_string_type (ns);
if (t == t_none) g_print ("t_none\n");
if (t == t_int) g_print ("t_int\n");
if (t == t_double) g_print ("t_double\n");
// => t_double appears on your display
```

TNumStr adds some public functions.

- `int t_num_str_get_string_type (TNumStr *self)`
- `void t_num_str_set_from_t_number (TNumStr *self, TNumber *num)`
- `TNumber *t_num_str_get_t_number (TNumStr *self)`

A child class extends the parent class.
So, a child is more specific and richer than the parent.

## Compilation and execution

There are `main.c`, `test1.c` and `test2.c` in `src/tstr` directory.
Two programs `test1.c` and `test2.c` generates `_build/test1` and `_build/test2` respectively.
They test `tstr.c` and `tnumstr.c`.
If there are errors, messages will appear.
Otherwise nothing appears.

The program `main.c` generates `_build/tnumstr`.
It shows how TStr and TNumStr work.

Compilation is done by usual way.
First, change your current directory to `src/tstr`.

~~~
$ meson _build
$ ninja -C _build
$ _build/test1
$ _build/test2
$ _build/tnumstr
~~~

The last part of `main.c` is conversion between TNumStr and TNumber.
There are some difference between them because of the following two reasons.

- floating point is rounded.
It is not an exact value when the value has long figures.
TNumStr "123.4567890123456789" is converted to TNumber 123.456789.
- There are two or more string expression in floating points.
TNumStr ".456" is converted to TNumber x, and x is converted to TNumStr "0.456000".

It is difficult to compare two TNumStr instances.
The best way is to compare TNumber instances converted from TNumStr.

Up: [Readme.md](../Readme.md),  Prev: [Section 7](sec7.md), Next: [Section 9](sec9.md)
