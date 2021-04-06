# Derivable type and abstract type

## Derivable type

There are two kinds of types, final type and derivable type.
Final type doesn't have any child object.
Derivable type has child objects.

The main difference between two objects above is their class.
Final type objects doesn't have its own class area.
The only member of the class is its parent class.

Derivable object has its own area in the class.
They are open to its descendants.

`G_DECLARE_DERIVABLE_TYPE` is used to define derivable type.
It is written in a header file like this:

~~~C
#define T_TYPE_NUMBER             (t_number_get_type ())
G_DECLARE_DERIVABLE_TYPE (TNumber, t_number, T, NUMBER, GObject)
~~~

## Abstract type

Abstract type doesn't have any instance.
This type of object is derivable and its children can use functions and signals of the abstract object.

The example of this section is TNumber, TInt and TDouble object.
TInt and TDouble have already made in the previous section.
They represent integer and floating point respectively.
Numbers are abstract in computers.
Integer and floating point are numbers, but more specific.
TNumber is an abstract object which represents numbers.

TNumber is a parent object of TInt and TDouble.
TNumber isn't instantiated because it's abstract type.
When an instance is TNumber type, it is an instance of TInt or TDouble as well.

TInt and TDouble have five operations: addition, subtraction, multiplication, division and unary minus operation.
Those operation can be defined on TNumber object.

In this section we will define TNumber object and five functions above.
And we will rewrite TInt and TDouble to implement the functions.

## TNumber object

`tnumber.h` is a header file for TNumber object.

@@@include
tnumber/tnumber.h
@@@

- 7: `G_DECLARE_DERIVABLE_TYPE` macro.
This is similar to `G_DECLARE_FINAL_TYPE` macro.
The difference is the object is derivable or final.
`G_DECLARE_DERIVABLE_TYPE` is expanded to:
  - Declaration of `t_number_get_type ()` function. This function must be defined in `tnumber.c` file. The definition is usually done with `G_DEFINE_TYPE` or its family macros.
  - Definition of TNumber instance, whose member is its parent only.
  - Declaration of TNumberClass. It should be defined later in the header file.
  - Convenient macros `T_NUMBER` (cast to instance), `T_NUMBER_CLASS` (cast to class), `T_IS_NUMBER` (instance check), `T_IS_NUMBER_CLASS` (class check) and `T_NUMBER_GET_CLASS` are defined.
  - `g_autoptr()` support.
- 9-19: Definition of the structure of TNumberClass.
- 11-16: These are pointers to functions.
They are called class methods or virtual functions.
They are expected to be overridden by the descendant object.
The methods are five arithmetic operators and `to_s` function.
`to_s` function is similar to sprintf function.
- 18: A pointer to the default signal handler of "div-by-zero" signal.
The offset of this pointer is given to `g_signal_new` as an argument.
- 21-40: Functions. They are public.

`tnumber.c` is below.

@@@include
tnumber/tnumber.c
@@@

- 5: `G_DEFINE_ABSTRACT_TYPE` macro.
This macro is used to define an abstract type object.
Abstract type isn't instantiated.
This macro is expanded to:
  - Declaration of `t_number_init ()` function.
  - Declaration of `t_number_class_init ()` function.
  - Definition of `t_number_get_type ()` function.
  - Definition of `t_number_parent_class ()` function
- 3, 7-10, 25-36: Defines division-by-zero signal.
`div_by_zero_default_cb` is a default handler of "div-by-zero" signal.
Default handler doesn't have user data parameter.
`g_signal_new` is used instead of `g_signal_new_class_handler`.
`g_signal_new` specifies a handler as the offset from the top of the class to the pointer to the handler.
- 12-36: `t_number_class_init`.
- 16-21: These class methods are virtual functions.
They are expected to be overridden in the descendant object of TNumber.
NULL is assigned here so that nothing happens when the methods are called.
- 23: Assigns the address of the function `dev_by_zero_default_cb` to `class->div_by_zero`.
This is the default handler of "div-by-zero" signal.
- 38-40: `t_number_init` is a initialization function for an instance.
But abstract object isn't instantiated.
So, nothing is done in this function.
But you can't leave out the definition of this function.
- 42-76: Public functions.
These functions just call the corresponding class methods if the pointer to the class method is not NULL.

## TInt object.

`tint.h` is a header file of TInt object.
TInt is a child object of TNumber.

@@@include
tnumber/tint.h
@@@

- 10-16:Declares public functions.
Arithmetic functions are declared in TNumber, so TInt doesn't declare any arithmetic functions.
Only instance creation functions are declared.

`tint.c` implements virtual functions (class methods).
And the pointers of the methods in TNumberClass are rewritten here.

@@@include
tnumber/tint.c
@@@

