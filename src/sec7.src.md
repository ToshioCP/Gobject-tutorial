# Derivable and non-abstract type

It is more common to make a non-abstract derivable type than abstract type.
This section covers how to make non-abstract derivable type objects.
An example in this section is an object for general pointer.
Children of the object can be string, array and so on, which are objects that use pointer.
The object has a property of a pointer.

## Header file

The name of the object is TPtr.
Its header file is as follows.

@@@include
tptr/tptr.h
@@@

- 7: Uses `G_DECLARE_DERIVABLE_TYPE` like the example in the previous section.
- 9-11: TPtrClass doesn't have its own class area.
- 13-18: Setter and getter.
- 21-25: Functions to create a TPtr object.

## C file

The C file of TPtr object is as follows.

@@@include
tptr/tptr.c
@@@

- 3-4: Property id and a static pointer for GParamSpec.
- 6-8: TPtrPrivate is a C structure.
It is a private data area for TPtr.
If TPtr were a final type, then no descendant would exist and TPtr instance could be a private data area.
But TPtr is derivable so you can't store such private data in TPtr instance that is open to the descendants.
The name of this structure is "<object name\>Private" like `TPtrPrivate`.
The structure must be defined before `G_DEFINE_TYPE_WITH_PRIVATE`.
- 10: `G_DEFINE_TYPE_WITH_PRIVATE` macro.
This macro expands to:
  - Declares `t_ptr_class_init` which is a class initialization function.
  - Declares `t_ptr_init` which is an instance initialization function.
  - Defines `t_ptr_parent_class` static variable.
It points to the parent class of TPtr.
  - Adds private instance data to the type.
It is a C structure and its name is `TPtrPrivate`. (See above).
  - Defines `t_ptf_get_type ()` function.
This function registers the type in its first call.
  - Defines a private instance getter `t_ptr_get_instance_private ()`.
This function has a parameter which is the pointer to the instance.
- 12-21: `t_ptr_set_property`.
This is similar to `t_int_set_property`, but the property value is stored in the private area.
- 15: Gets the pointer to `TPtrPrivate`.
- 18: Stores the pointer in `priv->pointer`.
- 23-32: `t_ptr_get_property`.
Gets the pointer to `TPtrPrivate` and retrieves the property value from `Priv->pointer`.
- 34-42: `t_ptr_class_init`
Initializes property related methods and GParamSpec.
Then installs the property.
- 44-48: `t_ptr_init`
It assigns NULL to `priv->pointer`.
- 50-64: Setter and getter functions.
They are convenient to set or get a pointer which is also a property.
`g_object_set` and `g_object_get` are fine.
But we have two reasons to have setter and getter.
  - Setter and getter is convenient.
  - It's impossible to use `g_object_get` in finalization process.
Children of this object need to free memory owned by them in their finalization process.
They use `t_ptr_get_pointer` to get the pointer to free it.
`t_ptr_get_pointer` doesn't use `g_object_get`.
- 51-54: `t_ptr_set_pointer` is a setter of "pointer" property.
It uses `g_object_set` so that "notify" signal is emitted.
- 59-64: `t_ptr_get_pointer` is a getter of "pointer" property.
It doesn't use `g_object_get`.
- 66-80: `t_ptr_new_with_pointer` and `t_ptr_new` create a new TPtr instance.

## main.c

`main.c` is a simple test program for TPtr object.

@@@include
tptr/main.c
@@@

- 4-14: "notify" signal handler.
It just shows that the value of the property is set.
- 16-31: `main` function.
- 24-25: Uses the setter function to check if the "notify::pointer" signal is emitted.
- 26: Checks the getter function.

## Compilation and execution

Change your current directory to `src/tptr` directory and type the following as usual.

~~~
$ meson _build
$ ninja -C _build
~~~

Execute `tptr`.

@@@shell
cd tptr; _build/tptr
@@@

## How to write a derivable type

- Use `G_DECLARE_DERIVABLE_TYPE` macro in the header file.
You need to write a macro like `#define T_TYPE_PTR (t_ptr_get_type ())` before `G_DECLARE_DERIVABLE_TYPE`.
- Declare your class structure in the header file.
The contents of the class are open to the descendants.
Most of the members are class methods.
- Use `G_DEFINE_TYPE_WITH_PRIVATE` in the C file.
You need to define the private area before `G_DEFINE_TYPE_WITH_PRIVATE`.
It is a C structure and the name is "<object name\>Private" like "TPtfPrivate".
- Define class and instance initialization functions.

