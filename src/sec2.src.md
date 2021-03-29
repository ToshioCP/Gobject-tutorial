# GObject

## Class and instance

GObject instance is created with `g_object_new` function.
GObject has not only instance but class.
It is GObjectClass.

- GObjectClass is created at the first call of `g_object_new`.
And only one GObjectClass exists.
- GObject instance is created whenever `g_object_new` is called.
So, two or more GObject can exist.

GObject and GObjectClass are C structures.
They are declared in `gobject.h`, which is one of the glib source files.
You can download the source file from [Gnome download page](https://download.gnome.org/sources/glib/).

~~~C
typedef struct _GObject                  GObject;
typedef struct _GObjectClass             GObjectClass;
~~~

The members of the structures are also declared in `gobject.h`, but it is not important now.

There are sample programs in [src/misc](misc) directory.
You can compile them by:

~~~
$ cd src/misc
$ meson _build
$ ninja -C _build
~~~

One of the programs is `example1.c`.
Its code is as follows.

@@@include
misc/example1.c
@@@

- 5: `instance1` and `instance2` are pointers that points GObject instance.
`class1` and `class2` points GObjecectClass.
- 8-11: A function `g_object_new` creates a GObject instance.
GObject instance is a chunk of memory which is GObject structure (`struct _GObject`).
The argument `G_TYPE_OBJECT` represents the type of GObject.
This type is different from C language type like `char`  or `int`.
There is *type system* which is a base system of gobject system.
Every data type such as GObject must be registered to the type system.
The type system has series of functions for the registration.
If one of the functions is called, then the type system determines `GType` type value for the object and returns it to the caller.
`GType` is an unsigned long integer on my computer but it depends on the hardware.
`g_object_new` allocates necessary memory to GObject instance and returns the pointer to the top address of the memory.
After the creation, this program displays the addresses of instances.
- 13-16: A macro `G_OBJECT_GET_CLASS` returns the pointer to the class of the argument.
Therefore, `class1` points the class (GObjectClass) of `instance1` and `class2` points the class of `instance2` respectively.
The addresses of the two classes are displayed.
- 18-19: `g_object_unref` will be explained in the next subsection.
It destroys the objects and the memory are freed.

Now, execute it.

@@@shell
cd misc; _build/example1
@@@

The locations of two instances `instance1` and `instance2` are different.
Each instance has its own memory.
The locations of two classes `class1` and `class2` are the same.
Two GObject instances share the same GObjectClass.

![Class and Instance](../image/class_instance.png){width=10cm height=7.5cm}

## Reference count

GObject instance has its own memory.
They are allocated by the system when it is created.
If it becomes useless, the memory must be freed.
However, how can we determine whether it is useless?
GObject system provides reference count to solve the problem.

Instance is created and used by someone (some other objects, some functions or something like that).
We say someone refers to the instance.
If the instance is referred by A and B, then the number of the reference is two.
This is reference count.
Let's think about a scenario like this: 

- A calls `g_object_new` and owns an instance G.
A refers G, so the reference count of G is 1.
- B wants to use G too.
B calls `g_object_ref` and increases the reference count by 1.
Now the reference count is 2.
- A no longer uses G.
A calls `g_object_unref` and decreases the reference count by 1.
Now the reference count is 1.
- B no longer uses G.
B calls `g_object_unref` and decreases the reference count by 1.
Now the reference count is 0.
- Because the reference count is zero, G knows that no one refers to it.
G begins finalizing process by itself.
G disappears and the memory are freed.

A program `example2.c` is based on the scenario above.

@@@include
misc/example2.c
@@@

Now execute it.

@@@shell
cd misc; _build/example2
@@@

`example2` shows:

- `g_object_new` creates a new GObject instance and sets its reference count to 1.
- `g_object_ref` increases the reference count by 1.
- `g_object_unref` decreases the reference count by 1.
If the reference count drops to zero, the instance destroys itself.

## Initialization and destruction process

The actual process of GObject initialization and destruction process is very complex.
The following is simplified description without details.

Initialization

1. Registers GObject type with the type system.
This is done in the glib initialization process before the function `main` is called.
(If the compiler is gcc, then `__attribute__ ((constructor))` is used to qualify the initialization function.
Refer to [GCC manual](https://gcc.gnu.org/onlinedocs/gcc-10.2.0/gcc/Common-Function-Attributes.html#Common-Function-Attributes).)
2. Allocates memory for GObjectClass and GObject structure.
3. Initializes GObjectClass structure.
4. Initializes GObject structure.

This initialization process is carried out when `g_object_new` function is called for the first time.
At the second and subsequent call for `g_object_new`, it performs only two processes: (1) memory allocation for GObject structure (2) initialization for GObject structure.

Destruction

1. Destroys GObject structure. The memory for GObject is freed.

GObject type is a static type.
Static type never destroys its class.
So, even if the destroyed instance is the last instance, the class still remains.

When you write code to define a child object of GObject, It is important to understand the process above.
The detailed process will be explained in the later sections.