- 5, 14-32, 128-131: Definition of the property "value".
This is the same as before.
- 7-10: Definition of the structure of TInt.
This must be defined before `G_DEFINE_TYPE`.
- 12: `G_DEFINE_TYPE` macro.
This macro expands to:
  - Declaration of `t_int_init ()` function.
  - Definition of `t_int_get_type ()` function.
  - Definition of `t_int_parent_class ()` function
- 34-36: `t_int_init`.
- 40-113: These functions are connected to the class method pointers in TIntClass.
They are the implementation of the virtual functions defined in `tnumber.c`.
- 40-49: Defines a macro used in `t_int_add`, `t_int_sub` and `t_int_mul` functions.
This macro is similar to `t_int_div` function.
refer to the explanation for `t_int_div`.
- 51-73: `t_int_add`, `t_int_sub` and `t_int_mul` functions.
The macro `t_int_binary_op` is used.
- 75-97: `t_int_div`.
`self` is the object on which the function is called.
`other` is another TNumber object.
It can be TInt or TDouble.
IF it is TDouble, its value is casted to int before the division is performed.
If the divisor (`other`) is zero, "div-by-zero" signal is emitted.
The signal is defined in TNumber, so TInt doesn't know the signal id.
The emission is done with `g_signal_emit_by_name` instead of `g_signal_emit`.
The return value of `t_int_div` is TNumber type object
However, because TNumber is abstract, the actual type of the object is TInt.
- 99-104: A function with unary minus operator.
- 106-113: `to_s` function. This function converts int to string.
For example, if the value of the object is 123, then the result is a string "123".
- 115- 132: `t_int_class_init`.
- 121-126: Overrides the class methods.
For example, if `t_number_add` is called on a TInt object, then the function calls the class method `*tnumber_class->add`.
The pointer points `t_int_add` function.
Therefore, `t_int_add` is called finally.
- 134-148: Instance creation functions are the same as before.

## TDouble object.

TDouble object is defined with `tdouble.h` and `tdouble.c`.
The definition is very similar to TInt.
So, this subsection just shows the contents of the files.

tdouble.h

@@@include
tnumber/tdouble.h
@@@

tdouble.c

@@@include
tnumber/tdouble.c
@@@

## main.c

`main.c` is a simple program to test the objects.

@@@include
tnumber/main.c
@@@

- 6-20: "notify" handler.
This handler is upgraded to support both TInt and TDouble.
- 22-71: A function `main`.
- 30-31: Connects the notify signals on `i` (TInt) and `d` (TDouble).
- 33-34: Set "value" properties on `i` and `d`.
- 36: Add `d` to `i`.
The answer is TInt object.
- 47: Add `i` to `d`.
The answer is TDouble object.
The addition of two TNumber objects isn't commutative because the type the result will be different if the two objects are exchanged.
- 56-63: Tests division by zero signal.

## Compilation and execution

`meson.buld` controls the compilation process.

@@@include
tnumber/meson.build
@@@

Compilation and execution is done by the usual way.

~~~
$ meson _build
$ ninja -C _build
$ _build/tnumber
~~~

Then the following is shown on the display.

@@@shell
cd tnumber; _build/tnumber
@@@

The two answers are different because of the different types.

This section has shown a simple example of derivable and abstract class.
You can define your derivable object like this.
If your object isn't abstract, use `G_DEFINE_TYPE` instead of `G_DEFINE_ABSTRACT_TYPE`.
And you need one more thing, how to manage private data in your derivable object.
There is a tutorial in [GObject API reference](https://developer.gnome.org/gobject/stable/howto-gobject.html).
See the tutorial for learning derivable object.

It is also good to see source files in Gtk.
The stable version is now Gtk4 but most of the distributions still use Gtk3.
Both of them are useful to learn how to write a derivable object.

## Class initialization process

### Initialization process of TNumberClass

1. GObjectClass has been initialized before the function `main` starts.
2. First call for `g_object_new (T_TYPE_NUMBER, ...)` initializes TNumberClass.
And the initialization process is as follows.
3. Memory is allocated for TNumberClass.
4. The parent (GObjectClass) part of the class is copied from GObjectClass.
5. `t_number_class_init` is called to initialize TNumberClass.
This includes initializing pointers to class method handlers and a default signal handler.

The diagram below shows the process.

![TNumberClass initialization](../image/tnumberclass_init.png)

### Initialization process of TIntClass

1. GNumberClass has been initialized before the initialization of GIntClass starts.
2. First call for `g_object_new (T_TYPE_INT, ...)` initializes TIntClass.
And the initialization process is as follows.
3. Memory is allocated for TIntClass.
TIntClass doesn't have its own area.
Therefore its structure is the same as its parent class (TNumberClass).
4. The parent (TNumberClass) part of the class (This is the same as whole TIntClass) is copied from TNumberClass.
5. `t_int_class_init` is called to initialize TIntClass.
This includes overriding class methods, set\_property and get\_property.

The diagram below shows the process.

![TIntClass initialization](../image/tintclass_init.png)

