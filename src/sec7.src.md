# Derivable and non-abstract type

It is more common to make a non-abstract derivable type than abstract type.
This section covers how to make non-abstract derivable type objects.
A derivable type example is an object for string.
It is TStr.
And its child is an object for numeric string.
A numeric string is a string that expresses a number.
For example, "0", "-100" and "123.45".
The child object (numeric string) will be explained in the next section.

Before the derivable object, I'd like to write about strings.
Because TStr is an object of a string and we need to be careful to write a program with strings.

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

If the string `s` is no longer useless, `s` must be freed, which means the allocated 7 bytes are returned to the system.
`g_free` frees the memory.

Strings bounded by double quotes like "Hello." is a string literal.
It is an array of characters, but the contents of the array is not allowed to change.
And it mustn't be freed.
If you write a character in a string literal or free a string literal, the result is undefined.
Maybe bad thing will happen, for example, a segmentation fault error.

There's a difference between arrays and pointers when you initialize them with string literal.
If an array is initialized with string literal, the array can be changed.

~~~C
char a[]="Hello!";
a[1]='a';
g_print ("%s\n", a); /* Hallo will appear in your display. */
~~~

The first line initializes an array `a`.
The initialization is not simple.
First, the compiler calculates the length of "Hello!".
It is 7 because the string literal has '\0' at the end of it.
Then 7 bytes memory is allocated in static memory or stack memory.
It depends on the class of the array, whether `static` or `auto`.
The memory is initialized with "Hello!".
So, the string in the array can me changed.
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
- stack.
If the class of an array is `auto`, then the array is placed in the stack.
stack is writable.
If the array is defined in a function, its default class is `auto`.
The stack area will disappear when the function returns to the caller.
- static area.
If the class of an array is `static`, then the array is placed in the static area.
It keeps its value and remains for the life of the program.
This area is writable.
- heap.
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

`g_strdup` allocates memory and initializes it with "Hello", then returns the pointer to the memory.
The function `g_strdup` is almost same as the `string_dup` function below.

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
TStr has a string type property.

The header file `tstr.h` is as follows.

@@@include
tstr/tstr.h
@@@

- 7: Uses `G_DECLARE_DERIVABLE_TYPE` like the example in the previous section.
- 9-11: TStrClass doesn't have its own class area.
But you can add some members like you did in `TNumber`.
- 13-14: `t_str_concat` connects two strings of TStr instances and returns a new TStr instance.
- 17-21: Setter and getter.
- 24-28: Functions to create a TStr object.

## C file

The C file of TStr object is as follows.
It is `tstr.c`.

@@@include
tstr/tstr.c
@@@

- 3-9: `enum` defines a property id.
`PROP_STRING` is the id of "string" property.
Only one property is defined here, so you can define it without `enum`.
However, `enum` can be applied to define two or more properties.
So, this way is more recommended.
In the same way, an array `str_properties` is used.
it stores a static pointer for GParamSpec.
- 11-13: TStrPrivate is a C structure.
It is a private data area for TStr.
If TStr were a final type, then no descendant would exist and TStr instance could be a private data area.
But TStr is derivable so you can't store such private data in TStr instance that is open to the descendants.
The name of this structure is "<object name\>Private" like `TStrPrivate`.
The structure must be defined before `G_DEFINE_TYPE_WITH_PRIVATE`.
- 15: `G_DEFINE_TYPE_WITH_PRIVATE` macro.
This macro expands to:
  - Declares `t_str_class_init` which is a class initialization function.
  - Declares `t_str_init` which is an instance initialization function.
  - Defines `t_str_parent_class` static variable.
It points to the parent class of TStr.
  - Adds private instance data to the type.
It is a C structure and its name is `TStrPrivate`. (See above).
  - Defines `t_str_get_type ()` function.
This function registers the type in its first call.
  - Defines a private instance getter `t_str_get_instance_private ()`.
This function has a parameter which is the pointer to the instance.
- 17-29: `t_str_set_property`.
This is similar to `t_int_set_property`, but the property value is stored in the private area.
- 20: Gets the pointer to `TStrPrivate` and assigns it to `priv`.
- 24-25: `priv->string` is a pointer to the string TStr holds.
Before updating the pointer, the previous string need to be freed.
- 26: Stores the string in `priv->string`.
The function `g_value_get_string` returns the pointer to the string that GValue owns.
You can't own the string.
That means you can't change or free the string.
Therefore, it is necessary to duplicate the string before it is stored in `prive->string`.
If you don't duplicate it, `priv->string` may be changed or freed by the GValue later.
- 31-40: `t_str_get_property`.
Gets the pointer to `TStrPrivate` and assigns it to `priv`.
Then it retrieves the string from `priv->string`, duplicates it and set the GValue `value` with the string.
Notice that the function `g_strdup` is necessary as well.
- 42-50: `t_str_finalize` is called when TStr instance is destroyed.
This function frees the string `priv->string`.
After that, it calls the parent's finalize method.
This is called "chain up to its parent" and it will be explained in the next section.
- 52-57: `t_str_init` initializes `priv->string`.
- 59-68: `t_str_class_init` function initializes the class of TStr object.
- 63: Overrides `finalize` method.
This method is called in the destruction process.
Overrides `set_property` and `get_property` method.
Creates GParamSpec.
Then installs the property.
- 71-86: Setter and getter functions.
General setting/getting property functions `g_object_set` and `g_object_get` are fine.
But the setter and getter are convenient and user friendly.
- 88-111 `t_str_concat` function.
It concatenates the string of `self` and `other` and creates a new TStr that has the new string.
- 114-122: `t_str_new_with_string` and `t_str_new` create a new TStr instances.

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

