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

There are six states.

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

![state diagram of a numeric string](../image/state_diagram.png){width=12cm height=9cm}

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

The header file of TNumStr is [`tnumstr.h`](tstr/tnumstr.h).
It is in the `src/tstr` directory.

@@@include
tstr/tnumstr.h
@@@

- 8: The macro `G_DECLARE_FINAL_TYPE` for TNumStr class.
It is a child class of TStr and a final type class.
- 11-15: These three enum data define the type of TNumStr string.
  - `t_none`: No string is stored or the string isn't a numeric string.
  - `t_int`: The string expresses an integer
  - `t_double`: The string expresses an real number, which is double type in C language.
- 18-19: The public function `t_num_str_get_string_type` returns the type of the string TStrNum object has.
The returned value is `t_none`, `t_int` or `t_double`.
- 22-30: Setter and getter from/to a TNumber object.
- 33-37: Functions to create new TNumStr objects.

## C file

The C file of TNumStr is [`tnumstr.c`](tstr/tnumstr.c).
It is in the `src/tstr` directory.

@@@include
tstr/tnumstr.c
@@@

- 9-12: TNumStr structure has its parent "TStr" and int type "type" members.
So, TNumStr instance holds a string, which is placed in the parent's private area, and a type.
- 14: `G_DEFINE_TYPE` macro.
- 16- 54: The function `t_num_str_string_type` checks the given string and returns `t_int`, `t_double` or `t_none`.
If the string is NULL or an non-numeric string, `t_none` will be returned.
The check algorithm is explained in the first subsection "Verification of a numeric string".
- 60-64: The function `t_num_str_real_set_string` sets TNumStr's string and its type.
This is a body of the class method pointed by `set_string` member of the class structure.
The class method is initialized in the class initialization function `t_num_str_class_init`.
- 66-69: The instance initialization function `t_num_str_init` sets the type to `t_none`
because its parent initialization function set the pointer `priv->string` to NULL.
- 71-76: The class initialization function `t_num_str_class_init` assigns `t_num_str_real_set_string` to the member `set_string`.
Therefore, the function `t_str_set_string` calls `t_num_str_real_set_string`, which sets not only the string but also the type.
The function `g_object_set` also calls it and sets both the string and type.
- 78-83: The public function `t_num_str_get_string_type` returns the type of the string.
- 86-113: Setter and getter.
The setter sets the numeric string from a TNumber object.
And the getter returns a TNumber object.
- 117-131: These two functions create TNumStr instances.

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

TNumStr extends the function `t_str_set_string` and it sets not only a string but also the type for a TNumStr instance.

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
$ cd src/tstr
$ meson setup _build
$ ninja -C _build
$ _build/test1
$ _build/test2
$ _build/tnumstr
String property is set to one.
"one" and "two" is "onetwo".
123 + 456 + 789 = 1368
TNumStr => TNumber => TNumStr
123 => 123 => 123
-45 => -45 => -45
+0 => 0 => 0
123.456 => 123.456000 => 123.456000
+123.456 => 123.456000 => 123.456000
-123.456 => -123.456000 => -123.456000
.456 => 0.456000 => 0.456000
123. => 123.000000 => 123.000000
0.0 => 0.000000 => 0.000000
123.4567890123456789 => 123.456789 => 123.456789
abc => (null) => abc
(null) => (null) => (null)
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
