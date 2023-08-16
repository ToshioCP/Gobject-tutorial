# Derivable and non-abstract type

It is more common to make a non-abstract derivable type than abstract type.
This section covers how to make non-abstract derivable type objects.
A derivable type example is an object for string.
It is TStr.
And its child is an object for numeric string.
A numeric string is a string that expresses a number.
For example, "0", "-100" and "123.45".
The child object (numeric string) will be explained in the next section.

This section describes memory management for strings before derivable objects.

## String and memory management

TStr has a string type value.
It is similar to TInt or TDouble but string is more complex than int and double.
When you make TStr program, you need to be careful about memory management, which is not necessary to TInt and TDouble.

### String and memory

String is an array of characters that is terminated with '\0'.
String is not a C type such as char, int, float or double.
But the pointer to a character array behaves like a string type of other languages.
So, we often call the pointer string.

If the pointer is NULL, it points nothing.
So, the pointer is not a string.
Programs with string will include bugs if you aren't careful about NULL pointer.

Another annoying problem is memory allocation.
Because string is an array of characters, memory allocation is necessary to create a new string.
We don't forget to allocate memory, but often forget to free the memory.
It causes memory leak.

~~~C
char *s;
s = g_strdup ("Hello.");
... ... ... do something with s
g_free (s);
~~~

`g_strdup` duplicates a string.
It does:

- Calculates the size of the string.
The size of "Hello." is 7 because strings are zero-terminated.
The string is an array 'H', 'e', 'l', 'l', 'o', '.' and zero ('\0').
- Requests the system to allocate 7 bytes memories.
- Copies the string to the memory.
- Returns the pointer to the newly-allocated memory.
- If the argument is NULL, then no memory is allocated and it returns NULL.

If the string `s` is no longer in use, `s` must be freed, which means the allocated 7 bytes must be returned to the system.
`g_free` frees the memory.

Strings bounded by double quotes like "Hello." are string literals.
They are an array of characters, but the contents of the array are not allowed to change.
And they mustn't be freed.
If you write a character in a string literal or free a string literal, the result is undefined.
Maybe bad things will happen, for example, a segmentation fault error.

There's a difference between arrays and pointers when you initialize them with a string literal.
If an array is initialized with a string literal, the array can be changed.

~~~C
char a[]="Hello!";
a[1]='a';
g_print ("%s\n", a); /* Hallo will appear on your display. */
~~~

