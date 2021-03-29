# Type system and registration process

GObject is a base object.
We don't use gobject itself usually.
Because GObject is very simple and not enough to be used by itself in most situations.
Instead, we use descendant objects of GObject such as many kinds of GtkWidget.
We can rather say such derivability is the most important feature of GObject.

This section describes how to define a child object of GObject.

## Name convention

An example of this section is an object represents a real number.
It is not so useful because we have already had double type in C language to represent real numbers.
However, I think this example is not so bad for knowing the technique to define a child object.

First, you need to know the naming convention.
An object name consists of name space and name.
For example, "GObject" consists of a name space "G" and a name "Object".
"GtkWidget" consists of a name space "Gtk" and a name "Widget".
Let the name space be "T" and the name be "Double" of the new object.
In this tutorial, we use "T" as a name space for all the objects we make.

TDouble is the object name.
It is a child object of GObject.
It represents a real number and the type of the number is double.
It has some useful functions.

## Define TDoubleClass and TDouble

TDouble type object has class and instance.
The type of the class is TDoubleClass.
Its structure is like this:

~~~C
typedef struct _TDoubleClass TDoubleClass
struct _TDoubleClass {
  GObjectClass parent_class;
}
~~~

\_TDoubleClass is a C structure tag name and TDoubleClass is "struct _TDoubleClass".

- Use typedef to define a class type.
- The first member of the structure must be the parent's class structure.

TDoubleClass doesn't need its own member.

The structure type of the instance of TDouble is TDouble type.

~~~C
typedef struct _TDouble TDouble
struct _TDouble {
  GObject parent;
  double value;
}
~~~

This is similar to the structure of the class.

- Use typedef to define an instance type.
- The first member of the structure must be the parent's instance structure.

TDouble has its own member, "value".
It is the value of TDouble instance.

The coding convention above needs to be kept by writers.

## Creation process of a child of GObject

The creation process of TDouble type is similar to the one of GObject.

1. Register TDouble type with type system.
2. Let the type system allocate memory for TDoubleClass and TDouble.
3. Initialize TDoubleClass.
4. Initialize TDouble.

## Registration

Usually registration is done by convenient macro such as `G_DECLARE_FINAL_TYPE` and `G_DEFINE_TYPE`.
So you don't need to care about registration.
But, in this tutorial, it is important to understand GObject type system, so I want to show you the registration without macro, first.

There are two kinds of types, static and dynamic.
Static type doesn't destroy its class even all the instances have been destroyed.
Dynamic type destroys its class when the last instance has been destroyed.
The type of GObject is static and its descendant objects' type is also static.
The function `g_type_register_static` registers a type of static object.
The following code is extracted from `gtype.h` in the Glib source files.

~~~C
GType
g_type_register_static (GType           parent_type,
                        const gchar     *type_name,
                        const GTypeInfo *info,
                        GTypeFlags      flags);
~~~

The parameters above are:

- parent_type: Parent type.
- type_name: The name of the type.
For example, "TDouble".
- info: Information of the type.
`GTypeInfo` structure will be explained below.
- flags: Flag.
If the type is abstract type or abstract value type, then set their flag.
Otherwise, set it to zero.

Because the type system maintains the parent-child relationship of the type, `g_type_refister_static` has a parent type parameter.
And the type system also keeps the information of the type.
After the registration, `g_type_register_static` returns the type of the new object.

`GTypeInfo` structure is defined as follows.

~~~C
typedef struct _GTypeInfo  GTypeInfo;

struct _GTypeInfo
{
  /* interface types, classed types, instantiated types */
  guint16                class_size;

  GBaseInitFunc          base_init;
  GBaseFinalizeFunc      base_finalize;

  /* interface types, classed types, instantiated types */
  GClassInitFunc         class_init;
  GClassFinalizeFunc     class_finalize;
  gconstpointer          class_data;

  /* instantiated types */
  guint16                instance_size;
  guint16                n_preallocs;
  GInstanceInitFunc      instance_init;

  /* value handling */
  const GTypeValueTable  *value_table;
};
~~~

This structure needs to be created before the registration.

