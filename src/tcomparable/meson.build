project ('tcomparable', 'c')

gobjdep = dependency('gobject-2.0')

sourcefiles=files('main.c', 'tnumber.c', 'tint.c', 'tdouble.c', 
                  '../tptr/tptr.c', 'tstr.c', 'tcomparable.c')

executable('tcomparable', sourcefiles, dependencies: gobjdep, install: false)