The first line initializes an array `a`.
The initialization is not simple.
First, the compiler calculates the length of "Hello!".
It is seven because the string literal has '\0' at the end of it.
Then seven bytes memory is allocated in static memory or stack memory.
It depends on the class of the array, whether `static` or `auto`.
The memory is initialized with "Hello!".
So, the string in the array can be changed.
This program successfully displays `Hallo!.

The first line of the program above is the same as follows.

~~~C
char a[] = {'H', 'e', 'l', 'l', 'o', '!', '\0'};
~~~

If you define a pointer with string literal, you can't change the string pointed by the pointer.

~~~C
char *a = "Hello";
*(a+1) = 'a'; /* This is illegal. */
g_print ("%s\n", a);
~~~

The first line just assigns the address of the string literal to the variable `a`.
String literal is an array of characters but it's read-only.
It might be in the program code area or some other non-writable area.
It depends on the implementation of your compiler.
Therefore, the second line tries to write a char 'a' to the read-only memory and the result is undefined, for example, a segmentation error happens.
Anyway, don't write a program like this.

In conclusion, a string is an array of characters and it is placed in one of the following.

- Read-only memory.
A string literal is read-only.
- Stack.
If the class of an array is `auto`, then the array is placed in the stack.
stack is writable.
If the array is defined in a function, its default class is `auto`.
The stack area will disappear when the function returns to the caller.
- Static area.
If the class of an array is `static`, then the array is placed in the static area.
It keeps its value and remains for the life of the program.
This area is writable.
- Heap.
You can allocate and free memory for a string.
For allocation, `g_new` or `g_new0` is used.
For freeing, `g_free` is used.

### Copying string

There are two ways to copy a string.
First way is just copying the pointer.

~~~C
char *s = "Hello";
char *t = s;
~~~

Two pointers `s` and `t` points the same address.
Therefore, you can't modify `t` because `t` points a string literal, which is read-only.

Second way is creating memory and copying the string to the memory.

~~~C
char *s = "Hello";
char *t = g_strdup (s);
~~~

The function `g_strdup` allocates memory and initializes it with "Hello", then returns the pointer to the memory.
The function `g_strdup` is almost same as the function `string_dup` below.

~~~C
#include <glib-object.h>
#include <string.h>

char *
string_dup (char *s) {
  int length;
  char *t;

  if (s == NULL)
    return NULL;
  length = strlen (s) + 1;
  t = g_new (char, length);
  strcpy (t, s);
  return t;
}
~~~

If `g_strdup` is used, the two pointers `s` and `t` point different memories.
You can modify `t` because it is placed in the memory allocated from the heap area.

It is important to know the difference between assigning pointers and duplicating strings.

### const qualifier

The qualifier `const` makes a variable won't change its value.
It can also be applied to an array.
Then, the elements of the array won't be changed.

~~~C
const double pi = 3.1415926536;
const char a[] = "read only string";
~~~

An array parameter in a function can be qualified with `const` to indicate that the function does not change the array.
In the same way, the return value (a pointer to an array or string) of a function can be qualified with `const`.
The caller mustn't modify or free the returned array or string.

~~~C
char *
string_dup (const char *s) {
  ... ...
}

const char *
g_value_get_string (const GValue *value);
~~~

The qualifier `const` indicates who is the owner of the string when it is used in the function of objects.
"Owner" is an object or a caller of the function that has the right to modify or free the string.

For example, `g_value_get_string` is given `const GValue *value` as an argument.
The GValue pointed by `value` is owned by the caller and the function doesn't change or free it.
The function returns a string qualified with `const`.
The returned string is owned by the object and the caller mustn't change or free the string.
It is possible that the string will be changed or freed by the object later.

## Header file

The rest of this section is about TStr.
TStr is a child of GObject and it holds a string.
The string is a pointer and an array of characters.
The pointer points the array.
The pointer can be NULL.
If it is NULL, TStr has no array.
The memory of the array comes from the heap area.
TStr owns the memory and is responsible to free it when it becomes useless.
TStr has a string type property.

The header file `tstr.h` is as follows.

@@@include
tstr/tstr.h
@@@

- 6: Uses `G_DECLARE_DERIVABLE_TYPE`.
The TStr class is derivable and its child class will be defined later.
- 8-12: TStrClass has one class method.
It is `set_string` member of the TStrClass structure.
This will be overridden by the child class `TNumStr`.
Therefore, Both TStr and TNumStr has `set_string` member in their classes but they point different functions.
- 14-15: The public function `t_str_concat` connects two strings of TStr instances and returns a new TStr instance.
- 18-22: Setter and getter.
- 25-29: Functions to create a TStr object.

## C file

The C file `tstr.c` for TStr is as follows.

@@@include
tstr/tstr.c
@@@

- 3-9: `enum` defines a property id.
The member `PROP_STRING` is the id of the "string" property.
Only one property is defined here, so it is possible to define it without `enum`.
However, `enum` can be applied to define two or more properties and it is more common.
The last member `N_PROPERTIES` is two because `enum` is zero-based.
An array `str_properties` has two elements since `N_PROPERTIES` is two.
The first element isn't used and it is assigned with NULL.
The second element will be assigned a pointer to a GParamSpec instance in the class initialization function.
- 11-13: TStrPrivate is a C structure.
It is a private data area for TStr.
If TStr were a final type, then no descendant would exist and TStr instance could be a private data area.
But TStr is derivable so you can't store such private data in TStr instance that is open to the descendants.
The name of this structure is "<object name\>Private" like `TStrPrivate`.
The structure must be defined before `G_DEFINE_TYPE_WITH_PRIVATE`.
- 15: `G_DEFINE_TYPE_WITH_PRIVATE` macro.
It is similar to `G_DEFINE_TYPE` macro but it adds the private data area for the derivable instance.
This macro expands to:
  - Declaration of `t_str_class_init` which is a class initialization function.
  - Declaration of `t_str_init` which is an instance initialization function.
  - Definition of `t_str_parent_class` static variable.
It points to the parent class of TStr.
  - The function call that adds private instance data to the type.
It is a C structure and its name is `TStrPrivate`.
  - Definition of `t_str_get_type ()` function.
This function registers the type in its first call.
  - Definition of the private instance getter `t_str_get_instance_private ()`.
- 17-28: The function `t_str_set_property` sets the "string" property and it is used by `g_object_set` family functions.
It uses `t_str_set_string` function to set the private data with the copy of the string from the GValue.
It is important because `t_str_set_string` calls the class method `set_string`, which will be overridden by the child class.
Therefore, the behavior of `t_str_set_property` function is different between TStr and TNumStr, which is a child of TStr.
The function `g_value_get_string` returns the pointer to the string that GValue owns.
So you need to duplicate the string and it is done in the function `t_str_set_string`.
- 30-40: The function `t_str_get_property` gets the "string" property and it is used by `g_object_get` family functions.
It just gives `priv->string` to the function `g_value_set_string`.
The variable `priv` points the private data area.
The second argument `priv->string` is owned by the TStr instance and the function `g_value_set_string` duplicates it to store in the GValue structure.
- 44-51 The function `t_str_real_set_string` is the body of the class method and pointed by `set_string` member in the class.
First, it gets the pointer to the private area with `t_str_get_instance_private` function.
If the instance holds a string, free it before setting it to a new string.
It copies the given string and assigns it to `priv->string`.
The duplication is important.
Thanks to that, the address of the string is hidden from the out of the instance.
- 53-61: The finalize function `t_str_finalize` is called when TStr instance is destroyed.
The destruction process has two phases, "dispose" and "finalize".
In the disposal phase, the instance releases instances.
In the finalization phase, the instance does the rest of all the finalization like freeing memories.
This function frees the string `priv->string` if necessary.
After that, it calls the parent's finalize method.
This is called "chain up to its parent" and it will be explained later in this section.
- 63-68: The instance initialization function `t_str_init` assigns NULL to `priv->string`.
- 70-81: The class initialization function `t_str_class_init` overrides `finalize`, `set_property` and `get_property` members.
It creates the GParamSpec instance with `g_param_spec_string` and installs the property with `g_object_class_install_properties`.
It assigns `t_str_real_set_string` to the member `set_string`.
It is a class method and is expected to be replaced in the child class.
- 84-101: Setter and getter.
The setter method `t_str_set_string` just calls the class method.
So, it is expected to be replaced in the child class.
It is used by property set function `t_str_set_property`.
So the behavior of property setting will change in the child class.
The getter method `t_str_get_string` just duplicates the string `priv->string` and return the copy.
- 103-126: The public function `t_str_concat` concatenates the string of `self` and `other`, and creates a new TStr.
- 129-137: Two functions `t_str_new_with_string` and `t_str_new` create a new TStr instances.

## Chaining up to its parent

The "chain up to its parent" process is illustrated with the diagram below.

![Chaining up process in GObject and TStr](../image/chainup.png){width=10.5cm height=9cm}

There are two classes, GObjectClass and TStrClass.
Each class has their finalize methods (functions) pointed by the pointers in the class structures.
The finalize method of TStrClass finalizes its own part of the TStr instance.
At the end of the function, it calls its parent's finalize method.
It is the finalize method of GObjectClass.
It calls its own finalize function and finalizes the GObject private data.

If the GObjectClass has two or more descendant classes, the number of the finalize functions may be the same as the number of the descendants.
And they are connected by "chain up to its parent" way.  

![Chaining up process](../image/chainup3.png){width=12cm height=9cm}

## How to write a derivable type

- Use `G_DECLARE_DERIVABLE_TYPE` macro in the header file.
You need to write a macro like `#define T_TYPE_STR (t_str_get_type ())` before `G_DECLARE_DERIVABLE_TYPE`.
- Declare your class structure in the header file.
The contents of the class are open to the descendants.
Most of the members are class methods.
- Use `G_DEFINE_TYPE_WITH_PRIVATE` in the C file.
You need to define the private area before `G_DEFINE_TYPE_WITH_PRIVATE`.
It is a C structure and the name is "<object name\>Private" like "TStrPrivate".
- Define class and instance initialization functions.