- class_size: The size of the class.
For example, TDouble's class size is `sizeof (TDoubleClass)`.
- base_init, base_finalize: These function initialize/finalize the dynamic members of the class.
In many cases, they aren't necessary, and are assigned with NULL.
For further information, see [GObject API reference](https://developer.gnome.org/gobject/stable/gobject-Type-Information.html#GClassInitFunc).
- class_init: Initializes static members of the class.
Assign your class initialization function to here.
By convention, the name is `<name space>_<name>_class_init`, for example, `t_double_class_init`.
- class_finalize: Finalizes the class.
Because descendant type of GObjec is static, its `class_finalize` member is assigned with NULL.
- class_data: User-supplied data passed to the class init/finalize functions.
Usually NULL is assigned.
- instance_size: The size of the instance.
For example, TDouble's instance size is `sizeof (TDouble)`.
- n_preallocs: This is ignored. it has been used by the old version of Glib.
- instance_init: Initializes instance members.
Assign your instance initialization function to here.
By convention, the name is `<name space>_<name>_init`, for example, `t_double_init`.
- value_table: This is usually only useful for fundamental types.
If the type is descendant of GObject, assign NULL.

These information is kept by the type system and used when the object is created or destroyed.
Class\_size and instance\_size are used to allocate memory for the class and instance.
Class\_init and instance\_init functions are called when class or instance is initialized.

`example3.c` shows how to use `g_type_register_static`.

@@@include
misc/example3.c
@@@

- 16-22: Class initialization function and instance initialization function.
They do nothing here but they are necessary for the registration.
- 24-43: `t_double_get_type` function.
This function returns the type of TDouble object.
The name of a function is always `<name space>_<name>_get_type`.
And a macro `<NAME_SPACE>_TYPE_<NAME>` (all characters are upper case) is replaced by this function.
Look at line 3.
`T_TYPE_DOUBLE` is a macro replaced by `t_double_get_type ()`.
This function has a static variable `type` to keep the type of the object.
At the first call of this function, `type` is zero.
Then it calls `g_type_register_static` to register the object to the type system.
At the second or subsequent call, the function just return `type`, which is non-zero because the variable class is static.
- 30-40 : Sets `info` structure and calls `g_type_register_static`.
- 45-63: Main function.
Gets the type of TDouble object and displays it.
`g_object_new` is used to instantiate the object.
Shows the address of the instance.

`example3.c` is in the [src/misc](misc) directory.

Execute it.

@@@shell
cd misc; _build/example3
@@@

## G_DEFINE_TYPE macro

The registration above is done with stereotyped algorithm.
Therefore, it can be defined as a macro such as `G_DEFINE_TYPE`.

`G_DEFINE_TYPE` does the following:

- Declares a class initialization function.
Its name is `<name space>_<name>_class_init`.
For example, if the object name is `TDouble`, it is `t_double_class_init`.
This is a declaration, not a definition.
A user needs to define it.
- Declares a instance initialization function.
Its name is `<name space>_<name>_init`.
For example, if the object name is `TDouble`, it is `t_double_init`.
This is a declaration, not a definition.
A user needs to define it.
- Defines a static variable pointing to the parent class.
Its name is `<name space>_<name>_parent_class`.
For example, if the object name is `TDouble`, it is `t_double_parent_class`.
- Defines a `<name space>_<name>_get_type ()` function.
(This is often written as `*_get_type()` function.)
For example, if the object name is `TDouble`, it is `t_double_get_type`.
The registration is done in this function like the previous subsection.

Using this macro reduces lines of the program.
See the following sample `example4.c` which works the same as `example3.c`.

@@@include
misc/example4.c
@@@

Thanks to `G_DEFINE_TYPE`, we are freed from writing bothersome code like `GTypeInfo` and `g_type_register_static`.
One important thing to be careful is to follow the convention of the naming of init functions.

Execute it.

@@@shell
cd misc; _build/example4
@@@

## G_DECLARE_FINAL_TYPE macro

Another useful macro is `G_DECLARE_FINAL_TYPE` macro.
This macro can be used for a final type.
A final type doesn't have any children.
If a type has children, it is a derivable type.
If you want to define a derivable type object, use `G_DECLARE_DERIVABLE_TYPE` instead.
However, you probably write final type objects in most cases.

`G_DECLARE_FINAL_TYPE` does the following:

- Declares `<name space>_<name>_get_type ()` function.
This is only declaration.
You need to define it.
But you can use `G_DEFINE_TYPE`, its expansion includes the definition of the function.
So, you actually don't need to write the definition by yourself.
- The C type of the object is defined as a typedef of structure.
For example, if the object name is `TDouble`, then `typedef struct _TDouble TDouble` is included in the output.
But you need to define the structure `struct _TDouble` by yourself before `G_DEFINE_TYPE`.
- `<NAME SPACE>_<NAME>` macro is defined.
For example, if the object is `TDouble` the macro is `T_DOUBLE`.
It will be expanded to a function which casts the argument to the pointer to the object.
For example, `T_Double (obj)` casts the type of `obj` to `TDouble *`.
- `<NAME SPACE>_IS_<NAME>` macro is defined.
For example, if the object is `TDouble` the macro is `T_IS_DOUBLE`.
It will be expanded to a function which checks if the argument is the instance of `T_TYPE_DOUBLE`.
It returns true if the argument is a descendant of `T_TYPE_DOUBLE`.
- The class structure is defined.
A final type object doesn't need to have its own member of class structure.
The definition is like the line 11 to 14 in the `example4.c`.

You need to write the macro definition of the type of the object before `G_DECLARE_FINAL_TYPE`.
For example, if the object is `TDouble`, then

~~~C
#define T_TYPE_DOUBLE  (t_double_get_type ())
~~~

needs to be defined before `G_DECLARE_FINAL_TYPE`.

`example5.c` uses this macro.
It works like `example3.c` or `example4.c`.

@@@include
misc/example5.c
@@@

Execute it.

@@@shell
cd misc; _build/example5
@@@

## Separate the file into main.c, tdouble.h and tdouble.c

Now it's time to separate the contents into three files, `main.c`, `tdouble.h` and `tdouble.c`.
An object is defined with two files, header file and C source file.

tdouble.h

@@@include
tdouble1/tdouble.h
@@@

- The contents of header files are public, i.e. it is open to any files.
Header files include macros, which are giving type information, cast and type check, and functions.
- 1,2,18: These directives prevent that the header file is read two or more times.
- 6,7: `T_TYPE_DOUBLE` is public.
`G_DECLARE_FINAL_TYPE` is also expanded to public definitions.
- 9-13: Function declarations.
They are getter and setter of the value of the object.
- 15-16: Object instantiation function.

tdouble.c

@@@include
tdouble1/tdouble.c
@@@

- 3-6: Declaration of the instance structure.
Since `G_DECLARE_FINAL_TYPE` macro emits `typeder struct _TDouble TDouble`, the tag name of the structure must be `_TDouble`.
- 8: `G_DEFINE_TYPE` macro.
- 10-16: class and instance initialization functions.
At present, they don't do anything.
- 18-24: Getter. The argument `value` is a pointer to double type variable.
Set the variable with the object value (`d->value`).
If it succeeds, it returns TRUE.
`g_return_val_if_fail` is used to check the argument type.
If the argument `d` is not TDouble type, it outputs error to the log and immediately returns FALSE.
This function is used to report a programmer's error.
You shouldn't use it for a runtime error.
See [Glib API reference](https://developer.gnome.org/glib/stable/glib-Warnings-and-Assertions.html#g-return-val-if-fail) for further information.
`g_return_val_if_fail` isn't used in static type functions because static functions are called only from other functions in the same file.
Such functions know the parameters type well.
`g_return_val_if_fail` i used in public functions.
- 26-31: Setter.
`g_return_if_fail` function is used to check the argument type.
This function doesn't return any value.
Because the type of `t_double_set_value` is `void` so no value will be returned.
Therefore, we use `g_return_if_fail`, not `g_return_val_if_fail`.
- 33-40: Object instantiation function.
It has one parameter `value` to set the value of the object.
- 37: This function uses `g_object_new` to instantiate the object.
The argument `T_TYPE_DOUBLE` is expanded to a function `t_double_get_type ()`.
If this is the first call for `t_double_get_type`, the registration will be carried out.

main.c

@@@include
tdouble1/main.c
@@@

- 2: Includes `tdouble.h`.
This is necessary for accessing TDouble object.
- 9: Instantiate TDouble object and set `d` to point the object.
- 10-13: Tests the getter of the object.
- 15-20: Tests the setter of the object.

The source files are located in [src/tdouble1](tdouble1).
Change your current directory to the directory above and type the following.

~~~
meson _build
ninja -C _build
~~~

Then, execute the program.

@@@shell
cd tdouble1; _build/example6
@@@

This example is very simple.
But any object has header file and C source file as the example above has.
And they follow the convention.
You probably aware of the importance of the convention.
For the further information refer to [GObject API reference](https://developer.gnome.org/gobject/stable/gtype-conventions.html).

