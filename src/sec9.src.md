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

@@@include
tcomparable/tcomparable.h
@@@

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

@@@include
tcomparable/tint.c
@@@

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
If you want to know how to write codes without the macros, see [`tint_without_macro.c`](tcomparble/tint_without_macro.c).
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

@@@include
tcomparable/tstr.c
@@@

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

@@@include
tcomparable/main.c
@@@

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

Change your current directory to [src/tcomparable](comparable).

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
If you want to know that, see the source files in [src/tcomparable](tcomparable).

## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the languages are similar.
Learning such languages is very useful to know GObject system.
