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

![state diagram of a numeric string](../image/state_diagram.png){width=12cm height=9cm}

The state matrix is:

|state＼input|0 |1 |2 |3 |4 |
|:-----------|:-|:-|:-|:-|:-|
|0           |1 |2 |3 |6 |6 |
|1           |6 |2 |3 |6 |6 |
|2           |6 |2 |3 |4 |6 |
|3           |6 |3 |6 |5 |6 |

## Header file

The header file of TNumStr is [`tnumstr.h`](tstr/tnumstr.h).
It is in the `src/tstr` directory.

@@@include
tstr/tnumstr.h
@@@

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

The C file of TNumStr is [`tnumstr.c`](tstr/tnumstr.c).
It is in the `src/tstr` directory.

@@@include
tstr/tnumstr.c
@@@

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

@@@include
tstr/tnumstr.c t_num_str_class_init
@@@

- 6-7: set\_property and get\_property methods are overridden by TNumStr's set/get functions.
- 8: `g_object_class_override_property` overrides the "string" property.
The arguments are TNumStr class, new property id and the property name.
Now, the property can be set/get with the TNumStr's own functions.

The set/get functions are shown below.

@@@include
tstr/tnumstr.c t_num_str_set_string_property t_num_str_set_property t_num_str_get_property
@@@

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

![Chaining up process](../image/chainup.png){width=12cm height=9cm}

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
