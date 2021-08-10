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

@@@include
tcomparable/tcomparable.h
@@@

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

@@@include
tcomparable/tcomparable.c
@@@

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

@@@include
tcomparable/tint.c
@@@

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

@@@include
tcomparable/tstr.c
@@@

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
If you want to know the details, see [`src/tcomparable/tnumstr.c](tcomparable/tnumstr.c).

## Test program.

`main.c` is a test program.

@@@include
tcomparable/main.c
@@@

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

Change your current directory to [src/tcomparable](comparable).

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
If you want to know that, see the source files in [src/tcomparable](tcomparable).


## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the language are similar.
Learning such languages is very useful to know GObject system.

