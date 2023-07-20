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

@@@include
tcomparable/tcomparable.h
@@@

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

@@@include
tcomparable/tint.c
@@@

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
If you want to know how to write codes without the macros, see [`tint_without_macro.c`](tcomparble/tint_without_macro.c).
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

@@@include
tcomparable/tstr.c
@@@

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

@@@include
tcomparable/main.c
@@@

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

Change your current directory to [src/tcomparable](comparable).

~~~
$ meson _build
$ ninja -C _build
~~~

Then execute it.

@@@shell
cd tcomparable; _build/tcomparable 2>&1
@@@

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
If you want to know that, see the source files in [src/tcomparable](tcomparable).

## GObject system and object oriented languages

If you know any object oriented languages, you probably have thought that GObject and the language are similar.
Learning such languages is very useful to know GObject system.

